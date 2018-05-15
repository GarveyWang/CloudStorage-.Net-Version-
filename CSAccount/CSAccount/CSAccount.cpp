// 这是主 DLL 文件。

#include "stdafx.h"

#include "CSAccount.h"

using namespace CSAccount;

CSAccount::User::User()
{
}

CSAccount::User::User(Int32 uId, String ^ pwd, String ^ name, Int32 rootDir, Int32 recyclebin, String ^ type)
{
	this->uId = uId;
	this->pwd = pwd;
	this->name = name;
	this->rootDir = rootDir;
	this->recyclebin = recyclebin;
	this->type = type;
}

Int32 CSAccount::User::getUId()
{
	return uId;
}

String^ CSAccount::User::getPwd()
{
	return pwd;
}

String ^ CSAccount::User::getName()
{
	return name;
}

Int32 CSAccount::User::getRootDir()
{
	return rootDir;
}

Int32 CSAccount::User::getRecyclebin()
{
	return recyclebin;
}

String ^ CSAccount::User::getType()
{
	return type;
}

void CSAccount::User::setUId(Int32 uId)
{
	this->uId = uId;
}

void CSAccount::User::setPwd(String ^ pwd)
{
	this->pwd = pwd;
}

void CSAccount::User::setName(String ^ name)
{
	this->name = name;
}

void CSAccount::User::setRootDir(Int32 rootDir)
{
	this->rootDir = rootDir;
}

void CSAccount::User::setRecyclebin(Int32 recyclebin)
{
	this->recyclebin = recyclebin;
}

void CSAccount::User::setType(String ^ type)
{
	this->type = type;
}


CSAccount::UserDao::UserDao(OdbcConnection^ Conn)
{
	//Conn = gcnew OdbcConnection("DSN=CloudStorage;UID=root;PWD=0530;");
	this->Conn = Conn;
}

User ^ CSAccount::UserDao::getUser(Int32 uId)
{
	DataSet^ ds;
	OdbcDataAdapter^ Adapter;
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from user where u_id = ?", Conn);
	cmd->Parameters->Add("@uId", uId);
	Adapter = gcnew OdbcDataAdapter(cmd);
	ds = gcnew DataSet();
	Adapter->Fill(ds, "user");
	if (ds->Tables["user"]->Rows->Count == 0) {
		return nullptr;
	}
	DataRow^ row = ds->Tables["user"]->Rows[0];
	User^ user = gcnew User((Int32)row["u_id"], (String^)row["password"], (String^)row["name"], (Int32)row["root_dir"], (Int32)row["recyclebin"], (String^)row["type"]);
	return user;
}

Int32 CSAccount::UserDao::insert(User ^ user)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("INSERT IGNORE user (`u_id`, `password`, `name`, `root_dir`, `recyclebin`, `type`) VALUES (?, ?, ?, ?, ?, ?)", Conn);
	cmd->Parameters->Add("@uId", user->getUId());
	cmd->Parameters->Add("@pwd", user->getPwd());
	cmd->Parameters->Add("@name", user->getName());
	cmd->Parameters->Add("@rootDir", user->getRootDir());
	cmd->Parameters->Add("@recyclebin", user->getRecyclebin());
	cmd->Parameters->Add("@type", user->getType());

	return cmd->ExecuteNonQuery();
}
