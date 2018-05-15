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
using System.Text;

namespace CloudStorage.Controllers
{
    public class FriendController : Controller
    {
        private AccountService accountService = new AccountService();

        private FileService fileService = new FileService();

        private FriendService friendService = new FriendService();


        // GET: Friend
        public ActionResult List()
        {
            if (Session["uId"] == null)
                return RedirectToAction("Login", "Account");

            int uId = (int)Session["uId"];
            int rootDir = (int)Session["rootDir"];
            int recyclebin = (int)Session["recyclebin"];

            List<User> friendList = friendService.GetPermittedFriends(uId);
            List<User> unpermittedFriendList = friendService.GetFriendRequest(uId);
            
            ViewBag.uId = uId;
            ViewBag.rootDir = rootDir;
            ViewBag.recyclebin = recyclebin;
            ViewBag.friendList = friendList;
            ViewBag.unpermittedFriendList = unpermittedFriendList;

            return View();
        }

        public ActionResult ShareFiles(FormCollection form)
        {
            if (Session["uId"] == null)
                return RedirectToAction("Login", "Account");
            if (form["friendId"] == null && TempData["friendId"]==null)
                return RedirectToAction("List", "Friend");

            int uId = (int)Session["uId"];
            int rootDir = (int)Session["rootDir"];
            int recyclebin = (int)Session["recyclebin"];
            int friendId = (form["friendId"]!=null)?int.Parse(form["friendId"]):(int)(TempData["friendId"]);

            List<FriendFileShareItem> mySharedList = fileService.GetFriendSharedFiles(uId, friendId);
            List<FriendFileShareItem> friendSharedList = fileService.GetFriendSharedFiles(friendId, uId);

            ViewBag.uId = uId;
            ViewBag.rootDir = rootDir;
            ViewBag.recyclebin = recyclebin;
            ViewBag.friendId = friendId;
            ViewBag.mySharedList = mySharedList;
            ViewBag.friendSharedList = friendSharedList;

            return View();
        }

        [HttpPost]
        public ActionResult Agree(FormCollection form)
        {
            int uId = (int)Session["uId"];
            friendService.PermitFriendRequest(uId, int.Parse(form["friendId"]));

            return RedirectToAction("List", "Friend");
        }

        [HttpPost]
        public ActionResult DeleteFriend(FormCollection form)
        {
            int uId = (int)Session["uId"];
            friendService.DeleteFriend(uId, int.Parse(form["friendId"]));

            return RedirectToAction("List", "Friend");
        }

        [HttpPost]
        public String Search(FormCollection form)
        {
            int uId = (int)Session["uId"];
            int friendId = int.Parse(form["friendId"]);

            if (uId == friendId)
            {
                return "搜索的为当前账号";
            }

            
            User friend = accountService.GetUser(friendId);
            if (friend == null)
            {
                return "无此账号";
            }
            StringBuilder result = new StringBuilder();
            result.Append("用户ID：");
            result.Append(friendId);
            result.Append("   用户名：");
            result.Append(friend.getName());
            result.Append("<button class='btn btn-default' id='friend_request_btn' type='submit' friend_id='");
            result.Append(friendId);
            result.Append("'>添加好友</button>");

            return result.ToString();
        }

        [HttpPost]
        public ActionResult SendRequest(FormCollection form)
        {
            int uId = (int)Session["uId"];
            friendService.SendFriendRequest(uId, int.Parse(form["friendId"]));

            return RedirectToAction("List", "Friend");
        }

    }
}