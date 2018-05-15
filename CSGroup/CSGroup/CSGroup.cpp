// 这是主 DLL 文件。

#include "stdafx.h"

#include "CSGroup.h"

using namespace CSGroup;

CSGroup::UserGroup::UserGroup()
{
}

CSGroup::UserGroup::UserGroup(Int32 gId, String ^ name, String ^ type, Int32 builderId)
{
	this->gId = gId;
	this->name = name;
	this->type = type;
	this->builderId = builderId;
}

Int32 CSGroup::UserGroup::getGId()
{
	return gId;
}

String ^ CSGroup::UserGroup::getName()
{
	return name;
}

String ^ CSGroup::UserGroup::getType()
{
	return type;
}

Int32 CSGroup::UserGroup::getBuilderId()
{
	return builderId;
}

void CSGroup::UserGroup::setGId(Int32 gId)
{
	this->gId = gId;
}

void CSGroup::UserGroup::setName(String ^ name)
{
	this->name = name;
}

void CSGroup::UserGroup::setType(String ^ type)
{
	this->type = type;
}

void CSGroup::UserGroup::setBuilderId(Int32 builderId)
{
	this->builderId = builderId;
}

CSGroup::GroupMember::GroupMember()
{
}

CSGroup::GroupMember::GroupMember(Int32 gId, Int32 uId, Byte ^ permitted)
{
	this->gId = gId;
	this->uId = uId;
	this->permitted = permitted;
}

Int32 CSGroup::GroupMember::getGId()
{
	return gId;
}

Int32 CSGroup::GroupMember::getUId()
{
	return uId;
}

Byte ^ CSGroup::GroupMember::getPermitted()
{
	return permitted;
}

void CSGroup::GroupMember::setGId(Int32 gId)
{
	this->gId = gId;
}

void CSGroup::GroupMember::setUId(Int32 uId)
{
	this->uId = uId;
}

void CSGroup::GroupMember::setPermitted(Byte ^ permitted)
{
	this->permitted = permitted;
}

CSGroup::UserGroupDao::UserGroupDao(OdbcConnection ^ Conn)
{
	this->Conn = Conn;
}

UserGroup ^ CSGroup::UserGroupDao::getUserGroup(Int32 gId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from user_group where g_id = ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "userGroups");
	if (ds->Tables->Count == 0 || ds->Tables["userGroups"]->Rows->Count == 0) {
		return nullptr;
	}
	DataRow^ row = ds->Tables["userGroups"]->Rows[0];

	UserGroup^ userGroup = gcnew UserGroup((Int32)row["g_id"], (String^)row["name"], (String^)row["type"], (Int32)row["builder_id"]);
	return userGroup;
}

List<UserGroup^>^ CSGroup::UserGroupDao::getUserGroups(Int32 builderId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from user_group where builder_id = ?", Conn);
	cmd->Parameters->Add("@builderId", builderId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "userGroups");

	List<UserGroup^>^ userGroups = gcnew List<UserGroup^>();

	for each (DataRow^ row in ds->Tables["userGroups"]->Rows)
	{
		UserGroup^ userGroup = gcnew UserGroup((Int32)row["g_id"], (String^)row["name"], (String^)row["type"], (Int32)row["builder_id"]);
		userGroups->Add(userGroup);
	}

	return userGroups;
}

Int32 CSGroup::UserGroupDao::insert(UserGroup ^ userGroup)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("INSERT ignore INTO user_group (`name`, `type`, `builder_id`) VALUES (?, ?, ?)", Conn);
	cmd->Parameters->Add("@name", userGroup->getName());
	cmd->Parameters->Add("@type", userGroup->getType());
	cmd->Parameters->Add("@builderId", userGroup->getBuilderId());
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::UserGroupDao::del(Int32 gId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM user_group WHERE `g_id`= ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::UserGroupDao::update(UserGroup ^ userGroup)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("UPDATE user_group SET `name`= ?, `type`= ?, `builderId`= ? WHERE `g_id`= ? ", Conn);
	cmd->Parameters->Add("@name", userGroup->getName());
	cmd->Parameters->Add("@type", userGroup->getType());
	cmd->Parameters->Add("@builderId", userGroup->getBuilderId());
	cmd->Parameters->Add("@gId", userGroup->getGId());
	return cmd->ExecuteNonQuery();
}

CSGroup::GroupFileShare::GroupFileShare()
{
}

