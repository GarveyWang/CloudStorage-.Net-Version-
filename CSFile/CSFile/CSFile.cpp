// 这是主 DLL 文件。

#include "stdafx.h"

#include "CSFile.h"

using namespace CSFile;

CSFile::Dir::Dir()
{
}

CSFile::Dir::Dir(Int32 dId, String ^ name, DateTime ^ createTime, Int32 parent)
{
	this->dId = dId;
	this->name = name;
	this->createTime = createTime;
	this->parent=parent;
}

Int32 CSFile::Dir::getDId()
{
	return dId;
}

String ^ CSFile::Dir::getName()
{
	return name;
}

DateTime ^ CSFile::Dir::getCreateTime()
{
	return createTime;
}

Int32 CSFile::Dir::getParent()
{
	return parent;
}

List<Dir^>^ CSFile::Dir::getChildrenDirList()
{
	return childrenDirList;
}

List<FileHeader^>^ CSFile::Dir::getChildrenFileHeaderList()
{
	return childrenFileHeaderList;
}



void CSFile::Dir::setDId(Int32 dId)
{
	this->dId = dId;
}

void CSFile::Dir::setName(String ^ name)
{
	this->name = name;
}

void CSFile::Dir::setCreateTime(DateTime ^ createTime)
{
	this->createTime = createTime;
}

void CSFile::Dir::setParent(Int32 parent)
{
	this->parent = parent;
}

void CSFile::Dir::setChildrenDirList(List<Dir^>^ childrenDirList)
{
	this->childrenDirList=childrenDirList;
}

void CSFile::Dir::setChildrenFileHeaderList(List<FileHeader^>^ childrenFileHeaderList)
{
	this->childrenFileHeaderList = childrenFileHeaderList;
}

CSFile::FileHeader::FileHeader()
{
}

CSFile::FileHeader::FileHeader(Int32 fId, String ^ name, String ^ path, Int32 size, DateTime ^ submitTime, Int32 parent, Int32 uId)
{
	this->fId=fId;
	this->name = name;
	this->path = path;
	this->size = size;
	this->submitTime = submitTime;
	this->parent = parent;
	this->uId = uId;
}

Int32 CSFile::FileHeader::getFId()
{
	return fId;
}

String ^ CSFile::FileHeader::getName()
{
	return name;
}

String ^ CSFile::FileHeader::getPath()
{
	return path;
}

Int32 CSFile::FileHeader::getSize()
{
	return size;
}

DateTime ^ CSFile::FileHeader::getSubmitTime()
{
	return submitTime;
}

Int32 CSFile::FileHeader::getParent()
{
	return parent;
}

Int32 CSFile::FileHeader::getUId()
{
	return uId;
}

void CSFile::FileHeader::setFId(Int32 fId)
{
	this->fId = fId;
}

void CSFile::FileHeader::setName(String ^ name)
{
	this->name = name;
}

void CSFile::FileHeader::setPath(String ^ path)
{
	this->path = path;
}

void CSFile::FileHeader::setSize(Int32 size)
{
	this->size = size;
}

void CSFile::FileHeader::setSubmitTime(DateTime ^ submitTime)
{
	this->submitTime = submitTime;
}

void CSFile::FileHeader::setParent(Int32 parent)
{
	this->parent = parent;
}

void CSFile::FileHeader::setUId(Int32 uId)
{
	this->uId = uId;
}

CSFile::DirDao::DirDao(OdbcConnection ^ Conn)
{
	this->Conn = Conn;
}

Dir ^ CSFile::DirDao::getDir(Int32 dId)
{
	DataSet^ ds;
	OdbcDataAdapter^ Adapter;
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from dir where d_id = ?", Conn);
	cmd->Parameters->Add("@dId", dId);
	Adapter = gcnew OdbcDataAdapter(cmd);
	ds = gcnew DataSet();
	Adapter->Fill(ds, "dir");
	if (ds->Tables->Count == 0) {
		return nullptr;
	}
	DataRow^ row = ds->Tables["dir"]->Rows[0];

	String^ format = (row["create_time"]->ToString()->Length == 19)?"yyyy/MM/dd HH:mm:ss":"yyyy/M/dd HH:mm:ss";

	Dir^ dir = gcnew Dir((Int32)row["d_id"], (String^)row["name"], DateTime::ParseExact(row["create_time"]->ToString(),format,System::Globalization::CultureInfo::CurrentCulture), (Int32)row["parent"]);
	return dir;
}

Int32 CSFile::DirDao::getLastId()
{
	DataSet^ ds;
	OdbcDataAdapter^ Adapter;
	OdbcCommand^ cmd = gcnew OdbcCommand("select last_insert_id() as dId;", Conn);
	Adapter = gcnew OdbcDataAdapter(cmd);
	ds = gcnew DataSet();
	Adapter->Fill(ds, "lastID");
	if (ds->Tables->Count == 0) {
		return -1;
	}
	DataRow^ row = ds->Tables["lastID"]->Rows[0];
	return Int32::Parse(row["dId"]->ToString());
}

Int32 CSFile::DirDao::insert(Dir ^ dir)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("INSERT ignore dir (`name`, `create_time`, `parent`) VALUES (?, ?, ?)", Conn);
	cmd->Parameters->Add("@name", dir->getName());
	cmd->Parameters->Add("@createTime", dir->getCreateTime());
	cmd->Parameters->Add("@parent", dir->getParent());
	return cmd->ExecuteNonQuery();
}

