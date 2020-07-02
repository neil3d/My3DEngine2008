/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once

namespace NexusEngine
{
	/**
	 * Native对象包装接口
	 * @remarks 对于nobject派生类的包装，使用基类管理nobject指针，派生类使用时再cast为具体类
	*/
	public ref class INativeObj abstract
	{
	public:
		nexus::nobject* GetRawPtr()
		{
			return RawPtr;
		}
	protected:
		property nexus::nobject* RawPtr
		{
			virtual nexus::nobject* get() = 0;
		}

		static nexus::nobject* ConstructObject(System::String^ nativeClassName);
	};

	/**
	 *	提供一个对Native Object的“Borrowned Reference”对象的包装
	 *	@remarks 不管理内部对象指针的生存期，其生存期通常有其他的Native Object持有
	*/
	public ref class NativeObjectBorrowed : public INativeObj
	{
	public:
		explicit NativeObjectBorrowed(nexus::nobject* native_obj);

	protected:
		property nexus::nobject* RawPtr
		{
			virtual nexus::nobject* get() override;
		}

		property nexus::nobject* NativePtr
		{
			nexus::nobject* get();
		}
	
	protected:
		nexus::nobject*	m_obj;
	};

	/**
	 *	提供一个对Native Object的“Owner Reference”对象的包装
	 *	@remarsk 通过shared_ptr和Native代码协调对象生存期
	*/
	public ref class NativeObjectOwnedBySharedPtr : public INativeObj
	{
	public:
		explicit NativeObjectOwnedBySharedPtr(const boost::shared_ptr<nexus::nobject>& native_obj);		
		NativeObjectOwnedBySharedPtr(void):m_smart_ptr(NULL)
		{}
		~NativeObjectOwnedBySharedPtr();		

	protected:
		property nexus::nobject* RawPtr
		{
			virtual nexus::nobject* get() override;
		}

		property nexus::nobject* NativePtr
		{
			nexus::nobject* get();
		}

	protected:
		boost::shared_ptr<nexus::nobject>*	m_smart_ptr;
	};

	/**
	 *	通过原始指针方式，独立拥有Native Object的对象包装
	*/
	public ref class NativeObjectOwnedRaw : public INativeObj
	{
	public:
		explicit NativeObjectOwnedRaw(nexus::nobject* obj);
		~NativeObjectOwnedRaw();

	protected:
		property nexus::nobject* RawPtr
		{
			virtual nexus::nobject* get() override;
		}
		property nexus::nobject* NativePtr
		{
			nexus::nobject* get();
		}

	protected:
		nexus::nobject*	m_obj;
	};
}//namespace NexusCore