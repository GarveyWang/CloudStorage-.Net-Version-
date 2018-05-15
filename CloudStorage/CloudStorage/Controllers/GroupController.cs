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
    public class GroupController : Controller
    {
        private GroupService groupService = new GroupService();

        private FileService fileService = new FileService();

        // GET: Group
        public ActionResult List()
        {
            if (Session["uId"] == null)
                return RedirectToAction("Login", "Account");

            int uId = (int)Session["uId"];
            int rootDir = (int)Session["rootDir"];
            int recyclebin = (int)Session["recyclebin"];

            List<UserGroup> ownedGroupList = groupService.getOwnedGroups(uId);
            List<UserGroup> joinedGroupList = groupService.GetJoinedGroups(uId);

            ViewBag.uId = uId;
            ViewBag.rootDir = rootDir;
            ViewBag.recyclebin = recyclebin;
            ViewBag.ownedGroupList = ownedGroupList;
            ViewBag.joinedGroupList = joinedGroupList;

            return View();
        }

        public ActionResult Detail(FormCollection form)
        {
            if (Session["uId"] == null)
                return RedirectToAction("Login", "Account");
            if (form["gId"] == null && TempData["gId"] == null) 
                return RedirectToAction("List", "Group");


            int uId = (int)Session["uId"];
            int rootDir = (int)Session["rootDir"];
            int recyclebin = (int)Session["recyclebin"];

            int gId = (form["gId"] != null)?int.Parse(form["gId"]):(int)TempData["gId"];

            User owner = groupService.GetGroupOwner(gId);//群主
            List<User> memberList = groupService.GetGroupMembers(gId);//群成员List
            List<GroupFileShareItem> groupFileShareList = fileService.GetGroupSharedFiles(gId);//群组共享文件List
            List<User> unpermittedMemberList = groupService.GetUnpermittedMembers(gId);//入群申请


            ViewBag.uId = uId;
            ViewBag.gId = gId;
            ViewBag.owner = owner;
            ViewBag.rootDir = rootDir;
            ViewBag.recyclebin = recyclebin;
            ViewBag.groupFileShareList = groupFileShareList;
            ViewBag.memberList = memberList;
            ViewBag.unpermittedMemberList = unpermittedMemberList;

            return View();
        }

        [HttpPost]
        public String Search(int gId)
        {
            int uId = (int)Session["uId"];

            UserGroup userGroup = groupService.GetGroup(gId);
            if (userGroup == null)
            {
                return "无此群组";
            }
            
            StringBuilder result = new StringBuilder();

            result.Append("群组ID：");
            result.Append(gId);
            result.Append(" 群组名：");
            result.Append(userGroup.getName());
            if (userGroup.getBuilderId() != uId)
            {
                bool isMember = false;
                List<User> members = groupService.GetGroupMembers(gId);
                foreach(User user in members)
                {
                    if (user.getUId() == uId)
                    {
                        isMember = true;
                        break;
                    }
                }
                if (!isMember)
                {
                    result.Append("<button class='btn btn-default' id='group_request_btn' type='submit' g_id='");
                    result.Append(gId);
                    result.Append("'>申请加入</button>");
                }
            }
            
            return result.ToString();
        }

        [HttpPost]
        public ActionResult AddGroup(FormCollection form)
        {
            if (form["name"] != null)
            {
                UserGroup group = new UserGroup();
                group.setName(form["name"]);
                group.setType(form["type"]);
                group.setBuilderId((int)Session["uId"]);
                groupService.CreateGroup(group);
            }
            return RedirectToAction("List", "Group");
        }

        [HttpPost]
        public ActionResult DeleteGroup(FormCollection form)
        {
            int uId = (int)Session["uId"];
            groupService.DeleteGroup(int.Parse(form["gId"]), uId);
            return RedirectToAction("List", "Group");
        }

        [HttpPost]
        public ActionResult QuitGroup(FormCollection form)
        {
            int uId = (int)Session["uId"];
            groupService.DeleteGroupMember(int.Parse(form["gId"]), uId);
            return RedirectToAction("List", "Group");
        }

        [HttpPost]
        public ActionResult SendRequest(FormCollection form)
        {
            int uId = (int)Session["uId"];
            groupService.SendJoinRequest(int.Parse(form["gId"]), uId);
            return RedirectToAction("List", "Group");
        }

        [HttpPost]
        public ActionResult Permit(FormCollection form)
        {
            TempData["gId"] = int.Parse(form["gId"]);
            groupService.PermitMemberRequest(int.Parse(form["gId"]),int.Parse(form["memberId"]));
            return RedirectToAction("Detail", "Group");
        }

        [HttpPost]
        public ActionResult Refuse(FormCollection form)
        {
            groupService.RefuseGroupRequest(int.Parse(form["gId"]), int.Parse(form["memberId"]));
            return RedirectToAction("List", "Group");
        }

        [HttpPost]
        public ActionResult DeleteMember(FormCollection form)
        {
            int memberId = int.Parse(form["memberId"]);
            int uId = (int)Session["uId"];
            groupService.DeleteGroupMember(int.Parse(form["gId"]), memberId);
            if (uId == memberId)
                return RedirectToAction("List", "Group");
            else
                return RedirectToAction("Detail", "Group");
        }

    }
}