#include "StdAfx.h"
#include "NFileQuery.h"

namespace NexusEngine
{
	NFileEntity^ NFileEntity::FromNative(const nfile_entity& file)
	{
		NFileEntity^ ret = gcnew NFileEntity;
		ret->type = static_cast<NexusEngine::EFileEntityType>(file.type);
		ret->pkg = gcnew System::String(file.pkg.c_str());
		ret->path = gcnew System::String(file.path.c_str());
		ret->fileSize = file.file_size;

		return ret;
	}

	String^ NFileEntity::FileName::get()
	{
		String^ ret = gcnew String(path);

		int p = ret->LastIndexOf(_T('/'));
		if( p != -1)
			ret = ret->Substring(p+1);

		ret->ToLower();
		return ret;
	}

	String^ NFileEntity::FileNameNoExtension::get()
	{
		String^ ret = this->FileName;
		
		int p = ret->LastIndexOf(_T('.'));
		if( p != -1)
			ret = ret->Substring(0, p);

		return ret;
	}

	String^ NFileEntity::FileExtension::get()
	{
		String^ ret = gcnew String(path);

		int p = ret->LastIndexOf(_T('.'));
		if( p != -1)
			ret = ret->Substring(p+1);

		ret->ToLower();
		return ret;
	}

	NFileQuery::NFileQuery(void)
	{
	}

	NativeFileQueryWrapper::NativeFileQueryWrapper(NFileQuery^ clrObj):m_objCLR(clrObj)
	{}

	void NativeFileQueryWrapper::on_file_entity(const nfile_entity& file)
	{
		NFileEntity^ clrFile = NFileEntity::FromNative(file);
		m_objCLR->OnFileEntity( *clrFile );
	}
}//namespace NexusEngine