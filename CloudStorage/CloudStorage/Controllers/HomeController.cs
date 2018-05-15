using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

using CloudStorage.Models;
using CloudStorage.Services;

using CSAccount;
using CSFile;
using CSGroup;
using System.Data;
using System.Data.Odbc;


namespace CloudStorage.Controllers
{
    public class HomeController : Controller
    {
        private AccountService accountService = new AccountService();

        private FileService fileService = new FileService();

        private FriendService friendService = new FriendService();

        private GroupService groupService = new GroupService();


        // Get: List
        public ActionResult List(String dId)
        {
            if (Session["uId"] == null)
                return RedirectToAction("Login", "Account");

            int currentDir = int.Parse(dId);

            int uId = (int)Session["uId"];
            int rootDir = (int)Session["rootDir"];
            int recyclebin = (int)Session["recyclebin"];

            Session["currentDir"] = currentDir;

            User user = accountService.GetUser(uId);

            Dir dir = fileService.GetDir(currentDir);
            List<Dir> dirList = fileService.GetFirstChildrenDirs(currentDir);
            List<FileHeader> fileHeaderList = fileService.GetFirstChildrenFileHeaders(currentDir);
            List<Dir> pathList = fileService.GetPathList(dir);

            List<Dir> moveList = fileService.GetFirstChildrenDirs(rootDir);

            List<User> friendList = friendService.GetPermittedFriends(uId);
            List<UserGroup> groupList = groupService.GetOwnedAndJoinedGroups(uId);

            ViewBag.uId = uId;
            ViewBag.userName = user.getName();
            ViewBag.currentDir = currentDir;
            ViewBag.rootDir = rootDir;
            ViewBag.recyclebin = recyclebin;

            ViewBag.friendList = friendList;
            ViewBag.dirList = dirList;
            ViewBag.fileHeaderList = fileHeaderList;
            ViewBag.pathList = pathList;
            ViewBag.moveList = moveList;
            ViewBag.groupList = groupList;

            return View();
        }

        public ActionResult Recyclebin(String dId)
        {
            if (Session["uId"] == null)
                return RedirectToAction("Login", "Account");

            int currentDir = int.Parse(dId);

            int uId = (int)Session["uId"];
            int rootDir = (int)Session["rootDir"];
            int recyclebin = (int)Session["recyclebin"];

            Session["currentDir"] = currentDir;

            User user = accountService.GetUser(uId);

            Dir dir = fileService.GetDir(currentDir);
            List<Dir> dirList = fileService.GetFirstChildrenDirs(currentDir);
            List<FileHeader> fileHeaderList = fileService.GetFirstChildrenFileHeaders(currentDir);
            List<Dir> pathList = fileService.GetPathList(dir);

            List<Dir> moveList = fileService.GetFirstChildrenDirs(rootDir);

            List<User> friendList = friendService.GetPermittedFriends(uId);
            List<UserGroup> groupList = groupService.GetOwnedAndJoinedGroups(uId);

            ViewBag.uId = uId;
            ViewBag.userName = user.getName();
            ViewBag.currentDir = currentDir;
            ViewBag.rootDir = rootDir;
            ViewBag.recyclebin = recyclebin;

            ViewBag.friendList = friendList;
            ViewBag.dirList = dirList;
            ViewBag.fileHeaderList = fileHeaderList;
            ViewBag.pathList = pathList;
            ViewBag.moveList = moveList;
            ViewBag.groupList = groupList;

            return View();
        }
    }
}