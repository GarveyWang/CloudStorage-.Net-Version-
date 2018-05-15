// CSFriend.h

#pragma once

using namespace System;
using namespace System::Data;
using namespace System::Data::Odbc;

using namespace System::Collections::Generic;

namespace CSFriend {

	public ref class UserFriend
	{
	public:
		UserFriend();
		UserFriend(Int32 fromId,Int32 toId, Byte^ permitted);

		Int32 getFromId();
		Int32 getToId();
		Byte^ getPermitted();

		void setFromId(Int32 fromId);
		void setToId(Int32 toId);
		void setPermitted(Byte^ permitted);

	private:
		Int32 fromId;
		Int32 toId;
		Byte^ permitted;
	};

	public ref class UserFileShare
	{
	public:
		UserFileShare();
		UserFileShare(Int32 fromId, Int32 toId, Int32 fId, String^ remark);

		Int32 getFromId();
		Int32 getToId();
		Int32 getFId();
		String^ getRemark();

		void setFromId(Int32 fromId);
		void setToId(Int32 toId);
		void setFId(Int32 fId);
		void setRemark(String^ remark);

	private:
		Int32 fromId;
		Int32 toId;
		Int32 fId;
		String^ remark;
	};

	public ref class UserFriendDao
	{
	public:
		UserFriendDao(OdbcConnection^ Conn);
		UserFriend^ getUserFriend(Int32 fromId, Int32 toId);
		List<UserFriend^>^ getUserFriends(Int32 uId);
		List<UserFriend^>^ getUnpermittedFriends(Int32 fromId);
		List<UserFriend^>^ getFriendRequests(Int32 toId);

		Int32 insert(UserFriend^ userFriend);
		Int32 del(Int32 fromId, Int32 toId);
		Int32 update(UserFriend^ userFriend);

	private:
		OdbcConnection^ Conn;
	};

	public ref class UserFileShareDao
	{
	public:
		UserFileShareDao(OdbcConnection^ Conn);
		UserFileShare^ getSharedFile(Int32 fromId, Int32 toId, Int32 fId);
		List<UserFileShare^>^ getSharedFiles(Int32 fromId, Int32 toId);

		Int32 insert(UserFileShare^ userFileShare);
		Int32 del(Int32 fId);
		Int32 del(Int32 fromId, Int32 toId);
		Int32 del(Int32 fromId, Int32 toId, Int32 fId);
		Int32 update(UserFileShare^ userFileShare);

	private:
		OdbcConnection^ Conn;
	};
}
