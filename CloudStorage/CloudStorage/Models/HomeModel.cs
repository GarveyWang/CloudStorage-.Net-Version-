using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

using CSAccount;
using CSFriend;
using CSFile;
using CSGroup;

namespace CloudStorage.Models
{
    public class HomeModel
    {
        public int uId;
        public String userName;
        public int dId;
        public int rootDir;
        public int recyclebin;
        public List<User> friendList;
        public List<FileHeader> fileHeaderList;
        public List<Dir> dirList;
        public List<Dir> pathList;
        public List<Dir> moveList;
        public List<UserGroup> groupList;
    }
}