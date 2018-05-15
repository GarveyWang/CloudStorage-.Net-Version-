using System;

using CSAccount;
using CSFile;
using System.Data.Odbc;

namespace CloudStorage.Services
{
    
    public class AccountService
    {
        static private OdbcConnection Conn;
        
        static AccountService()
        {
            Conn = new OdbcConnection("DSN=CloudStorage;UID=root;PWD=0530;");
            Conn.Open();
        }

        private UserDao userDao = new UserDao(Conn);

        private DirDao dirDao = new DirDao(Conn);

        public User GetUser(int uId)
        {
            return userDao.getUser(uId);
        }

        public bool Register(User user)
        {
            if (user == null || user.getName() == null || "".Equals(user.getName().Trim())
                || user.getPwd() == null || "".Equals(user.getPwd().Trim())
                || user.getUId() <= 0)
            {
                return false;
            }

            User userFromDB = userDao.getUser(user.getUId());
            if (userFromDB != null)
            {
                return false;
            }

            Dir rootDir = new Dir();
            rootDir.setCreateTime(DateTime.Now);
            rootDir.setName("root");
            rootDir.setParent(-1);
            dirDao.insert(rootDir);

            user.setRootDir(dirDao.getLastId());

            Dir recyclebin = new Dir();
            recyclebin.setCreateTime(DateTime.Now);
            recyclebin.setName("recyclebin");
            dirDao.insert(recyclebin);
            user.setRecyclebin(dirDao.getLastId());

            int insertCount = userDao.insert(user);
            if (insertCount == 1)
            {
                return true;
            }
            return false;
        }

        public bool Validate(User user)
        {
            //如果用户信息为空，返回信息不完整状态
            if (user == null || user.getPwd() == null)
            {
                return false;
            }
            User userFromDB = userDao.getUser(user.getUId());
            //如果无此账号或者账号密码不正确，返回信息错误状态
            if (userFromDB == null || !userFromDB.getPwd().Equals(user.getPwd()))
            {
                return false;
            }
            //验证通过
            return true;
        }
    }
}