CSGroup::GroupFileShare::GroupFileShare(Int32 gId, Int32 providerId, Int32 fId, String ^ remark)
{
	this->gId = gId;
	this->providerId = providerId;
	this->fId = fId;
	this->remark = remark;
}

Int32 CSGroup::GroupFileShare::getGId()
{
	return gId;
}

Int32 CSGroup::GroupFileShare::getProviderId()
{
	return providerId;
}

Int32 CSGroup::GroupFileShare::getFId()
{
	return fId;
}

String ^ CSGroup::GroupFileShare::getRemark()
{
	return remark;
}

void CSGroup::GroupFileShare::setGId(Int32 gId)
{
	this->gId = gId;
}

void CSGroup::GroupFileShare::setProviderId(Int32 providerId)
{
	this->providerId = providerId;
}

void CSGroup::GroupFileShare::setFId(Int32 fId)
{
	this->fId = fId;
}

void CSGroup::GroupFileShare::setRemark(String^ remark)
{
	this->remark = remark;
}

CSGroup::GroupMemberDao::GroupMemberDao(OdbcConnection ^ Conn)
{
	this->Conn = Conn;
}

GroupMember ^ CSGroup::GroupMemberDao::getGroupMember(Int32 gId, Int32 uId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from group_member where g_id = ? and u_id = ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	cmd->Parameters->Add("@uId", uId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "groupMembers");

	if (ds->Tables->Count ==0 || ds->Tables["groupMembers"]->Rows->Count == 0) {
		return nullptr;
	}
	DataRow^ row = ds->Tables["groupMembers"]->Rows[0];

	GroupMember^ groupMember = gcnew GroupMember((Int32)row["g_id"], (Int32)row["u_id"], Byte::Parse(row["permitted"]->ToString()));
	return groupMember;
}

List<GroupMember^>^ CSGroup::GroupMemberDao::getGroupMembersByUId(Int32 uId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from group_member where u_id = ? and permitted = 1", Conn);
	cmd->Parameters->Add("@uId", uId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "groupMembers");

	List<GroupMember^>^ groupMembers = gcnew List<GroupMember^>();

	for each (DataRow^ row in ds->Tables["groupMembers"]->Rows)
	{
		GroupMember^ groupMember = gcnew GroupMember((Int32)row["g_id"], (Int32)row["u_id"], Byte::Parse(row["permitted"]->ToString()));
		groupMembers->Add(groupMember);
	}

	return groupMembers;
}

List<GroupMember^>^ CSGroup::GroupMemberDao::getGroupMembersByGId(Int32 gId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from group_member where g_id = ? and permitted = 1", Conn);
	cmd->Parameters->Add("@gId", gId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "groupMembers");

	List<GroupMember^>^ groupMembers = gcnew List<GroupMember^>();

	for each (DataRow^ row in ds->Tables["groupMembers"]->Rows)
	{
		GroupMember^ groupMember = gcnew GroupMember((Int32)row["g_id"], (Int32)row["u_id"], Byte::Parse(row["permitted"]->ToString()));
		groupMembers->Add(groupMember);
	}

	return groupMembers;
}

List<GroupMember^>^ CSGroup::GroupMemberDao::getUnpermittedGroupMembers(Int32 gId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from group_member where g_id = ? and permitted = 0", Conn);
	cmd->Parameters->Add("@gId", gId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "groupMembers");

	List<GroupMember^>^ groupMembers = gcnew List<GroupMember^>();

	for each (DataRow^ row in ds->Tables["groupMembers"]->Rows)
	{
		GroupMember^ groupMember = gcnew GroupMember((Int32)row["g_id"], (Int32)row["u_id"], Byte::Parse(row["permitted"]->ToString()));
		groupMembers->Add(groupMember);
	}

	return groupMembers;
}

