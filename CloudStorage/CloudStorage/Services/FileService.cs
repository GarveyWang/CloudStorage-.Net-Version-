using System;
using System.Collections.Generic;

using CSAccount;
using CSFile;
using CSFriend;
using CSGroup;
using System.Data.Odbc;
using System.IO;
using CloudStorage.Models;


namespace CloudStorage.Services
{
    public class FileService
    {
        static private OdbcConnection Conn;

        static FileService()
        {
            Conn = new OdbcConnection("DSN=CloudStorage;UID=root;PWD=0530;");
            Conn.Open();
        }
        private UserDao userDao = new UserDao(Conn);

        private DirDao dirDao = new DirDao(Conn);

        private FileHeaderDao fileHeaderDao = new FileHeaderDao(Conn);

        private UserFileShareDao userFileShareDao = new UserFileShareDao(Conn);

        private GroupFileShareDao groupFileShareDao = new GroupFileShareDao(Conn);


        public bool CreateDir(Dir dir)
        {
            //若创建信息不完整，创建失败，返回
            if (dir == null || dir.getName() == null || "".Equals(dir.getName().Trim()))
            {
                return false;
            }

            //若创建信息包括非法字符，创建失败，返回
            if (dir.getName().Contains("/") || dir.getName().Contains("\\"))
            {
                return false;
            }

            //        //当前父目录下若具有重命文件夹，创建失败，返回
            //        List<Dir>childrenDirList=dirDao.selectByParentId(dir.getdId());
            //        for(Dir childrenDir:childrenDirList){
            //            if(childrenDir.getName().equals(dir.getName())){
            //                return CreateStateEnum.DIR_NAME_REPEAT;
            //            }
            //        }

            //开始插入文件夹记录
            dir.setCreateTime(DateTime.Now);
            int insertCount = dirDao.insert(dir);
            if (insertCount == 1)
            {
                return true;
            }
            return false;
        }

        public bool UpdateDir(Dir dir)
        {
            //若更新信息不完整，更新失败，返回
            if (dir == null || dir.getName() == null || "".Equals(dir.getName().Trim()))
            {
                return false;
            }

            //若更新信息包括非法字符，更新失败，返回
            if (dir.getName().Contains("/") || dir.getName().Contains("\\"))
            {
                return false;
            }

            //        //当前父目录下若具有重命文件夹，更新失败，返回
            //        List<Dir>siblings= getSiblingDirs(dir);
            //        for (Dir sibling:siblings){
            //            if(sibling.getName().equals(dir.getName())){
            //                return UpdateStateEnum.FAILED;
            //            }
            //        }

            //开始更新
            int updateCount = dirDao.update(dir);
            if (updateCount == 1)
            {
                return true;
            }
            return false;
        }

        //删除文件夹，需要考虑到其所有的子孙文件夹与文件。都需要删除，因此需要递归
        public bool DeleteDir(int dId)
        {
            //如果无此文件夹，返回删除失败
            Dir dir = dirDao.getDir(dId);
            if (dir == null)
            {
                return false;
            }

            //删除该文件夹下的第一代子文件
            List<FileHeader> firstChildrenFileHeaderList = GetFirstChildrenFileHeaders(dId);
            if (firstChildrenFileHeaderList != null)
            {
                foreach (FileHeader fileHeader in firstChildrenFileHeaderList)
                {
                    fileHeaderDao.del(fileHeader.getFId());
                }
            }

            //删除该文件夹下的第一代子文件夹。要用到递归
            List<Dir> firstChildrenDirList = GetFirstChildrenDirs(dId);
            if (firstChildrenDirList != null)
            {
                foreach (Dir child in firstChildrenDirList)
                {
                    //递归调用删除子文件夹
                    DeleteDir(child.getDId());
                    dirDao.del(child.getDId());
                }
            }

            //将该文件夹删除
            dirDao.del(dId);
            return true;
        }

        public Dir GetDir(int dId)
        {
            Dir dir = dirDao.getDir(dId);
            return dir;
        }

        public Dir GetDirWithFirstChildren(int dId)
        {
            Dir dir = dirDao.getDir(dId);
            //填充子文件List和子文件夹List
            if (dir != null)
            {
                List<Dir> childrenDirList = dirDao.getChildDirs(dId);
                List<FileHeader> childrenFileList = fileHeaderDao.getChildFileHeaders(dId);

                dir.setChildrenDirList(childrenDirList);
                dir.setChildrenFileHeaderList(childrenFileList);
            }
            return dir;
        }

        public List<Dir> GetFirstChildrenDirs(int dId)
        {
            List<Dir> childrenDirList = dirDao.getChildDirs(dId);
            return childrenDirList;
        }

        public List<FileHeader> GetFirstChildrenFileHeaders(int dId)
        {
            List<FileHeader> childrenFileList = fileHeaderDao.getChildFileHeaders(dId);
            return childrenFileList;
        }

        //返回一个包含所有子孙文件夹和子孙文件的文件夹对象，需要用到递归。
        public Dir GetDirWithAllChildren(int dId)
        {
            Dir rootDir = dirDao.getDir(dId);
            if (rootDir != null)
            {
                //添加该文件夹下的第一代子文件夹
                List<Dir> firstChildrenDirList = dirDao.getChildDirs(dId);
                if (firstChildrenDirList != null)
                {
                    int len = firstChildrenDirList.Count;
                    for (int i = 0; i < len; ++i)
                    {
                        Dir dir = firstChildrenDirList[i];
                        //递归调用，填充子文件夹的孙文件夹，孙文件
                        dir.setChildrenDirList(GetDirWithAllChildren(dir.getDId()).getChildrenDirList());
                    }
                    rootDir.setChildrenDirList(firstChildrenDirList);
                }
                //添加该文件夹下的第一代子文件
                List<FileHeader> firstChildrenFileHeaderList = fileHeaderDao.getChildFileHeaders(dId);
                rootDir.setChildrenFileHeaderList(firstChildrenFileHeaderList);
            }
            return rootDir;
        }

