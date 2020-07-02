#pragma once
#include "NMath.h"
#include "NDistribution.h"

namespace NexusEngine
{
	/**
	 *	Vector2 Distribution基类
	*/
	public ref class NDistributionVector2 : public NDistribution
	{
	public:
		NDistributionVector2(nexus::ndistribution<vector2>::ptr nativeObj);		
		NDistributionVector2(void)
		{
			m_native = NULL;
		}
		~NDistributionVector2(void);		

		Vector2 GetValue(float time);

		nexus::ndistribution<vector2>::ptr GetNativeSmartPtr()
		{
			return *m_native;
		}
	protected:
		nexus::ndistribution<vector2>::ptr* m_native;

		property nexus::ndistribution<vector2>* NativePtr
		{
			nexus::ndistribution<vector2>* get()
			{
				return m_native->get();
			}
		}
	};

	/*
	 *	Vector2常量
	*/
	public ref class NDistributionVector2Const : public NDistributionVector2
	{
	public:
		[Category("Distribution")]
		property Vector2 Value
		{
			Vector2 get()
			{
				return Vector2::FromNative(
					((nexus::ndistribution_const<vector2>*)NativePtr)->m_value
					);
			}
			void set(Vector2 val)
			{
				((nexus::ndistribution_const<vector2>*)NativePtr)->m_value 
					= val.ToNative();
			}
		}

	public:
		NDistributionVector2Const(nexus::ndistribution_const<vector2>::ptr nativeObj)
			: NDistributionVector2(nativeObj)
		{}
		NDistributionVector2Const(void);
	};

	/**
	 *	Vector2随机范围
	*/
	public ref class NDistributionVector2Uniform: public NDistributionVector2
	{
	public:
		[Category("Distribution")]
		property Vector2 MaxValue
		{
			Vector2 get()
			{
				return Vector2::FromNative( NativePtr->m_max );
			}
			void set(Vector2 val)
			{
				NativePtr->m_max = val.ToNative();
			}
		}

		[Category("Distribution")]
		property Vector2 MinValue
		{
			Vector2 get()
			{
				return Vector2::FromNative( NativePtr->m_min );
			}
			void set(Vector2 val)
			{
				NativePtr->m_min = val.ToNative();
			}
		}

	public:
		NDistributionVector2Uniform(nexus::ndistribution_random<vector2>::ptr nativeObj)
			: NDistributionVector2(nativeObj)
		{}
		NDistributionVector2Uniform(void);

	private:
		property nexus::ndistribution_random<vector2>* NativePtr
		{
			nexus::ndistribution_random<vector2>* get()
			{
				return (nexus::ndistribution_random<vector2>*)(m_native->get());
			}
		}
	};

	/**
	 *	Vector2随时间进行曲线变化
	*/
	public ref class NDistributionVector2Curve : public NDistributionVector2
	{
	public:
		NDistributionVector2Curve(nexus::ndistribution_curve<vector2>::ptr nativeObj);
		NDistributionVector2Curve(void);

		[Category("Distribution")]
		property int StreamCount
		{
			int get()
			{
				return 2;
			}
		}

		property int	KeyCount
		{
			int get()
			{
				return NativePtr->get_num_keys();;
			}
		}

		float	GetTime(int index)
		{
			return NativePtr->get_key(index).second;
		}

		Vector2	GetValue(float time)
		{
			return Vector2::FromNative(NativePtr->get_value(time));
		}

		Vector2	GetValue(int index)
		{
			return Vector2::FromNative(NativePtr->get_key(index).first);
		}

		void SetValue(int index,float time,Vector2 value)
		{
			return NativePtr->set_key_value(index,value.ToNative(),time);
		}

		void AddKey(Vector2 val, float time)
		{
			NativePtr->add_key(val.ToNative(), time);
		}

		void RemoveKey(int index)
		{
			NativePtr->remove_key((size_t)index);
		}

		void Clear()
		{
			NativePtr->clear();
		}

	private:
		property nexus::ndistribution_curve<vector2>* NativePtr
		{
			nexus::ndistribution_curve<vector2>* get()
			{
				return (nexus::ndistribution_curve<vector2>*)(m_native->get());
			}
		}
	};

	//===================================================================================
	public ref class NDistributionUniformVector2:public NDistribution
	{
	public:
		[Category("Distribution"), Description("数据流的数量"),Browsable(false)]
		property int StreamCount
		{
			int get()
			{
				return 4;
			}
		}

		[Category("Distribution"), Description("曲线插值的节点数量"),Browsable(false)]
		property int	KeyCount
		{
			int get()
			{
				return NativePtr->get_num_keys();;
			}
		}

		[Category("Distribution"), Description("是否是随机模式")]
		property bool	IsRandom
		{
			bool get()
			{
				return NativePtr->is_random();
			}
			void set(bool value)
			{
				NativePtr->set_random(value);
			}
		}

		[Category("Distribution"), Description("随机模式下的最大值")]
		property Vector2 MaxValue
		{
			Vector2 get()
			{
				return Vector2::FromNative( NativePtr->get_random_max() );
			}
			void set(Vector2 val)
			{
				NativePtr->set_random_max(val.ToNative());
			}
		}

		[Category("Distribution"), Description("随机模式下的最小值")]
		property Vector2 MinValue
		{
			Vector2 get()
			{
				return Vector2::FromNative( NativePtr->get_random_min() );
			}
			void set(Vector2 val)
			{
				NativePtr->set_random_min(val.ToNative());
			}
		}

		[Category("Distribution"), Description("是否循环,只在曲线插值模式下有意义")]
		property bool	IsLoop
		{
			bool get()
			{
				return NativePtr->is_loop();
			}
			void set(bool value)
			{
				NativePtr->set_loop(value);
			}
		}

		float	GetTime(int index)
		{
			return NativePtr->get_key(index).second;
		}

		Vector2	GetValueByTime(float time)
		{
			return Vector2::FromNative(NativePtr->get_value(time));
		}

		Vector2	GetValue(int index)
		{
			return Vector2::FromNative(NativePtr->get_key(index).first);
		}

		void SetValue(int index,float time,Vector2 value)
		{
			return NativePtr->set_key_value(index,value.ToNative(),time);
		}

		void AddKey(Vector2 val, float time)
		{
			NativePtr->add_key(val.ToNative(), time);
		}

		void RemoveKey(int index)
		{
			NativePtr->remove_key((size_t)index);
		}

		void Clear()
		{
			NativePtr->clear();
		}

		[Browsable(false)]
		property nexus::ndistribution_uniform<vector2>* NativePtr
		{
			nexus::ndistribution_uniform<vector2>* get()
			{
				return m_NativePtr;
			}
		}
	private:
		ndistribution_uniform<vector2>*	m_NativePtr;
	public:
		NDistributionUniformVector2()
		{
			m_NativePtr=new ndistribution_uniform<vector2>();
		}
		~NDistributionUniformVector2()
		{
			delete m_NativePtr;
		}
	};

}//namespace NexusEngine