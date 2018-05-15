using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

using CSAccount;
using CSFile;
using CSGroup;
using CloudStorage.Services;
using System.Data;
using System.Data.Odbc;
using System.Text;
using System.IO;

namespace CloudStorage.Controllers
{
    public class FileController : Controller
    {
        private FileService fileService = new FileService();

        [HttpPost]
        public ActionResult Upload()
        {
            int uId = (int)Session["uId"];
            int currentDir = (int)Session["currentDir"];
            HttpFileCollectionBase files = HttpContext.Request.Files;

            if (files.Count > 0)
            {
                String path = "G:\\_NET\\CloudStorage\\File\\" + uId;

                if (!System.IO.Directory.Exists(path))
                {
                    System.IO.Directory.CreateDirectory(path);
                }

                path += ("\\" + DateTime.Now.GetHashCode().ToString() + files[0].FileName);

                System.IO.Stream stream = files[0].InputStream;
                files[0].SaveAs(path);

                FileHeader fileHeader = new FileHeader();
                fileHeader.setName(files[0].FileName);
                fileHeader.setSize((int)files[0].ContentLength);
                fileHeader.setParent(currentDir);
                fileHeader.setUId(uId);
                fileHeader.setPath(path);
                fileService.CreateFileHeader(fileHeader);
            }
            return RedirectToAction("List", "Home", new { dId = currentDir });
        }

        [HttpPost]
        public void Download(FormCollection form)
        {
            int fId = int.Parse(form["fId"]);
            FileHeader fileHeader = fileService.GetFileHeader(fId);

            System.IO.FileInfo fileInfo = new System.IO.FileInfo(fileHeader.getPath());

            if (fileInfo.Exists == true)
            {
                const long ChunkSize = 102400;//100K 每次读取文件，只读取100Ｋ，这样可以缓解服务器的压力  
                byte[] buffer = new byte[ChunkSize];

                Response.Clear();
                System.IO.FileStream iStream = System.IO.File.OpenRead(fileHeader.getPath());
                long dataLengthToRead = iStream.Length;//获取下载的文件总大小  
                Response.ContentType = "application/octet-stream";
                Response.AddHeader("Content-Disposition", "attachment; filename=" + HttpUtility.UrlEncode(fileHeader.getName()));
                while (dataLengthToRead > 0 && Response.IsClientConnected)
                {
                    int lengthRead = iStream.Read(buffer, 0, Convert.ToInt32(ChunkSize));//读取的大小  
                    Response.OutputStream.Write(buffer, 0, lengthRead);
                    Response.Flush();
                    dataLengthToRead = dataLengthToRead - lengthRead;
                }
                Response.Close();
            }
        }

        [HttpPost]
        public ActionResult RemoveFile(FormCollection form)
        {
            int recyclebin = (int)Session["recyclebin"];
            int fId = int.Parse(form["fId"]);

            FileHeader fileHeader = fileService.GetFileHeader(fId);
            fileHeader.setParent(recyclebin);
            fileService.UpdateFileHeader(fileHeader);

            int currentDir = (int)Session["currentDir"];
            return RedirectToAction("List", "Home", new { did = currentDir });
        }

        [HttpPost]
        public ActionResult RecoverFile(FormCollection form)
        {
            FileHeader fileHeader = fileService.GetFileHeader(int.Parse(form["fId"]));
            fileHeader.setParent(int.Parse(form["newParentId"]));
            fileService.UpdateFileHeader(fileHeader);

            int currentDir = (int)Session["currentDir"];
            return RedirectToAction("Recyclebin", "Home", new { did = currentDir });
        }

        [HttpPost]
        public ActionResult DeleteFile(FormCollection form)
        {
            int currentDir = (int)Session["currentDir"];
            fileService.DeleteFileHeader(int.Parse(form["fId"]));
            return RedirectToAction("Recyclebin", "Home", new { did = currentDir });
        }



        [HttpPost]
        public ActionResult ShareToFriend(FormCollection form)
        {
            int currentDir = (int)Session["currentDir"];
            int uId = (int)Session["uId"];
            fileService.ShareFileToFriend(uId, int.Parse(form["friendId"]),int.Parse(form["fId"]),form["remark"]);
            return RedirectToAction("List", "Home", new { did = currentDir });
        }

        [HttpPost]
        public ActionResult CancelShareToFriend(FormCollection form)
        {
            int uId = (int)Session["uId"];
            fileService.DeleteFriendSharedFile(uId, int.Parse(form["friendId"]), int.Parse(form["fId"]));
            TempData["friendId"] = int.Parse(form["friendId"]);
            return RedirectToAction("ShareFiles", "Friend");
        }

        [HttpPost]
        public ActionResult ShareToGroup(FormCollection form)
        {
            int currentDir = (int)Session["currentDir"];
            int uId = (int)Session["uId"];
            fileService.ShareFileToGroup( int.Parse(form["gId"]), uId, int.Parse(form["fId"]), form["remark"]);
            return RedirectToAction("List", "Home", new { did = currentDir });
        }

