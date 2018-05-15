using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

using CSAccount;
using CloudStorage.Services;

namespace CloudStorage.Controllers
{
    public class AccountController : Controller
    {
        private AccountService accountService = new AccountService();

        // GET: Account
        public ActionResult Login()
        {
            return View();
        }

        [HttpPost]
        public ActionResult Login(FormCollection form)
        {
            User user = new User();

            if (form["uId"].Equals(""))
            {
                user.setUId(-1);
            }
            else
            {
                user.setUId(int.Parse(form["uId"]));
            }

            user.setPwd(form["pwd"]);
            bool loginState = accountService.Validate(user);
            //登录状态不成功，则返回登录界面重新登录
            if (!loginState)
            {
                TempData["msg"] = "登录失败";
                return View();
            }

            //登录成功，进入首页
            int uId = user.getUId();
            User userFromDB = accountService.GetUser(uId);
            int rootDir = userFromDB.getRootDir();

            Session["uId"] = uId;
            Session["uName"] = userFromDB.getName();
            Session["currentDir"] = userFromDB.getRootDir();
            Session["rootDir"] = userFromDB.getRootDir();
            Session["recyclebin"] = userFromDB.getRecyclebin();

            return RedirectToAction("List","Home", new { dId = rootDir });
        }


        public ActionResult Register()
        {
            return View();
        }

        [HttpPost]
        public ActionResult Register(FormCollection form)
        {
            User user = new User();
            user.setUId(int.Parse(form["uId"]));
            user.setPwd(form["pwd"]);
            user.setName(form["name"]);
            user.setType(form["type"]);

            bool registerState = accountService.Register(user);
            
            //注册成功，返回登录界面
            if (registerState)
            {
                TempData["msg"] = "注册成功";
                return Redirect("/Account/Login");
            }
            //注册不成功，重新返回注册界面
            ViewBag.msg = "注册失败";
            ViewBag.uId = form["uId"];
            ViewBag.pwd = form["pwd"];
            ViewBag.name = form["name"];
            return View();
        }
    }
}