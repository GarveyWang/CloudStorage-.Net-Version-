using CSAccount;
using CSFile;
using CSGroup;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace CloudStorage.Models
{
    public class GroupFileShareItem
    {
        //群组分享信息，主要是因为要用到里面的remark属性
        private GroupFileShare groupFileShare;

        //分享文件
        private FileHeader fileHeader;

        //分享者
        private User provider;

        public GroupFileShare getGroupFileShare()
        {
            return groupFileShare;
        }

        public void setGroupFileShare(GroupFileShare groupFileShare)
        {
            this.groupFileShare = groupFileShare;
        }

        public FileHeader getFileHeader()
        {
            return fileHeader;
        }

        public void setFileHeader(FileHeader fileHeader)
        {
            this.fileHeader = fileHeader;
        }

        public User getProvider()
        {
            return provider;
        }

        public void setProvider(User provider)
        {
            this.provider = provider;
        }
    }
}