        [HttpPost]
        public ActionResult CancelShareToGroup(FormCollection form)
        {
            int currentDir = (int)Session["currentDir"];
            int uId = (int)Session["uId"];
            TempData["gId"] = int.Parse(form["gId"]);
            fileService.DeleteGroupSharedFile(int.Parse(form["gId"]), uId, int.Parse(form["fId"]));
            return RedirectToAction("Detail", "Group");
        }



        [HttpPost]
        public ActionResult AddDir(FormCollection form)
        {
            Dir dir = new Dir();
            int currentDir = (int)Session["currentDir"];
            dir.setParent(currentDir);
            dir.setName(form["name"]);
            fileService.CreateDir(dir);
            return RedirectToAction("List", "Home", new { dId = currentDir });
        }

        [HttpPost]
        public ActionResult RemoveDir(FormCollection form)
        {
            int recyclebin = (int)Session["recyclebin"];
            int dId = int.Parse(form["dId"]);
            Dir dir = fileService.GetDir(dId);
            dir.setParent(recyclebin);
            fileService.UpdateDir(dir);

            int currentDir = (int)Session["currentDir"];
            return RedirectToAction("List","Home",new { did = currentDir});
        }

        [HttpPost]
        public ActionResult RecoverDir(FormCollection form)
        {
            Dir dir = fileService.GetDir(int.Parse(form["dId"]));
            dir.setParent(int.Parse(form["newParentId"]));
            fileService.UpdateDir(dir);

            int currentDir = (int)Session["currentDir"];
            return RedirectToAction("Recyclebin", "Home", new { did = currentDir });
        }

        [HttpPost]
        public ActionResult RenameDir(FormCollection form)
        {
            Dir dir = fileService.GetDir(int.Parse(form["dId"]));
            dir.setName(form["name"]);
            fileService.UpdateDir(dir);

            int currentDir = (int)Session["currentDir"];
            return RedirectToAction("List", "Home", new { did = currentDir });
        }

        [HttpPost]
        public ActionResult MoveDir(FormCollection form)
        {
            Dir dir = fileService.GetDir(int.Parse(form["dId"]));
            dir.setParent(int.Parse(form["newParentId"]));
            fileService.UpdateDir(dir);

            int currentDir = (int)Session["currentDir"];
            return RedirectToAction("List", "Home", new { did = currentDir });
        }

        [HttpPost]
        public ActionResult DeleteDir(FormCollection form)
        {
            int currentDir = (int)Session["currentDir"];
            fileService.DeleteDir(int.Parse(form["dId"]));
            return RedirectToAction("Recyclebin", "Home", new { did = currentDir });
        }

        [HttpPost]
        public ActionResult MoveFile(FormCollection form)
        {
            FileHeader fileHeader = fileService.GetFileHeader(int.Parse(form["fId"]));
            fileHeader.setParent(int.Parse(form["newParentId"]));
            fileService.UpdateFileHeader(fileHeader);

            int currentDir = (int)Session["currentDir"];
            return RedirectToAction("List", "Home", new { did = currentDir });
        }

        [HttpPost]
        public String GetChildDirs(int srcId, int dId, String moveType)
        {
            Dir dir = fileService.GetDirWithFirstChildren(dId);

            StringBuilder responseText = new StringBuilder();
            if (moveType.Equals("dir"))
            {
                responseText.Append("<div class=\"btn-group\"><button class=\"btn btn-default\" id=\"dir_move_btn\" d_id=\"");
            }
            else
            {
                responseText.Append("<div class=\"btn-group\"><button class=\"btn btn-default\" id=\"file_move_btn\" d_id=\"");
            }

            responseText.Append(dId);
            responseText.Append("\">移入</button>");
            if (dir.getParent() != -1)
            {
                responseText.Append("<button class=\"btn btn-default\" id=\"");
                if (moveType.Equals("dir"))
                {
                    responseText.Append("dir_move_back_btn");
                }
                else
                {
                    responseText.Append("file_move_back_btn");
                }
                responseText.Append("\" d_id=\"");
                responseText.Append(dir.getParent());
                responseText.Append("\">返回上一级</button>");
            }

            responseText.Append("<button type=\"button\" id=\"");
            if (moveType.Equals("dir"))
            {
                responseText.Append("dir_move_form_close_btn");
            }
            else
            {
                responseText.Append("file_move_form_close_btn");
            }
            responseText.Append("\" class=\"btn btn-default\" data-dismiss=\"modal\">取消</button></div>");

            List<Dir> childrenDirList = dir.getChildrenDirList();
            foreach (Dir child in childrenDirList)
            {
                if (moveType.Equals("dir") && child.getDId() == srcId)
                {
                    continue;
                }
                if (moveType.Equals("dir"))
                {
                    responseText.Append("<li class=\"list-group-item\"><button class=\"btn btn-default dir_move_enter_btn\" d_id=\"");
                }
                else
                {
                    responseText.Append("<li class=\"list-group-item\"><button class=\"btn btn-default file_move_enter_btn\" d_id=\"");
                }
                responseText.Append(child.getDId());
                responseText.Append("\">打开</button>");
                responseText.Append(child.getName());
                responseText.Append("</li>");
            }
            return responseText.ToString();
        }

    }
}