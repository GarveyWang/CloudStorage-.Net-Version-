// 这是主 DLL 文件。

#include "stdafx.h"

#include "CSFriend.h"

using namespace CSFriend;

CSFriend::UserFriend::UserFriend()
{
}

CSFriend::UserFriend::UserFriend(Int32 fromId, Int32 toId, Byte ^ permitted)
{
	this->fromId = fromId;
	this->toId = toId;
	this->permitted = permitted;
}

Int32 CSFriend::UserFriend::getFromId()
{
	return fromId;
}

Int32 CSFriend::UserFriend::getToId()
{
	return toId;
}

Byte ^ CSFriend::UserFriend::getPermitted()
{
	return permitted;
}

void CSFriend::UserFriend::setFromId(Int32 fromId)
{
	this->fromId = fromId;
}

void CSFriend::UserFriend::setToId(Int32 toId)
{
	this->toId = toId;
}

void CSFriend::UserFriend::setPermitted(Byte ^ permitted)
{
	this->permitted = permitted;
}

CSFriend::UserFileShare::UserFileShare()
{
}

CSFriend::UserFileShare::UserFileShare(Int32 fromId, Int32 toId, Int32 fId, String^ remark)
{
	this->fromId = fromId;
	this->toId = toId;
	this->fId = fId;
	this->remark = remark;
}

Int32 CSFriend::UserFileShare::getFromId()
{
	return fromId;
}

Int32 CSFriend::UserFileShare::getToId()
{
	return toId;
}

Int32 CSFriend::UserFileShare::getFId()
{
	return fId;
}

String ^ CSFriend::UserFileShare::getRemark()
{
	return remark;
}

void CSFriend::UserFileShare::setFromId(Int32 fromId)
{
	this->fromId = fromId;
}

void CSFriend::UserFileShare::setToId(Int32 toId)
{
	this->toId = toId;
}

void CSFriend::UserFileShare::setFId(Int32 fId)
{
	this->fId = fId;
}

void CSFriend::UserFileShare::setRemark(String ^ remark)
{
	this->remark = remark;
}

CSFriend::UserFriendDao::UserFriendDao(OdbcConnection ^ Conn)
{
	this->Conn = Conn;
}

UserFriend ^ CSFriend::UserFriendDao::getUserFriend(Int32 fromId, Int32 toId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from user_friend where from_id = ? and to_id = ?", Conn);
	cmd->Parameters->Add("@fromId", fromId);
	cmd->Parameters->Add("@toId", toId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "userFriends");
	if (ds->Tables->Count == 0) {
		return nullptr;
	}
	DataRow^ row = ds->Tables["userFriends"]->Rows[0];

	UserFriend^ userFriend = gcnew UserFriend((Int32)row["from_id"], (Int32)row["to_id"], Byte::Parse(row["permitted"]->ToString()));
	return userFriend;
}

List<UserFriend^>^ CSFriend::UserFriendDao::getUserFriends(Int32 uId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from user_friend where permitted = 1 and (from_id = ? or to_id = ?)", Conn);
	cmd->Parameters->Add("@fromId", uId);
	cmd->Parameters->Add("@toId", uId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "userFriends");

	List<UserFriend^>^ userFriends = gcnew List<UserFriend^>();

	for each (DataRow^ row in ds->Tables["userFriends"]->Rows)
	{
		UserFriend^ userFriend = gcnew UserFriend((Int32)row["from_id"], (Int32)row["to_id"], Byte::Parse(row["permitted"]->ToString()));
		userFriends->Add(userFriend);
	}

	return userFriends;
}

List<UserFriend^>^ CSFriend::UserFriendDao::getUnpermittedFriends(Int32 fromId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from user_friend where permitted = 0 and from_id = ?", Conn);
	cmd->Parameters->Add("@fromId", fromId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "userFriends");

	List<UserFriend^>^ userFriends = gcnew List<UserFriend^>();

	for each (DataRow^ row in ds->Tables["userFriends"]->Rows)
	{
		UserFriend^ userFriend = gcnew UserFriend((Int32)row["from_id"], (Int32)row["to_id"], Byte::Parse(row["permitted"]->ToString()));
		userFriends->Add(userFriend);
	}

	return userFriends;
}

List<UserFriend^>^ CSFriend::UserFriendDao::getFriendRequests(Int32 toId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from user_friend where permitted = 0 and to_id = ?", Conn);
	cmd->Parameters->Add("@toId", toId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "userFriends");

	List<UserFriend^>^ userFriends = gcnew List<UserFriend^>();

	for each (DataRow^ row in ds->Tables["userFriends"]->Rows)
	{
		UserFriend^ userFriend = gcnew UserFriend((Int32)row["from_id"], (Int32)row["to_id"], Byte::Parse(row["permitted"]->ToString()));
		userFriends->Add(userFriend);
	}

	return userFriends;
}

