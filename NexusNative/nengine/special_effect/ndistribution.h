#ifndef _NEXUS_DISTRIBUTION_H_
#define _NEXUS_DISTRIBUTION_H_

namespace nexus
{
	//-- Distribution基类 -----------------------------------------------------
	enum ndistribution_type
	{
		distribution_const=0,		//恒值
		distribution_uniform,		//随机值
		distribution_curve,			//曲线插值
	};

	//	描述某种类型的值随时间的变化
	template<typename T>
	class ndistribution
	{
	public:
		typedef boost::shared_ptr<ndistribution> ptr;

		virtual nstring get_type() = 0;
		virtual void serialize(narchive& ar) = 0;
		virtual T get_value(float time) = 0;
		virtual void sort()=0;
	};

	//-- 常数值 -------------------------------------------------------------
	template<typename T>
	class ndistribution_const : public ndistribution<T>
	{
	public:
		T	m_value;
	
		ndistribution_const(const T& val):m_value(val)
		{}

		ndistribution_const(void)
		{
			m_value = T();
		}

		virtual nstring get_type()
		{
			return _T("ndistribution_const");
		}

		virtual T get_value(float time)
		{
			(void)time;
			return m_value;
		}

		virtual void serialize(narchive& ar)
		{
			nSERIALIZE(ar, m_value);
		}

		virtual void sort()
		{

		}
	};

	//-- 在两个值之间进行随机 -------------------------------------------------------------------------------
	inline float _get_rand(float max_val, float min_val)
	{
		return ((float)rand()/RAND_MAX)*(max_val-min_val)+min_val;
	}

	inline vector2 _get_rand(const vector2& max_val, const vector2& min_val)
	{
		return vector2( _get_rand(max_val.x, min_val.x),
			_get_rand(max_val.y, min_val.y) );
	}

	inline vector3 _get_rand(const vector3& max_val, const vector3& min_val)
	{
		return vector3( _get_rand(max_val.x, min_val.x),
			_get_rand(max_val.y, min_val.y),
			_get_rand(max_val.z, min_val.z) );
	}

	inline color4f _get_rand(const color4f& max_val, const color4f& min_val)
	{
		return color4f( _get_rand(max_val.R, min_val.R),
			_get_rand(max_val.G, min_val.G),
			_get_rand(max_val.B, min_val.B),
			_get_rand(max_val.A, min_val.A)
			);
	}

	template<typename T>
	class ndistribution_random : public ndistribution<T>
	{
	public:
		T	m_max;
		T	m_min;
		
		ndistribution_random(const T& max_val, const T& min_val):m_max(max_val),m_min(min_val)
		{}

		ndistribution_random(void)
		{
			m_max = m_min = T();
		}

		virtual nstring get_type()
		{
			return _T("ndistribution_const");
		}

		virtual T get_value(float time)
		{			
			(void)time;
			return _get_rand(m_max, m_min);
		}

		virtual void serialize(narchive& ar)
		{
			nSERIALIZE(ar, m_max);
			nSERIALIZE(ar, m_min);
		}

		virtual void sort()
		{

		}
	};

	enum EDistributionCurveType
	{
		EDC_Linear,
	};

	inline float _get_lerp(float t0, float t1, float r)
	{
		return t0*(1-r) + t1*r;
	}

	inline vector2 _get_lerp(const vector2& t0, const vector2& t1, float r)
	{
		return vector2( _get_lerp(t0.x, t1.x, r),
			_get_lerp(t0.y, t1.y, r)
			);
	}

	inline vector3 _get_lerp(const vector3& t0, const vector3& t1, float r)
	{
		return vector3( _get_lerp(t0.x, t1.x, r),
			_get_lerp(t0.y, t1.y, r),
			_get_lerp(t0.z, t1.z, r)
			);
	}

	inline color4f _get_lerp(const color4f& t0, const color4f& t1, float r)
	{
		return color4f( _get_lerp(t0.R, t1.R, r),
			_get_lerp(t0.G, t1.G, r),
			_get_lerp(t0.B, t1.B, r),
			_get_lerp(t0.A, t1.A, r)
			);
	}

	template<typename T>
	class ndistribution_curve : public ndistribution<T>
	{
	public:
		typedef std::pair<T, float> key_type;
		typedef std::vector<key_type> st_key_array;

		bool					m_loop;
		EDistributionCurveType	m_type;

		st_key_array			m_keys;
		float					m_time_len;
	public:

		ndistribution_curve(void)
		{
			m_type = EDC_Linear;
			m_loop = true;
			m_time_len = 0;
		}

		~ndistribution_curve(void)
		{}

		virtual nstring get_type()
		{
			return _T("ndistribution_curve");
		}

		bool is_loop()
		{
			return m_loop;
		}

		void set_loop(bool loop)
		{
			m_loop=loop;
		}

		void clear()
		{
			m_keys.clear();
		}