        public List<Dir> GetSiblingDirs(Dir dir)
        {
            //先找到父文件夹，再得到除了自己以外的父文件夹下的其他子文件夹，即为兄弟文件夹
            Dir parentDir = GetDirWithFirstChildren(dir.getParent());
            List<Dir> siblingsDirList = null;
            if (parentDir != null)
            {
                siblingsDirList = parentDir.getChildrenDirList();
                for (int i = siblingsDirList.Count-1; i>=0;--i)
                {
                    if (siblingsDirList[i].getDId() == dir.getDId())
                    {
                        siblingsDirList.Remove(siblingsDirList[i]);
                    }
                }
            }
            return siblingsDirList;
        }

        public List<Dir> GetPathList(Dir dir)
        {
            //不断寻找父文件夹，直到root文件夹（root文件夹的父文件夹为空）
            List<Dir> path = new List<Dir>();
            Dir currentDir = dir;
            path.Add(currentDir);
            while (currentDir.getParent() != -1)
            {
                currentDir = GetDir(currentDir.getParent());
                path.Add(currentDir);
            }
            path.Reverse();
            return path;
        }



        public bool CreateFileHeader(FileHeader fileHeader)
        {
            if (fileHeader == null)
            {
                return false;
            }

            fileHeader.setSubmitTime(DateTime.Now);
            int insertCount = fileHeaderDao.insert(fileHeader);
            if (insertCount == 1)
            {
                return true;
            }
            return false;
        }

        public bool DeleteFileHeader(int fId)
        {
            FileHeader fileHeader = GetFileHeader(fId);

            
            if (File.Exists(fileHeader.getPath()))
            {
                File.Delete(fileHeader.getPath());
            }

            userFileShareDao.del(fId);
            groupFileShareDao.delByFId(fId);
            int deleteCount = fileHeaderDao.del(fId);
            if (deleteCount == 1)
            {
                return true;
            }
            return false;
        }

        public FileHeader GetFileHeader(int fId)
        {
            return fileHeaderDao.getFileHeader(fId);
        }


        public bool UpdateFileHeader(FileHeader fileHeader)
        {
            if (fileHeader == null)
            {
                return false;
            }
            int updateCount = fileHeaderDao.update(fileHeader);
            if (updateCount == 1)
            {
                return true;
            }
            return false;
        }

        
        public List<FriendFileShareItem> GetFriendSharedFiles(int fromId, int toId)
        {
            //先得到UserFileShare的List，再填充FriendFileShareItemList
            List<UserFileShare> userFileShareList = userFileShareDao.getSharedFiles(fromId, toId);
            List<FriendFileShareItem> fileShareList = new List<FriendFileShareItem>();
            foreach (UserFileShare userFileShare in userFileShareList)
            {
                FriendFileShareItem shareItem = new FriendFileShareItem();
                FileHeader fileHeader = fileHeaderDao.getFileHeader(userFileShare.getFId());
                if (fileHeader != null)
                {
                    shareItem.setFileHeader(fileHeader);
                    shareItem.setUserFileShare(userFileShare);
                    fileShareList.Add(shareItem);
                }
            }
            return fileShareList;
        }

        public bool ShareFileToFriend(int fromId, int toId, int fId, String remark)
        {
            UserFileShare userFileShare = new UserFileShare(fromId,toId,fId,remark);
            if (userFileShareDao.insert(userFileShare) == 1)
            {
                return true;
            }
            return false;
        }

        public bool DeleteFriendSharedFile(int fromId, int toId, int fId)
        {
            if (userFileShareDao.del(fromId, toId, fId) == 1)
            {
                return true;
            }
            return false;
        }

        public List<GroupFileShareItem> GetGroupSharedFiles(int gId)
        {
            //先得到GroupFileShare的List，随后填充GroupFileShareItem
            List<GroupFileShare> groupFileShareList = groupFileShareDao.getSharedFiles(gId);
            List<GroupFileShareItem> fileShareList = new List<GroupFileShareItem>();

            foreach (GroupFileShare groupFileShare in groupFileShareList)
            {
                GroupFileShareItem shareItem = new GroupFileShareItem();
                FileHeader fileHeader = fileHeaderDao.getFileHeader(groupFileShare.getFId());
                if (fileHeader != null)
                {
                    User provider = userDao.getUser(groupFileShare.getProviderId());
                    shareItem.setFileHeader(fileHeader);
                    shareItem.setGroupFileShare(groupFileShare);
                    shareItem.setProvider(provider);
                    fileShareList.Add(shareItem);
                }
            }
            return fileShareList;
        }


        public bool ShareFileToGroup(int gId, int providerId, int fId, String remark)
        {
            GroupFileShare groupFileShare = new GroupFileShare(gId, providerId, fId, remark);
            if (groupFileShareDao.insert(groupFileShare) == 1)
            {
                return true;
            }
            return false;
        }
        

        public bool DeleteGroupSharedFile(int gId, int providerId, int fId)
        {
            int deleteCount = groupFileShareDao.del(gId, providerId, fId);
            if (deleteCount == 1)
            {
                return true;
            }
            return false;
        }


        public bool DeleteGroupSharedFile(int gId, int fId)
        {
            if (groupFileShareDao.delByGIdAndFId(gId, fId) == 1)
            {
                return true;
            }
            return false;
        }

    }
}


