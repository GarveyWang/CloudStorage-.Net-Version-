using System.Collections.Generic;

using CSAccount;
using CSGroup;
using System.Data.Odbc;
using System;

namespace CloudStorage.Services
{
    public class GroupService
    {
        static private OdbcConnection Conn;

        static GroupService()
        {
            Conn = new OdbcConnection("DSN=CloudStorage;UID=root;PWD=0530;");
            Conn.Open();
        }

        private UserDao userDao = new UserDao(Conn);

        private UserGroupDao userGroupDao = new UserGroupDao(Conn);

        private GroupMemberDao groupMemberDao = new GroupMemberDao(Conn);

        private GroupFileShareDao groupFileShareDao = new GroupFileShareDao(Conn);

        public UserGroup GetGroup(int gId)
        {
            return userGroupDao.getUserGroup(gId);
        }

        public List<UserGroup> getOwnedGroups(int builderId)
        {
            return userGroupDao.getUserGroups(builderId);
        }


        public List<UserGroup> GetJoinedGroups(int uId)
        {
            //先找到含有自己Id的GroupMember对象，再通过里面的gId，得到群组对象List
            List<GroupMember> groupMemberList = groupMemberDao.getGroupMembersByUId(uId);
            List<UserGroup> detailGroupList = new List<UserGroup>();
            foreach (GroupMember groupMember in groupMemberList)
            {
                UserGroup userGroup = userGroupDao.getUserGroup(groupMember.getGId());
                if (userGroup != null)
                {
                    detailGroupList.Add(userGroup);
                }
            }
            return detailGroupList;
        }

        public List<UserGroup> GetOwnedAndJoinedGroups(int uId)
        {
            List<UserGroup> groupList = getOwnedGroups(uId);
            groupList.AddRange(GetJoinedGroups(uId));
            return groupList;
        }

        public List<User> GetGroupMembers(int gId)
        {
            List<GroupMember> groupMemberList = groupMemberDao.getGroupMembersByGId(gId);
            List<User> detailMemberList = new List<User>();
            foreach (GroupMember groupMember in groupMemberList)
            {
                User member = userDao.getUser(groupMember.getUId());
                if (member != null)
                {
                    detailMemberList.Add(member);
                }
            }
            return detailMemberList;
        }

        public User GetGroupOwner(int gId)
        {
            UserGroup userGroup = userGroupDao.getUserGroup(gId);
            if (userGroup != null)
            {
                return userDao.getUser(userGroup.getBuilderId());
            }
            return null;
        }

        public bool DeleteGroup(int gId, int builderId)
        {
            UserGroup userGroup = userGroupDao.getUserGroup(gId);
            if (userGroup == null || userGroup.getBuilderId() != builderId)
            {
                return false;
            }
            if (userGroupDao.del(gId) == 1)
            {
                groupMemberDao.del(gId);
                groupFileShareDao.delByGId(gId);
                return true;
            }
            return false;
        }

        public bool CreateGroup(UserGroup userGroup)
        {
            if (userGroup == null || userGroup.getName() == null || userGroup.getName().Trim().Equals(""))
            {
                return false;
            }
            if (userGroupDao.insert(userGroup) == 1)
            {
                return true;
            }
            return false;
        }

        public GroupMember GetGroupMember(int gId, int uId)
        {
            return groupMemberDao.getGroupMember(gId, uId);
        }
        
        public List<User> GetUnpermittedMembers(int gId)
        {
            //GroupMember里含有uId，需要用此uId得到User对象
            List<GroupMember> groupMemberList = groupMemberDao.getUnpermittedGroupMembers(gId);
            List<User> detailMemberList = new List<User>();
            foreach (GroupMember groupMember in groupMemberList)
            {
                User member = userDao.getUser(groupMember.getUId());
                if (member != null)
                {
                    detailMemberList.Add(member);
                }
            }
            return detailMemberList;
        }

        public bool PermitMemberRequest(int gId, int uId)
        {
            Byte permitted = 1;
            GroupMember groupMember = new GroupMember(gId,uId,permitted);
            if (groupMemberDao.update(groupMember) == 1)
            {
                return true;
            }
            return false;
        }
        
        public bool RefuseGroupRequest(int gId, int uId)
        {
            if (groupMemberDao.del(gId, uId) == 1)
            {
                return true;
            }
            return false;
        }

        public bool DeleteGroupMember(int gId, int uId)
        {
            if (groupMemberDao.del(gId, uId) == 1)
            {
                groupFileShareDao.delByGIdAndProviderId(gId, uId);
                return true;
            }
            return false;
        }
        
        public bool SendJoinRequest(int gId, int uId)
        {
            GroupMember groupMember = new GroupMember();
            groupMember.setUId(uId);
            groupMember.setGId(gId);

            //如果该群为空，返回
            UserGroup userGroup = userGroupDao.getUserGroup(gId);
            if (userGroup == null)
            {
                return false;
            }

            //如果该群组创建者是自己，返回
            if (userGroup.getBuilderId() == uId)
            {
                return false;
            }

            //如果已经加入了该群组，返回
            GroupMember groupMemberFromDB = groupMemberDao.getGroupMember(gId, uId);
            if (groupMemberFromDB != null)
            {
                return false;
            }

            byte permitted;
            //如果群组是公开的，可以直接加入，如果不是，暂时将permitted字段设为0
            if (userGroup.getType().Equals("PRIVATE"))
            {
                permitted = 0;
            }
            else
            {
                permitted = 1;
            }
            groupMember.setPermitted(permitted);
            if (groupMemberDao.insert(groupMember) == 1)
            {
                return true;
            }
            return false;
        }
    }
}