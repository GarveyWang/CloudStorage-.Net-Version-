// CSAccount.h

#pragma once

using namespace System;
using namespace System::Data;
using namespace System::Data::Odbc;

namespace CSAccount {
	public ref class User 
	{
	public:
		User();
		User(Int32 uId, String^ pwd, String^ name, Int32 rootDir, Int32 recyclebin, String^ type);

		Int32 getUId();
		String^ getPwd();
		String^ getName();
		Int32 getRootDir();
		Int32 getRecyclebin();
		String^ getType();

		void setUId(Int32 uId);
		void setPwd(String^ pwd);
		void setName(String^ name);
		void setRootDir(Int32 rootDir);
		void setRecyclebin(Int32 recyclebin);
		void setType(String^ type);

	private:
		Int32 uId;
		String^ pwd;
		String^ name;
		Int32 rootDir;
		Int32 recyclebin;
		String^ type;
	};

	public ref class UserDao
	{
	public:
		UserDao(OdbcConnection^ Conn);
		User^ getUser(Int32 uId);
		Int32 insert(User^ user);

	private:
		OdbcConnection^ Conn;
	};
}
