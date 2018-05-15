using CSFile;
using CSFriend;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace CloudStorage.Models
{
    public class FriendFileShareItem
    {
        //好友分享文件信息，主要是因为要用到里面的remark属性
        private UserFileShare userFileShare;

        //文件信息
        private FileHeader fileHeader;

        public UserFileShare getUserFileShare()
        {
            return userFileShare;
        }

        public void setUserFileShare(UserFileShare userFileShare)
        {
            this.userFileShare = userFileShare;
        }

        public FileHeader getFileHeader()
        {
            return fileHeader;
        }

        public void setFileHeader(FileHeader fileHeader)
        {
            this.fileHeader = fileHeader;
        }
    }
}