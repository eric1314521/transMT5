// MT5CPP.cpp: 定义应用程序的入口点。
//
#define _AFXDLL
#define API_REQUEST (WM_USER+1)
#define API_DISCONNECT 0xffff
#include "windows.h"
#include "MT5APIManager.h"
#include "iostream"
#include <thread>
#include <vector>
#include <mutex>
#include <locale>
#include <codecvt>
#include <memory>
#include <queue>
#include <condition_variable>
#include <future>
#include <chrono>
#include <cstdint> 

#pragma warning(disable:4996)
#define GLOG_USE_GLOG_EXPORT
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>

#include "Utility.h"
#include "MT5Extension.h"

using namespace std;
using namespace google;



int main(int argc, char* argv[])
{

	LPCWSTR server = L"192.168.1.116:8019";
	LPCWSTR passwd = L"123456";
	LPCWSTR newpasswd = L"654321!";
	LPCWSTR newpasswd1 = L"eric1234!";
	UINT64 login = 88888888;
	UINT bytess = 0;

	MT5Extension mt5ext(server, login, passwd);


	if (!mt5ext.init()) {
		LOG(ERROR) << "init failed";
		return 0;
	};

	//get api manager 
	IMTManagerAPI* m_manager = mt5ext.getManagerAPI();

	//init logger
	Utility::initLogger(argv[0]);

#
	//create demo user
	DemoUser demoUser(3888,
		"test\\Test-123-234324",
		"Eric",
		"WU",
		"+123123",
		"123123!",
		1000,
		static_cast<UINT64>(IMTUser::EnUsersRights::USER_RIGHT_ENABLED) |
		static_cast<UINT64>(IMTUser::EnUsersRights::USER_RIGHT_PASSWORD));

	IMTUser* DemoUser = mt5ext.creatDemoUser(demoUser);

	if (!DemoUser) {
		LOG(ERROR) << "create demo user failed";
		return 0;
	}

	IMTUser* user = m_manager->UserCreate();

	MTAPIRES res = m_manager->UserRequest(123123, user);
	if (res != MT_RET_OK) {
		LOG(ERROR) << "user request failed";
		return 0;
	}
	else {
		LPCWSTR  wstr_group = user->Group();
		UINT64  rights = user->Rights();
		LOG(INFO) << "group name: " << Utility::wcharToString(wstr_group);
		LOG(INFO) << "rights: " << rights;

		user->Rights(IMTUser::EnUsersRights::USER_RIGHT_ENABLED | IMTUser::EnUsersRights::USER_RIGHT_PASSWORD);

		res = m_manager->UserUpdate(user);
		if (res != MT_RET_OK) {
			LOG(ERROR) << "user update failed";
			return 0;
		}
		else {
			LOG(INFO) << "user update success";
		}
	}

	res = m_manager->UserPasswordCheck(IMTUser::EnUsersPasswords::USER_PASS_MAIN, 123123, Utility::stringToWChar("+aQj3aVj"));
	if (res != MT_RET_OK) {
		LOG(ERROR) << "user password check failed";
		return 0;
	}
	else {
		LOG(INFO) << "password check success";
	}


	UINT64 countOrders = 5;
	MTAPIRES* results = new MTAPIRES[countOrders + 1];

	bool ret = mt5ext.orderAddBatch(countOrders, results);
	if (!ret) {
		LOG(ERROR) << "order add batch failed";
		return 0;
	}
	else {
		if (results) {
			// 遍历 results
			for (UINT64 i = 0; i < countOrders; ++i) {
				MTAPIRES result = results[i];
				if (result == MT_RET_OK) {
					std::cout << "Order " << i + 1 << " added successfully." << std::endl;
				}
				else {
					std::cout << "Order " << i + 1 << " failed with error: " << result << std::endl;
				}
			}
		}
		else {
			LOG(ERROR) << "order add batch results is null";
		}
	}


	//get history data before ready
	/*IMTConSymbol* symbolptr = m_manager->SymbolCreate();
	LPCWSTR name = Utility::stringToWChar("XAU");
	UINT64 account = 2888;
	INT64 from = 1704052409, to = 1729232874;
	std::vector<std::thread> threads;
	IMTDealArray* pa = m_manager->DealCreateArray();*/

	//get symbol info once
	//mt5ext.getSymbol();


	// get Histroy data once
	// mt5ext.processHistoryRequest(account, from, to, m_manager, pa, 0);

	// get Histroy data multi thread for 10 times
	//for (int i = 0; i < mt5ext.getRequestThreadsCount(); i++) {
	//	threads.emplace_back(std::bind(&MT5Extension::processHistoryRequest, &mt5ext, account, from, to, m_manager, pa, i));
	//}

	//Run 10 times sequentially
	/*for (int i = 0; i < mt5ext.getRequestThreadsCount(); i++) {
		IMTDealArray* pa = m_manager->DealCreateArray();
		mt5ext.processHistoryRequest(account, from, to, m_manager, pa, i);
	}*/

	//get Symbol data multi thread for 10 times
	//for (int i = 0; i < mt5ext.getRequestThreadsCount(); i++) {
	//	threads.emplace_back(std::bind(&MT5Extension::processSysget, &mt5ext, m_manager, name, symbolptr, i));
	//}

	//Run 10 times sequentially
	//for (int i = 0; i < mt5ext.getRequestThreadsCount(); i++) {
	//	mt5ext.processSysget(m_manager, name, symbolptr, i);
	//}

	//get null request multi thread for 10 times
	/*for (int i = 0; i < mt5ext.getRequestThreadsCount(); i++) {
		threads.emplace_back(std::bind(&MT5Extension::processSysget, &mt5ext, i));
	}*/

	//get position request Run 10 times sequentially
	/*for (int i = 0; i < mt5ext.getRequestThreadsCount(); i++) {
		mt5ext.processSPositionRequest(m_manager, account, i);
	}*/

	// waitting for all threads done
	/*for (auto& thread : threads) {
		thread.join();
	}*/

	return 0;
}