		void set_key_value(size_t index,T value,float time)
		{
			if(index>=m_keys.size()) return;
			m_keys[index]=make_pair(value,time);
		}

		void add_key(T val, float time)
		{
			m_keys.push_back( make_pair(val, time) );
			if(m_time_len < time)
				m_time_len = time;
			//按时间排序
			sort();
		}

		void remove_key(size_t key_index)
		{
			if(key_index >= m_keys.size())
				return;
			st_key_array::iterator i=m_keys.begin();
			advance(i,key_index);
			m_keys.erase( i );
		}

		size_t get_num_keys() const	{	return m_keys.size();}
		key_type get_key(size_t i) const {	return m_keys[i]; }

		virtual T get_value(float time)
		{			
			if( m_keys.empty() )
				return T();

			size_t num_keys = m_keys.size();
			if(num_keys == 1 // 只有一个key
				|| time <= m_keys[0].second)
				return m_keys[0].first;

			if(time>m_keys[m_keys.size()-1].second)
			{
				if(m_loop)
				{
					//-- 修正time值，去掉循环次数
					int loop_count = (int)(time/m_time_len);
					time -= m_time_len*loop_count;
				}
				else
				{
					return m_keys[m_keys.size()-1].first;
				}
			}			

			//-- find key			
			size_t i = 0;
			for (i=0; i<num_keys; i++)
			{
				if(time < m_keys[i].second)
					break;
			}			

			if( i >= num_keys )
			{
				if(m_loop)
					i = 1;	// 循环模式，重新开始
				else
					return m_keys[num_keys-1].first; // 非循环模式，返回最后一个key的值
			}// end of if

			if(i>0)
			{
				const key_type& last_key = m_keys[i-1];
				const key_type& cur_key = m_keys[i];

				float time_len = cur_key.second - last_key.second;
				float time_pass = time-last_key.second;
				return _get_lerp(last_key.first, cur_key.first, time_pass/time_len);
			}
			else
			{
				return m_keys[i].first;
			}			
		}

		virtual void serialize(narchive& ar)
		{			
			nSERIALIZE(ar, m_loop);
			nserialize_enum_as_int(ar, m_type, _T("m_type"));

			//--
			const TCHAR* array_name = _T("m_keys");
			size_t array_size = 0;
			if( ar.is_loading() )
			{
				m_keys.clear();
			}
			else
			{
				array_size = m_keys.size();
			}

			ar.array_begin(array_name, array_size);
			if( array_size > 0 )
			{
				if( ar.is_loading() )
					m_keys.resize(array_size);

				for(size_t i=0; i<array_size; i++)
				{
					key_type& obj = m_keys[i];
					serialize_key(ar, obj, ELEM_ArrayItem);			
					ar.array_next();
				}
			}
			ar.array_end();
		}

		virtual void sort()
		{
			std::sort(m_keys.begin(),m_keys.end(),compare_key_value);
		}

	private:
		void serialize_key(narchive& ar, key_type& obj, const TCHAR* obj_name)
		{
			nstring class_name(_T("key_type"));
			ar.object_begin(obj_name, class_name);
				nserialize(ar, obj.first, _T("val"));
				nserialize(ar, obj.second, _T("time"));
			ar.object_end();
		}

		static int compare_key_value(const key_type& a,const key_type& b)
		{
			return a.second<b.second;
		}
	};

	//-- 序列化相关操作 -------------------------------------------------------------------------------------
	#define nSERIALIZE_DISTRIBUTION(ar, val) nserialize_distribution(ar, val, _T(#val))

	template<typename T>
	inline narchive& nserialize_distribution(narchive& ar, 
		boost::shared_ptr<ndistribution<T> >& obj_ptr, 
		const TCHAR* obj_name)
	{
		const TCHAR* const NONE_CLASS = _T("NoneClass");

		if(ar.is_loading())
		{
			nstring class_name;
			ar.object_begin(obj_name, class_name);

			if(class_name == _T("ndistribution_const"))			
				obj_ptr.reset( new ndistribution_const<T> );
			else if(class_name == _T("ndistribution_uniform"))			
				obj_ptr.reset( new ndistribution_random<T> );
			else if(class_name == _T("ndistribution_curve"))			
				obj_ptr.reset( new ndistribution_curve<T> );

			if( obj_ptr )
				obj_ptr->serialize(ar);
			ar.object_end();
		}
		else
		{
			nstring class_name;
			if( obj_ptr )
				class_name = obj_ptr->get_type();
			else
				class_name = NONE_CLASS;

			ar.object_begin(obj_name, class_name);
			if( obj_ptr )
				obj_ptr->serialize(ar);
			ar.object_end();
		}
		return ar;
	}