Int32 CSFriend::UserFriendDao::insert(UserFriend ^ userFriend)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("INSERT ignore INTO user_friend (`from_id`, `to_id`, `permitted`) VALUES (?, ?, ?)", Conn);
	cmd->Parameters->Add("@fromId", userFriend->getFromId());
	cmd->Parameters->Add("@toId", userFriend->getToId());
	cmd->Parameters->Add("@permitted", userFriend->getPermitted());
	return cmd->ExecuteNonQuery();
}

Int32 CSFriend::UserFriendDao::del(Int32 fromId, Int32 toId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM user_friend WHERE `from_id`= ? and `to_id` = ?", Conn);
	cmd->Parameters->Add("@fromId", fromId);
	cmd->Parameters->Add("@toId", toId);
	return cmd->ExecuteNonQuery();
}

Int32 CSFriend::UserFriendDao::update(UserFriend ^ userFriend)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("UPDATE user_friend SET `permitted`= ? WHERE `from_id`= ? and `to_id`= ? ", Conn);
	cmd->Parameters->Add("@permitted", userFriend->getPermitted());
	cmd->Parameters->Add("@fromId", userFriend->getFromId());
	cmd->Parameters->Add("@toId", userFriend->getToId());
	return cmd->ExecuteNonQuery();
}

CSFriend::UserFileShareDao::UserFileShareDao(OdbcConnection ^ Conn)
{
	this->Conn = Conn;
}

UserFileShare ^ CSFriend::UserFileShareDao::getSharedFile(Int32 fromId, Int32 toId, Int32 fId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from user_file_share where from_id = ? and to_id = ? and fId = ?", Conn);
	cmd->Parameters->Add("@fromId", fromId);
	cmd->Parameters->Add("@toId", toId);
	cmd->Parameters->Add("@fId", fId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "sharedFiles");
	if (ds->Tables->Count == 0) {
		return nullptr;
	}
	DataRow^ row = ds->Tables["sharedFiles"]->Rows[0];

	UserFileShare^ userFileShare = gcnew UserFileShare((Int32)row["from_id"], (Int32)row["to_id"], (Int32)row["fId"], (String^)row["remark"]);
	return userFileShare;
}

List<UserFileShare^>^ CSFriend::UserFileShareDao::getSharedFiles(Int32 fromId, Int32 toId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from user_file_share where from_id = ? and to_id = ?", Conn);
	cmd->Parameters->Add("@fromId", fromId);
	cmd->Parameters->Add("@toId", toId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "sharedFiles");

	List<UserFileShare^>^ sharedFiles = gcnew List<UserFileShare^>();

	for each (DataRow^ row in ds->Tables["sharedFiles"]->Rows)
	{
		UserFileShare^ userFriendShare = gcnew UserFileShare((Int32)row["from_id"], (Int32)row["to_id"], (Int32)row["f_id"], (String^)row["remark"]);
		sharedFiles->Add(userFriendShare);
	}

	return sharedFiles;
}

Int32 CSFriend::UserFileShareDao::insert(UserFileShare ^ userFileShare)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("INSERT ignore INTO user_file_share (`from_id`, `to_id`, `f_id`, `remark`) VALUES (?, ?, ?, ?)", Conn);
	cmd->Parameters->Add("@fromId", userFileShare->getFromId());
	cmd->Parameters->Add("@toId", userFileShare->getToId());
	cmd->Parameters->Add("@fId", userFileShare->getFId());
	cmd->Parameters->Add("@remark", userFileShare->getRemark());
	return cmd->ExecuteNonQuery();
}

Int32 CSFriend::UserFileShareDao::del(Int32 fId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM user_file_share WHERE `f_id` = ?", Conn);
	cmd->Parameters->Add("@fId", fId);
	return cmd->ExecuteNonQuery();
}

Int32 CSFriend::UserFileShareDao::del(Int32 fromId, Int32 toId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM user_file_share WHERE `from_id`= ? and `to_id` = ?", Conn);
	cmd->Parameters->Add("@fromId", fromId);
	cmd->Parameters->Add("@toId", toId);
	return cmd->ExecuteNonQuery();
}

Int32 CSFriend::UserFileShareDao::del(Int32 fromId, Int32 toId, Int32 fId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM user_file_share WHERE `from_id`= ? and `to_id` = ? and `f_id` = ?", Conn);
	cmd->Parameters->Add("@fromId", fromId);
	cmd->Parameters->Add("@toId", toId);
	cmd->Parameters->Add("@fId", fId);
	return cmd->ExecuteNonQuery();
}

Int32 CSFriend::UserFileShareDao::update(UserFileShare ^ userFileShare)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("UPDATE user_file_share SET `remark`= ? WHERE `from_id`= ? and `to_id`= ? and `f_id` = ?", Conn);
	cmd->Parameters->Add("@remark", userFileShare->getRemark());
	cmd->Parameters->Add("@fromId", userFileShare->getFromId());
	cmd->Parameters->Add("@toId", userFileShare->getToId());
	cmd->Parameters->Add("@fId", userFileShare->getFId());
	return cmd->ExecuteNonQuery();
}