Int32 CSFile::DirDao::del(Int32 dId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM dir WHERE `d_id`= ?", Conn);
	cmd->Parameters->Add("@dId", dId);
	return cmd->ExecuteNonQuery();
}

Int32 CSFile::DirDao::update(Dir ^ dir)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("UPDATE dir SET `name`= ?, `create_time`= ?, `parent`= ? WHERE `d_id`= ?", Conn);
	cmd->Parameters->Add("@name", dir->getName());
	cmd->Parameters->Add("@createTime", dir->getCreateTime());
	cmd->Parameters->Add("@parent", dir->getParent());
	cmd->Parameters->Add("@dId", dir->getDId());
	return cmd->ExecuteNonQuery();
}

List<Dir^>^ CSFile::DirDao::getChildDirs(Int32 parent)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from dir where parent = ?", Conn);
	cmd->Parameters->Add("@parent", parent);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "dirs");

	List<Dir^>^ childDirs = gcnew List<Dir^>();

	for each (DataRow^ row in ds->Tables["dirs"]->Rows)
	{
		String^ format = (row["create_time"]->ToString()->Length == 19) ? "yyyy/MM/dd HH:mm:ss" : "yyyy/M/dd HH:mm:ss";
		Dir^ dir = gcnew Dir((Int32)row["d_id"], (String^)row["name"], DateTime::ParseExact(row["create_time"]->ToString(), format, System::Globalization::CultureInfo::CurrentCulture), (Int32)row["parent"]);
		childDirs->Add(dir);
	}

	return childDirs;
}

CSFile::FileHeaderDao::FileHeaderDao(OdbcConnection ^ Conn)
{
	this->Conn = Conn;
}

FileHeader ^ CSFile::FileHeaderDao::getFileHeader(Int32 fId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from file_header where f_id = ?", Conn);
	cmd->Parameters->Add("@fId", fId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "fileHeaders");
	if (ds->Tables->Count == 0) {
		return nullptr;
	}
	DataRow^ row = ds->Tables["fileHeaders"]->Rows[0];

	String^ format = (row["submit_time"]->ToString()->Length == 19) ? "yyyy/MM/dd HH:mm:ss" : "yyyy/M/dd HH:mm:ss";

	FileHeader^ fileHeader = gcnew FileHeader((Int32)row["f_id"], (String^)row["name"], (String^)row["path"], (Int32)row["size"], DateTime::ParseExact(row["submit_time"]->ToString(), format, System::Globalization::CultureInfo::CurrentCulture), (Int32)row["parent"], (Int32)row["u_id"]);
	return fileHeader;
}

Int32 CSFile::FileHeaderDao::insert(FileHeader ^ fileHeader)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("INSERT ignore file_header (`name`, `path`, `size` , `submit_time`, `parent`, `u_id`) VALUES (?, ?, ?, ?, ?, ?)", Conn);
	cmd->Parameters->Add("@name", fileHeader->getName());
	cmd->Parameters->Add("@path", fileHeader->getPath());
	cmd->Parameters->Add("@size", fileHeader->getSize());
	cmd->Parameters->Add("@submitTime", fileHeader->getSubmitTime());
	cmd->Parameters->Add("@parent", fileHeader->getParent());
	cmd->Parameters->Add("@uId", fileHeader->getUId());
	return cmd->ExecuteNonQuery();
}

Int32 CSFile::FileHeaderDao::del(Int32 fId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM file_header WHERE `f_id`= ?", Conn);
	cmd->Parameters->Add("@fId", fId);
	return cmd->ExecuteNonQuery();
}

Int32 CSFile::FileHeaderDao::update(FileHeader ^ fileHeader)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("UPDATE file_header SET `name`= ?, `path`= ?, `size`= ?, `submit_time`= ?, `parent`= ?, `u_id`= ? WHERE `f_id`= ?", Conn);
	cmd->Parameters->Add("@name", fileHeader->getName());
	cmd->Parameters->Add("@path", fileHeader->getPath());
	cmd->Parameters->Add("@size", fileHeader->getSize());
	cmd->Parameters->Add("@submitTime", fileHeader->getSubmitTime());
	cmd->Parameters->Add("@parent", fileHeader->getParent());
	cmd->Parameters->Add("@uId", fileHeader->getUId());
	cmd->Parameters->Add("@fId", fileHeader->getFId());
	return cmd->ExecuteNonQuery();
}

List<FileHeader^>^ CSFile::FileHeaderDao::getChildFileHeaders(Int32 parent)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from file_header where parent = ?", Conn);
	cmd->Parameters->Add("@parent", parent);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "fileHeader");

	List<FileHeader^>^ childFileHeaders = gcnew List<FileHeader^>();

	for each (DataRow^ row in ds->Tables["fileHeader"]->Rows)
	{
		String^ format = (row["submit_time"]->ToString()->Length == 19) ? "yyyy/MM/dd HH:mm:ss" : "yyyy/M/dd HH:mm:ss";
		FileHeader^ fileHeader = gcnew FileHeader((Int32)row["f_id"], (String^)row["name"], (String^)row["path"], (Int32)row["size"], DateTime::ParseExact(row["submit_time"]->ToString(), format, System::Globalization::CultureInfo::CurrentCulture), (Int32)row["parent"], (Int32)row["u_id"]);
		childFileHeaders->Add(fileHeader);
	}

	return childFileHeaders;
}
