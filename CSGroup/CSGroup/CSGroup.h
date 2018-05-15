// CSGroup.h

#pragma once

using namespace System;
using namespace System::Data;
using namespace System::Data::Odbc;

using namespace System::Collections::Generic;

namespace CSGroup {

	public ref class UserGroup
	{
	public:
		UserGroup();
		UserGroup(Int32 gId, String^ name, String^ type, Int32 builderId);

		Int32 getGId();
		String^ getName();
		String^ getType();
		Int32 getBuilderId();

		void setGId(Int32 gId);
		void setName(String^ name);
		void setType(String^ type);
		void setBuilderId(Int32 builderId);

	private:
		Int32 gId;
		String^ name;
		String^ type;
		Int32 builderId;
	};

	public ref class GroupMember
	{
	public:
		GroupMember();
		GroupMember(Int32 gId, Int32 uId, Byte^ permitted);

		Int32 getGId();
		Int32 getUId();
		Byte^ getPermitted();

		void setGId(Int32 gId);
		void setUId(Int32 uId);
		void setPermitted(Byte^ permitted);

	private:
		Int32 gId;
		Int32 uId;
		Byte^ permitted;
	};

	public ref class GroupFileShare 
	{
	public:
		GroupFileShare();
		GroupFileShare(Int32 gId, Int32 providerId, Int32 fId, String^ remark);

		Int32 getGId();
		Int32 getProviderId();
		Int32 getFId();
		String^ getRemark();

		void setGId(Int32 gId);
		void setProviderId(Int32 providerId);
		void setFId(Int32 fId);
		void setRemark(String^ remark);


	private:
		Int32 gId;
		Int32 providerId;
		Int32 fId;
		String^ remark;
	};



	public ref class UserGroupDao
	{
	public:
		UserGroupDao(OdbcConnection^ Conn);
		UserGroup^ getUserGroup(Int32 gId);
		List<UserGroup^>^ getUserGroups(Int32 builderId);

		Int32 insert(UserGroup^ userGroup);
		Int32 del(Int32 gId);
		Int32 update(UserGroup^ userGroup);

	private:
		OdbcConnection^ Conn;
	};

	public ref class GroupMemberDao
	{
	public:
		GroupMemberDao(OdbcConnection^ Conn);
		GroupMember^ getGroupMember(Int32 gId, Int32 uId);
		List<GroupMember^>^ getGroupMembersByUId(Int32 uId);
		List<GroupMember^>^ getGroupMembersByGId(Int32 gId);
		List<GroupMember^>^ getUnpermittedGroupMembers(Int32 gId);

		Int32 insert(GroupMember^ groupMember);
		Int32 del(Int32 gId, Int32 uId);
		Int32 del(Int32 gId);
		Int32 update(GroupMember^ groupMember);

	private:
		OdbcConnection^ Conn;
	};

	public ref class GroupFileShareDao
	{
	public:
		GroupFileShareDao(OdbcConnection^ Conn);
		GroupFileShare^ getSharedFile(Int32 gId, Int32 providerId, Int32 fId);
		List<GroupFileShare^>^ getSharedFiles(Int32 gId);

		Int32 insert(GroupFileShare^ groupFileShare);
		Int32 del(Int32 gId, Int32 providerId, Int32 fId);
		Int32 delByGIdAndProviderId(Int32 gId, Int32 providerId);
		Int32 delByGId(Int32 gId);
		Int32 delByFId(Int32 fId);
		Int32 delByGIdAndFId(Int32 gId, Int32 fId);
		Int32 update(GroupFileShare^ groupFileShare);

	private:
		OdbcConnection^ Conn;
	};

}
