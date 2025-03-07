#pragma once
#ifndef MT5EXTENSION_H_
#define MT5EXTENSION_H_

#include "windows.h"
#include "MT5APIManager.h"
#include "iostream"
#include "mutex"
#include "Utility.h"
#include "define.h"

#pragma warning(disable:4996)
#define GLOG_USE_GLOG_EXPORT
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>

class MT5Extension
{
public:
	MT5Extension(void);
	MT5Extension(LPCWSTR server, UINT64 login, LPCWSTR password);
	~MT5Extension(void);

	bool init();

	void processHistoryRequest(UINT64 account,
		INT64 from,
		INT64 to,
		IMTManagerAPI* m_manager,
		IMTDealArray* pa,
		int index);

	void processSysget(IMTManagerAPI* m_manager,
		LPCWSTR name,
		IMTConSymbol* symbol,
		int index);

	void processSPositionRequest(IMTManagerAPI* m_manager,
		UINT64 account,
		int index);

	void processRequestNull(int index);

	IMTUser* creatDemoUser(const DemoUser& demoUser);

	IMTManagerAPI* getManagerAPI() { return m_manager; };

	const int getRequestThreadsCount() { return totalRequests; };

	void getSymbol();

	bool orderAddBatch(UINT64 countOrders, MTAPIRES* results);

private:
	std::mutex mtx;
	std::chrono::duration<double, std::milli> totolTime;
	std::chrono::duration<double, std::milli> beforetotolTime;
	const int totalRequests;
	CMTManagerAPIFactory m_factory;     // Manager API factory
	IMTManagerAPI* m_manager = NULL;
	LPCWSTR server;
	UINT64 login;
	LPCWSTR password;
};
#endif // MT5EXTENSION_H_