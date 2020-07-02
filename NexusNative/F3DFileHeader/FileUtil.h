
#pragma once
#include <string>
using namespace std;

namespace F3D
{
	inline void FWriteString(FILE *fp,const char *str)
	{
		unsigned int strLen=strlen(str)+1;
		fwrite(&strLen,sizeof(unsigned int),1,fp);
		fwrite(str,strLen,1,fp);
	}
	
#ifdef VFS_READ
	inline void FReadString(IFS *pFS,unsigned int hFile,string& str)
	{
		unsigned int strLen;
		pFS->Read(&strLen,sizeof(unsigned int),hFile);
		if(strLen > 0)
		{
			char *pTemp=new char[strLen];
			pFS->Read(pTemp,strLen,hFile);
			str=pTemp;
			delete[] pTemp;
		}
		else
			str="";
	}
#else
	inline void FReadString(FILE *fp,string& str)
	{
		unsigned int strLen;
		fread(&strLen,sizeof(unsigned int),1,fp);
		if(strLen > 0)
		{
			char *pTemp=new char[strLen];
			fread(pTemp,strLen,1,fp);
			str=pTemp;
			delete pTemp;
		}
		else
			str="";
	}
#endif//#ifdef VFS_READ
	
	//--对于简单的int等值类型的读写
	template<typename ValType>
		void FWriteValue(FILE *fp,ValType& val)
	{
		fwrite(&val,sizeof(ValType),1,fp);
	}
#ifdef VFS_READ
	template<typename ValType>
		void FReadValue(IFS *pFS,DWORD hFile,ValType& val)
	{
		pFS->Read(&val,sizeof(ValType),hFile);
	}
#else
	template<typename ValType>
		void FReadValue(FILE *fp,ValType& val)
	{
		fread(&val,sizeof(ValType),1,fp);
	}
#endif//#ifdef VFS_READ
	
	//--对一个简单类型(int,float等)的vector进行文件读写
	template<typename ValType>
		void FWriteValVector(FILE *fp,vector<ValType>& valVector)
	{
		unsigned int size=(unsigned int)valVector.size();
		fwrite(&size,sizeof(size),1,fp);
		if(size==0)
			return;
		for(vector<ValType>::iterator iter=valVector.begin();
			iter!=valVector.end();iter++)
		{
			fwrite(&(*iter),sizeof(ValType),1,fp);
		}
	}
#ifdef VFS_READ
	template<typename ValType>
		void FReadValVector(IFS *pFS,DWORD hFile,vector<ValType>& valVector)
	{
		valVector.clear();
		//--
		unsigned int size=0;
		pFS->Read(&size,sizeof(size),hFile);
		if(size==0)
			return;
		valVector.resize(size);
		for(vector<ValType>::iterator iter=valVector.begin();
			iter!=valVector.end();iter++)
		{
			pFS->Read(&(*iter),sizeof(ValType),hFile);
		}
	}
#else
	template<typename ValType>
		void FReadValVector(FILE *fp,vector<ValType>& valVector)
	{
		valVector.clear();
		//--
		unsigned int size=0;
		fread(&size,sizeof(size),1,fp);
		if(size==0)
			return;
		valVector.resize(size);
		for(vector<ValType>::iterator iter=valVector.begin();
			iter!=valVector.end();iter++)
		{
			fread(&(*iter),sizeof(ValType),1,fp);
		}
	}
#endif//#ifdef VFS_READ

}//namespace F3D