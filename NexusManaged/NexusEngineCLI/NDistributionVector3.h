#pragma once
#include "NMath.h"
#include "NDistribution.h"

namespace NexusEngine
{
	/**
	*	Vector3 Distribution基类
	*/
	public ref class NDistributionVector3 : public NDistribution
	{
	public:
		NDistributionVector3(nexus::ndistribution<vector3>::ptr nativeObj);		
		NDistributionVector3(void)
		{
			m_native = NULL;
		}
		~NDistributionVector3(void);		

		Vector3 GetValue(float time);

		nexus::ndistribution<vector3>::ptr GetNativeSmartPtr()
		{
			return *m_native;
		}
	protected:
		nexus::ndistribution<vector3>::ptr* m_native;

		property nexus::ndistribution<vector3>* NativePtr
		{
			nexus::ndistribution<vector3>* get()
			{
				return m_native->get();
			}
		}
	};

	/*
	*	Vector3常量
	*/
	public ref class NDistributionVector3Const : public NDistributionVector3
	{
	public:
		[Category("Distribution")]
		property Vector3 Value
		{
			Vector3 get()
			{
				return Vector3::FromNative(
					((nexus::ndistribution_const<vector3>*)NativePtr)->m_value
					);
			}
			void set(Vector3 val)
			{
				((nexus::ndistribution_const<vector3>*)NativePtr)->m_value 
					= val.ToNative();
			}
		}

	public:
		NDistributionVector3Const(nexus::ndistribution_const<vector3>::ptr nativeObj)
			: NDistributionVector3(nativeObj)
		{}
		NDistributionVector3Const(void);
	};

	/**
	*	Vector3随机范围
	*/
	public ref class NDistributionVector3Uniform: public NDistributionVector3
	{
	public:
		[Category("Distribution")]
		property Vector3 MaxValue
		{
			Vector3 get()
			{
				return Vector3::FromNative( NativePtr->m_max );
			}
			void set(Vector3 val)
			{
				NativePtr->m_max = val.ToNative();
			}
		}

		[Category("Distribution")]
		property Vector3 MinValue
		{
			Vector3 get()
			{
				return Vector3::FromNative( NativePtr->m_min );
			}
			void set(Vector3 val)
			{
				NativePtr->m_min = val.ToNative();
			}
		}

	public:
		NDistributionVector3Uniform(nexus::ndistribution_random<vector3>::ptr nativeObj)
			: NDistributionVector3(nativeObj)
		{}
		NDistributionVector3Uniform(void);

	private:
		property nexus::ndistribution_random<vector3>* NativePtr
		{
			nexus::ndistribution_random<vector3>* get()
			{
				return (nexus::ndistribution_random<vector3>*)(m_native->get());
			}
		}
	};

	/**
	*	Vector3随时间进行曲线变化
	*/
	public ref class NDistributionVector3Curve : public NDistributionVector3
	{
	public:
		NDistributionVector3Curve(nexus::ndistribution_curve<vector3>::ptr nativeObj);
		NDistributionVector3Curve(void);

		[Category("Distribution")]
		property int StreamCount
		{
			int get()
			{
				return 3;
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

		Vector3	GetValue(float time)
		{
			return Vector3::FromNative(NativePtr->get_value(time));
		}

		Vector3	GetValue(int index)
		{
			return Vector3::FromNative(NativePtr->get_key(index).first);
		}

		void SetValue(int index,float time,Vector3 value)
		{
			return NativePtr->set_key_value(index,value.ToNative(),time);
		}

		void AddKey(Vector3 val, float time)
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
		property nexus::ndistribution_curve<vector3>* NativePtr
		{
			nexus::ndistribution_curve<vector3>* get()
			{
				return (nexus::ndistribution_curve<vector3>*)(m_native->get());
			}
		}
	};

	//===================================================================================
	public ref class NDistributionUniformVector3:public NDistribution
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
		property Vector3 MaxValue
		{
			Vector3 get()
			{
				return Vector3::FromNative( NativePtr->get_random_max() );
			}
			void set(Vector3 val)
			{
				NativePtr->set_random_max(val.ToNative());
			}
		}

		[Category("Distribution"), Description("随机模式下的最小值")]
		property Vector3 MinValue
		{
			Vector3 get()
			{
				return Vector3::FromNative( NativePtr->get_random_min() );
			}
			void set(Vector3 val)
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

		Vector3	GetValueByTime(float time)
		{
			return Vector3::FromNative(NativePtr->get_value(time));
		}

		Vector3	GetValue(int index)
		{
			return Vector3::FromNative(NativePtr->get_key(index).first);
		}

		void SetValue(int index,float time,Vector3 value)
		{
			return NativePtr->set_key_value(index,value.ToNative(),time);
		}

		void AddKey(Vector3 val, float time)
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
		property nexus::ndistribution_uniform<vector3>* NativePtr
		{
			nexus::ndistribution_uniform<vector3>* get()
			{
				return m_NativePtr;
			}
		}
	private:
		ndistribution_uniform<vector3>*	m_NativePtr;
	public:
		NDistributionUniformVector3()
		{
			m_NativePtr=new ndistribution_uniform<vector3>();
		}
		~NDistributionUniformVector3()
		{
			delete m_NativePtr;
		}
	};
}//namespace NexusEngine