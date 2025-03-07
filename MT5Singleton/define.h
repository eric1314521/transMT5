#pragma once
#ifndef _DEFINE_H_
#define _DEFINE_H_

#include "windows.h"
#include <string>
#include "Utility.h"

// 用户结构体定义
struct DemoUser {
	UINT64 login_user;      // 用户登录ID
	std::string group_name; // 用户组名
	std::string first_name; // 用户的名
	std::string last_name;  // 用户的姓
	LPCWSTR master_pass;    // 主密码
	LPCWSTR investor_pass;  // 投资者密码
	uint32_t leverage;      // 杠杆
	UINT64 rights;          // 用户权限

	// 构造函数
	DemoUser(UINT64 login,
		const std::string& group,
		const std::string& first,
		const std::string& last,
		const std::string& master,
		const std::string& investor,
		uint32_t lev,
		UINT64 user_rights)
		: login_user(login),
		group_name(group),
		first_name(first),
		last_name(last),
		master_pass(Utility::stringToWChar(master)),
		investor_pass(Utility::stringToWChar(investor)),
		leverage(lev),
		rights(user_rights) {}
};

#endif // _DEFINE_H_