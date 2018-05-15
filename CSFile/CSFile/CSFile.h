// CSFile.h

#pragma once

using namespace System;
using namespace System::Data;
using namespace System::Data::Odbc;

using namespace System::Collections::Generic;


namespace CSFile {

	public ref class FileHeader
	{
	public:
		FileHeader();
		FileHeader(Int32 fId,String^ name,String^ path,Int32 size,DateTime^ submitTime,Int32 parent,Int32 uId);

		Int32 getFId();
		String^ getName();
		String^ getPath();
		Int32 getSize();
		DateTime^ getSubmitTime();
		Int32 getParent();
		Int32 getUId();

		void setFId(Int32 fId);
		void setName(String^ name);
		void setPath(String^ path);
		void setSize(Int32 size);
		void setSubmitTime(DateTime^ submitTime);
		void setParent(Int32 parent);
		void setUId(Int32 uId);

	private:
		Int32 fId;
		String^ name;
		String^ path;
		Int32 size;
		DateTime^ submitTime;
		Int32 parent;
		Int32 uId;
	};

	public ref class Dir
	{
	public:
		Dir();
		Dir(Int32 dId, String^ name, DateTime^ createTime, Int32 parent);

		Int32 getDId();
		String^ getName();
		DateTime^ getCreateTime();
		Int32 getParent();
		List<Dir^>^ getChildrenDirList();
		List<FileHeader^>^ getChildrenFileHeaderList();

		void setDId(Int32 dId);
		void setName(String^ name);
		void setCreateTime(DateTime^ createTime);
		void setParent(Int32 parent);
		void setChildrenDirList(List<Dir^>^ childrenDirList);
		void setChildrenFileHeaderList(List<FileHeader^>^ childrenFileHeaderList);

	private:
		Int32 dId;
		String^ name;
		DateTime^ createTime;
		Int32 parent;

		List<Dir^>^ childrenDirList;
		List<FileHeader^>^ childrenFileHeaderList;
	};

	public ref class DirDao
	{
	public:
		DirDao(OdbcConnection^ Conn);
		Dir^ getDir(Int32 dId);
		
		Int32 getLastId();
		Int32 insert(Dir^ dir);
		Int32 del(Int32 dId);
		Int32 update(Dir^ dir);

		List<Dir^>^ getChildDirs(Int32 parent);

	private:
		OdbcConnection^ Conn;
	};

	public ref class FileHeaderDao
	{
	public:
		FileHeaderDao(OdbcConnection^ Conn);
		FileHeader^ getFileHeader(Int32 fId);

		Int32 insert(FileHeader^ fileHeader);
		Int32 del(Int32 fId);
		Int32 update(FileHeader^ fileHeader);

		List<FileHeader^>^ getChildFileHeaders(Int32 parent);

	private:
		OdbcConnection^ Conn;
	};
}