Int32 CSGroup::GroupMemberDao::insert(GroupMember ^ groupMember)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("INSERT ignore Into group_member (`g_id`, `u_id`, `permitted`) VALUES (?, ?, ?)", Conn);
	cmd->Parameters->Add("@gId", groupMember->getGId());
	cmd->Parameters->Add("@uId", groupMember->getUId());
	cmd->Parameters->Add("@permitted", groupMember->getPermitted());
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::GroupMemberDao::del(Int32 gId, Int32 uId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM group_member WHERE `g_id`= ? and `u_id` = ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	cmd->Parameters->Add("@uId", uId);
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::GroupMemberDao::del(Int32 gId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM group_member WHERE `g_id`= ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::GroupMemberDao::update(GroupMember ^ groupMember)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("UPDATE group_member SET `permitted`= ? WHERE `g_id`= ? and `u_id`= ? ", Conn);
	cmd->Parameters->Add("@permitted", groupMember->getPermitted());
	cmd->Parameters->Add("@gId", groupMember->getGId());
	cmd->Parameters->Add("@uId", groupMember->getUId());
	return cmd->ExecuteNonQuery();
}

CSGroup::GroupFileShareDao::GroupFileShareDao(OdbcConnection ^ Conn)
{
	this->Conn = Conn;
}

GroupFileShare ^ CSGroup::GroupFileShareDao::getSharedFile(Int32 gId, Int32 providerId, Int32 fId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from group_file_share where g_id = ? and provider_id = ? and fId = ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	cmd->Parameters->Add("@providerId", providerId);
	cmd->Parameters->Add("@fId", fId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "sharedFiles");
	if (ds->Tables->Count == 0 || ds->Tables["sharedFiles"]->Rows->Count == 0) {
		return nullptr;
	}
	DataRow^ row = ds->Tables["sharedFiles"]->Rows[0];

	GroupFileShare^ groupFileShare = gcnew GroupFileShare((Int32)row["g_id"], (Int32)row["provider_id"], (Int32)row["f_id"], (String^)row["remark"]);
	return groupFileShare;
}

List<GroupFileShare^>^ CSGroup::GroupFileShareDao::getSharedFiles(Int32 gId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("select * from group_file_share where g_id = ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	OdbcDataAdapter^ Adapter = gcnew OdbcDataAdapter(cmd);
	DataSet^ ds = gcnew DataSet();
	Adapter->Fill(ds, "sharedFiles");
	if (ds->Tables->Count == 0) {
		return nullptr;
	}

	List<GroupFileShare^>^ sharedFiles = gcnew List<GroupFileShare^>();

	for each (DataRow^ row in ds->Tables["sharedFiles"]->Rows)
	{
		GroupFileShare^ groupFileShare = gcnew GroupFileShare((Int32)row["g_id"], (Int32)row["provider_id"], (Int32)row["f_id"], (String^)row["remark"]);
		sharedFiles->Add(groupFileShare);
	}

	return sharedFiles;
}

Int32 CSGroup::GroupFileShareDao::insert(GroupFileShare ^ groupFileShare)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("INSERT ignore Into group_file_share (`g_id`, `provider_id`, `f_id`, `remark`) VALUES (?, ?, ?, ?)", Conn);
	cmd->Parameters->Add("@gId", groupFileShare->getGId());
	cmd->Parameters->Add("@providerId", groupFileShare->getProviderId());
	cmd->Parameters->Add("@fId", groupFileShare->getFId());
	cmd->Parameters->Add("@remark", groupFileShare->getRemark());
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::GroupFileShareDao::del(Int32 gId, Int32 providerId, Int32 fId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM group_file_share WHERE `g_id`= ? and `provider_id` = ? and `f_id` = ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	cmd->Parameters->Add("@providerId", providerId);
	cmd->Parameters->Add("@fId", fId);
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::GroupFileShareDao::delByGIdAndProviderId(Int32 gId, Int32 providerId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM group_file_share WHERE `g_id`= ? and `provider_id` = ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	cmd->Parameters->Add("@providerId", providerId);
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::GroupFileShareDao::delByGId(Int32 gId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM group_file_share WHERE `g_id`= ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::GroupFileShareDao::delByFId(Int32 fId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM group_file_share WHERE `f_id`= ?", Conn);
	cmd->Parameters->Add("@fId", fId);
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::GroupFileShareDao::delByGIdAndFId(Int32 gId, Int32 fId)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM group_file_share WHERE `g_id`= ? and `f_id` = ?", Conn);
	cmd->Parameters->Add("@gId", gId);
	cmd->Parameters->Add("@fId", fId);
	return cmd->ExecuteNonQuery();
}

Int32 CSGroup::GroupFileShareDao::update(GroupFileShare ^ groupFileShare)
{
	OdbcCommand^ cmd = gcnew OdbcCommand("UPDATE group_file_share SET `remark`= ? WHERE `g_id`= ? and `provider_id`= ? and `f_id` = ?", Conn);
	cmd->Parameters->Add("@remark", groupFileShare->getRemark());
	cmd->Parameters->Add("@gId", groupFileShare->getGId());
	cmd->Parameters->Add("@providerId", groupFileShare->getProviderId());
	cmd->Parameters->Add("@fId", groupFileShare->getFId());
	return cmd->ExecuteNonQuery();
}
