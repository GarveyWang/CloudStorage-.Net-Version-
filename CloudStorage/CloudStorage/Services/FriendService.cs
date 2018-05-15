using System.Collections.Generic;

using CSFile;
using CSAccount;
using CSFriend;
using System.Data.Odbc;
using System;

namespace CloudStorage.Services
{
    public class FriendService
    {
        static private OdbcConnection Conn;
    
        static FriendService()
        {
            Conn = new OdbcConnection("DSN=CloudStorage;UID=root;PWD=0530;");
            Conn.Open();
        }

        private UserFriendDao userFriendDao = new UserFriendDao(Conn);

        private UserDao userDao = new UserDao(Conn);

        private UserFileShareDao fileShareDao = new UserFileShareDao(Conn);

        public List<User> GetPermittedFriends(int uId)
        {
            List<UserFriend> permittedFriendsList = userFriendDao.getUserFriends(uId);
            List<User> permittedFriendsDetailList = new List<User>();
            foreach (UserFriend userFriend in permittedFriendsList)
            {
                int friendId;
                //在UserFriend对象中，属性toId和fromId之一为本用户Id，另一个就是好友Id
                if (userFriend.getToId() != uId)
                {
                    friendId = userFriend.getToId();
                }
                else
                {
                    friendId = userFriend.getFromId();
                }
                User friendDetail = userDao.getUser(friendId);
                permittedFriendsDetailList.Add(friendDetail);
            }
            return permittedFriendsDetailList;
        }

        public List<User> GetFriendRequest(int uId)
        {
            List<UserFriend> unpermittedFriendsList = userFriendDao.getFriendRequests(uId);
            List<User> unpermittedFriendsDetailList = new List<User>();
            foreach (UserFriend userFriend in unpermittedFriendsList)
            {
                int friendId;
                //在UserFriend对象中，属性toId和fromId之一为本用户Id，另一个就是好友Id
                if (userFriend.getFromId() != uId)
                {
                    friendId = userFriend.getFromId();
                }
                else
                {
                    friendId = userFriend.getToId();
                }
                User friendDetail = userDao.getUser(friendId);
                unpermittedFriendsDetailList.Add(friendDetail);
            }
            return unpermittedFriendsDetailList;
        }


        public bool PermitFriendRequest(int uId, int friendId)
        {
            //将permitted字段设为1，即为同意好友申请，两个用户成为好友
            byte permitted = 1;
            
            UserFriend userFriend = new UserFriend(friendId, uId, permitted);
            if (userFriendDao.update(userFriend) == 1)
            {
                return true;
            }
            return false;
        }


    public bool DeleteFriend(int uId, int friendId)
        {
            int deleteCount = userFriendDao.del(uId,friendId);
            if (deleteCount != 1)
            {
                //可能是因为userFriendKey中的toId和fromId的次序问题，导致没删除。将两个参数颠倒，再尝试一次删除
                deleteCount = userFriendDao.del(friendId,uId);
            }
            fileShareDao.del(uId, friendId);
            fileShareDao.del(friendId, uId);
            if (deleteCount == 1)
            {
                return true;
            }
            return false;
        }
        
        public bool SendFriendRequest(int fromId, int toId)
        {
            UserFriend userFriend = new UserFriend();
            userFriend.setFromId(fromId);
            userFriend.setToId(toId);

            //如果是公共账号，直接同意好友申请，直接成为好友
            User friend = userDao.getUser(toId);
            if (friend.getType().Equals("PRIVATE"))
            {
                userFriend.setPermitted((byte)0);
            }
            else
            {
                userFriend.setPermitted((byte)1);
            }

            if (userFriendDao.insert(userFriend) == 1)
            {
                return true;
            }
            return false;
        }

    }
}