	//===============================================================================
	template<typename T>
	class ndistribution_uniform : public ndistribution<T>
	{
	public:
		typedef std::pair<T, float> key_type;
		typedef std::vector<key_type> st_key_array;
		//-----------------------------------------------------------------------------------
		virtual nstring get_type()
		{
			return _T("ndistribution_uniform");
		}

		bool is_loop()
		{
			return m_loop;
		}

		void set_loop(bool loop)
		{
			m_loop=loop;
		}

		void clear()
		{
			m_keys.clear();
		}

		void set_key_value(size_t index,T value,float time)
		{
			if(index>=m_keys.size()) return;
			m_keys[index]=make_pair(value,time);
		}

		void add_key(T val, float time)
		{
			m_keys.push_back( make_pair(val, time) );
			if(m_time_len < time)
				m_time_len = time;
			//按时间排序
			sort();
		}

		void remove_key(size_t key_index)
		{
			if(key_index >= m_keys.size())
				return;
			st_key_array::iterator i=m_keys.begin();
			advance(i,key_index);
			m_keys.erase( i );
		}

		size_t get_num_keys() const	{	return m_keys.size();}
		key_type get_key(size_t i) const {	return m_keys[i]; }

		virtual T get_value(float time)
		{
			if(m_is_random)
			{
				return _get_rand(m_max,m_min);
			}

			if( m_keys.empty() )
				return T();

			size_t num_keys = m_keys.size();
			//只有一个节点或者时间值小于第一个节点的时间值时，直接使用第一个节点的值
			if(num_keys == 1 || time <= m_keys[0].second)
				return m_keys[0].first;

			//如果时间值大于总时间
			if(time>m_time_len)
			{
				if(m_loop)
				{
					//如果开启循环，则修正time值，去掉循环次数
					int loop_count = (int)(time/m_time_len);
					time -= m_time_len*loop_count;
				}
				else
				{
					//如果没有开启循环，则保留最后的状态
					return m_keys[m_keys.size()-1].first;
				}
			}			

			// 寻找对应的节点			
			size_t i = 0;
			for (i=0; i<num_keys; i++)
			{
				if(time < m_keys[i].second)
					break;
			}

			if(i>0)
			{
				const key_type& last_key = m_keys[i-1];
				const key_type& cur_key = m_keys[i];

				float time_len = cur_key.second - last_key.second;
				float time_pass = time-last_key.second;
				return _get_lerp(last_key.first, cur_key.first, time_pass/time_len);
			}
			else
			{
				return m_keys[i].first;
			}
		}

		virtual bool	is_random()
		{
			return m_is_random;
		}

		virtual void	set_random(bool is_rand)
		{
			m_is_random=is_rand;
		}

		virtual void	set_random_max(const T& max)
		{
			m_max=max;
		}

		virtual void	set_random_min(const T& min)
		{
			m_min=min;
		}

		virtual T&		get_random_max()
		{
			return m_max;
		}

		virtual T&		get_random_min()
		{
			return m_min;
		}

		virtual void sort()
		{
			std::sort(m_keys.begin(),m_keys.end(),compare_key_value);
		}

		virtual void serialize(narchive& ar)
		{
			nSERIALIZE(ar, m_loop);
			nSERIALIZE(ar, m_is_random);
			nSERIALIZE(ar, m_max);
			nSERIALIZE(ar, m_min);
			nSERIALIZE(ar, m_time_len);

			//--
			const TCHAR* array_name = _T("m_keys");
			size_t array_size = 0;
			if( ar.is_loading() )
			{
				m_keys.clear();
			}
			else
			{
				array_size = m_keys.size();
			}

			ar.array_begin(array_name, array_size);
			if( array_size > 0 )
			{
				if( ar.is_loading() )
					m_keys.resize(array_size);

				for(size_t i=0; i<array_size; i++)
				{
					key_type& obj = m_keys[i];
					serialize_key(ar, obj, ELEM_ArrayItem);			
					ar.array_next();
				}
			}
			ar.array_end();
		}
	private:
		//是否是随机值
		bool	m_is_random;
		//随机值的最大值和最小值
		T		m_max;
		T		m_min;
		//不是随机值时使用的插值节点表
		st_key_array			m_keys;
		//是否循环遍历
		bool					m_loop;
		//一次遍历的总时间
		float					m_time_len;
	private:
		void serialize_key(narchive& ar, key_type& obj, const TCHAR* obj_name)
		{
			nstring class_name(_T("key_type"));
			ar.object_begin(obj_name, class_name);
			nserialize(ar, obj.first, _T("val"));
			nserialize(ar, obj.second, _T("time"));
			ar.object_end();
		}

		static int compare_key_value(const key_type& a,const key_type& b)
		{
			return a.second<b.second;
		}
	public:
		ndistribution_uniform()
		{
			m_loop = true;
			m_time_len = 0;
		}
		virtual ~ndistribution_uniform()
		{

		}
	};

}//namespace nexus

#endif //_NEXUS_DISTRIBUTION_H_