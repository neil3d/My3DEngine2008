#pragma once

namespace NexusEngine
{

#define RENDER_RESOURCE_CLASS(managedType, nativeType)			\
	public:														\
		explicit managedType( nativeType* nativeRes )			\
		{														\
			m_nativeRenderResPtr								\
				= new render_res_ptr<nativeType>( nativeRes );	\
		}														\
																\
		virtual ~managedType(void)								\
		{														\
			delete m_nativeRenderResPtr;						\
			m_nativeRenderResPtr = NULL;						\
		}														\
																\
		property nativeType* RawPtr								\
		{														\
			nativeType* get()									\
			{													\
				if( m_nativeRenderResPtr )						\
				{												\
					return m_nativeRenderResPtr->get();			\
				}												\
				else											\
				{												\
					return NULL;								\
				}												\
			}													\
		}														\
																\
	private:													\
		render_res_ptr<nativeType>* m_nativeRenderResPtr;		\
																
} // end of namespace NexusEngine								
