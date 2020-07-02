#pragma once
#include "NMath.h"
#include "NDistribution.h"

namespace NexusEngine
{
	public ref class NDistributionFloat : public NDistribution
	{
	public:
		NDistributionFloat(nexus::ndistribution<float>::ptr nativeObj);		
		NDistributionFloat(void)
		{
			m_native = NULL;
		}
		~NDistributionFloat(void);		

		virtual float GetValue(float time);

		nexus::ndistribution<float>::ptr GetNativeSmartPtr()
		{
			return *m_native;
		}
	protected:
		nexus::ndistribution<float>::ptr* m_native;

		property nexus::ndistribution<float>* NativePtr
		{
			nexus::ndistribution<float>* get()
			{
				return m_native->get();
			}
		}
	};

	/*
	*	float常量
	*/
	public ref class NDistributionFloatConst : public NDistributionFloat
	{
	public:
		[Category("Distribution")]
		property float Value
		{
			float get()
			{
				return ((nexus::ndistribution_const<float>*)NativePtr)->m_value;
			}
			void set(float val)
			{
				((nexus::ndistribution_const<float>*)NativePtr)->m_value= val;
			}
		}

	public:
		NDistributionFloatConst(nexus::ndistribution_const<float>::ptr nativeObj)
			: NDistributionFloat(nativeObj)
		{}
		NDistributionFloatConst(void);
	};

	/**
	*	float随机范围
	*/
	public ref class NDistributionFloatUniform: public NDistributionFloat
	{
	public:
		[Category("Distribution")]
		property float MaxValue
		{
			float get()
			{
				return NativePtr->m_max;
			}
			void set(float val)
			{
				NativePtr->m_max = val;
			}
		}

		[Category("Distribution")]
		property float MinValue
		{
			float get()
			{
				return NativePtr->m_min;
			}
			void set(float val)
			{
				NativePtr->m_min = val;
			}
		}

	public:
		NDistributionFloatUniform(nexus::ndistribution_random<float>::ptr nativeObj)
			: NDistributionFloat(nativeObj)
		{}
		NDistributionFloatUniform(void);

	private:
		property nexus::ndistribution_random<float>* NativePtr
		{
			nexus::ndistribution_random<float>* get()
			{
				return (nexus::ndistribution_random<float>*)(m_native->get());
			}
		}
	};

	/**
	*	float随时间进行曲线变化
	*/
	public ref class NDistributionFloatCurve : public NDistributionFloat
	{
	public:
		NDistributionFloatCurve(nexus::ndistribution_curve<float>::ptr nativeObj);
		NDistributionFloatCurve(void);

		[Category("Distribution")]
		property int KeyCount
		{
			int get()
			{
				return NativePtr->get_num_keys();
			}
		}

		float	GetTime(int index)
		{
			return NativePtr->get_key(index).second;
		}

		float GetValue(int index)
		{
			return NativePtr->get_key(index).first;
		}

		void SetValue(int index,float time,float value)
		{
			return NativePtr->set_key_value(index,value,time);
		}

		void AddKey(float val, float time)
		{
			NativePtr->add_key(val, time);
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
		property nexus::ndistribution_curve<float>* NativePtr
		{
			nexus::ndistribution_curve<float>* get()
			{
				return (nexus::ndistribution_curve<float>*)(m_native->get());
			}
		}
	};

	//===================================================================================
	public ref class NDistributionUniformFloat:public NDistribution
	{
	public:
		[Category("Distribution"), Description("数据流的数量"),Browsable(false)]
		property int StreamCount
		{
			int get()
			{
				return 1;
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
		property float MaxValue
		{
			float get()
			{
				return NativePtr->get_random_max();
			}
			void set(float val)
			{
				NativePtr->set_random_max(val);
			}
		}

		[Category("Distribution"), Description("随机模式下的最小值")]
		property float MinValue
		{
			float get()
			{
				return NativePtr->get_random_min();
			}
			void set(float val)
			{
				NativePtr->set_random_min(val);
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

		float	GetValueByTime(float time)
		{
			return NativePtr->get_value(time);
		}

		float	GetValue(int index)
		{
			return NativePtr->get_key(index).first;
		}

		void SetValue(int index,float time,float value)
		{
			return NativePtr->set_key_value(index,value,time);
		}

		void AddKey(float val, float time)
		{
			NativePtr->add_key(val, time);
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
		property nexus::ndistribution_uniform<float>* NativePtr
		{
			nexus::ndistribution_uniform<float>* get()
			{
				return m_NativePtr;
			}
		}
	private:
		ndistribution_uniform<float>*	m_NativePtr;
	public:
		NDistributionUniformFloat()
		{
			m_NativePtr=new ndistribution_uniform<float>();
		}
		~NDistributionUniformFloat()
		{
			delete m_NativePtr;
		}
	};
}