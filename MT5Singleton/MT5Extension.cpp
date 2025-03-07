#include "MT5Extension.h"

MT5Extension::MT5Extension(void) :totalRequests(10), server(nullptr), login(0), password(nullptr) {
	totolTime = std::chrono::duration<double, std::milli>::zero();
	beforetotolTime = std::chrono::duration<double, std::milli>::zero();
}

MT5Extension::MT5Extension(LPCWSTR server, UINT64 login, LPCWSTR password)
	:totalRequests(10), server(server), login(login), password(password) {
	totolTime = std::chrono::duration<double, std::milli>::zero();
	beforetotolTime = std::chrono::duration<double, std::milli>::zero();
}

MT5Extension::~MT5Extension(void) {
	if (m_manager)
	{
		m_manager->Release();
		m_manager = NULL;
	}

	m_factory.Shutdown();
}

bool MT5Extension::init() {

	MTAPIRES res = MT_RET_OK_NONE;
	UINT     version = 0;

	//--- initialize factory
	if ((res = m_factory.Initialize(NULL)) != MT_RET_OK)
	{
		wprintf_s(L"ManagerExtension: loading manager API  failed [%u]\n", res);
		return(false);
	}
	//--- check Manager API version
	if ((res = m_factory.Version(version)) != MT_RET_OK)
	{
		wprintf_s(L"ManagerExtension: getting version failed [%u]\n", res);
		return(false);
	}
	//--- show version  
	wprintf(L"ManagerExtension:: Manager API version %u has been loaded\n", version);
	if (version != MTManagerAPIVersion)
	{
		wprintf_s(L"ManagerExtension: wrong Manager API version, version %u required\n", MTManagerAPIVersion);
		return(false);
	}
	//--- create manager interface
	if ((res = m_factory.CreateManager(MTManagerAPIVersion, &m_manager)) != MT_RET_OK)
	{
		wprintf_s(L"ManagerExtension: creating manager interface failed [%u]\n", res);
		return(false);
	}

	if (!m_manager)
	{
		wprintf_s(L"ManagerExtension: manager interface is NULL\n");
		return(false);
	}


	//--- connect to server

	if ((res = m_manager->Connect(server, login, password, L"", IMTManagerAPI::PUMP_MODE_ORDERS, 30000)) != MT_RET_OK)
	{
		wprintf_s(L"manager Connection failed [%u] [%I64u]\n", res, login);
		return(false);
	}

	wprintf_s(L"Connection successful [%I64u]\n", login);

	return true;
}

void MT5Extension::processHistoryRequest(UINT64 account, INT64 from, INT64 to, IMTManagerAPI* m_manager, IMTDealArray* pa, int index) {
	// begin time
	auto start = std::chrono::high_resolution_clock::now();

	MTAPIRES err = m_manager->DealRequest(account, from + 28800, to + 28800, pa);


	// 保护输出
	//int err = 0;
	std::lock_guard<std::mutex> lock(mtx);
	if (err == MT_RET_OK) {
		wprintf_s(L"Thread[%d]: manager call DealRequest success [%u]\n", index, err);
	}
	else {
		wprintf_s(L"Thread[%d]: Fail to call DealRequest [%u]\n", index, err);
	}
	// end time
	auto end = std::chrono::high_resolution_clock::now();
	// duration
	std::chrono::duration<double, std::milli> duration = end - start;
	totolTime += duration;
	LOG(INFO) << "Thread[" << index << "] MT5 [DealCreateArray & DealRequest] ExeTime: " << duration.count() << "ms";
	LOG(INFO) << "Thread[" << index << "] Time Taken for " << totalRequests << " Concurrent Requests ExeTime: " << totolTime.count() / totalRequests << "ms";
}

void MT5Extension::processSysget(IMTManagerAPI* m_manager, LPCWSTR name, IMTConSymbol* symbol, int index) {
	// begin time
	auto start = std::chrono::high_resolution_clock::now();

	// 假设 m_manager 是全局或传递的管理对象
	MTAPIRES err = m_manager->SymbolGet(name, symbol);

	//std::this_thread::sleep_for(std::chrono::milliseconds(200));


	// 保护输出
	//int err = 0;
	std::lock_guard<std::mutex> lock(mtx);
	if (err == MT_RET_OK) {
		wprintf_s(L"Thread[%d]: manager call DealRequest success [%u]\n", index, err);
	}
	else {
		wprintf_s(L"Thread[%d]: Fail to call DealRequest [%u]\n", index, err);
	}
	// end time
	auto end = std::chrono::high_resolution_clock::now();
	// duration
	std::chrono::duration<double, std::milli> duration = end - start;
	totolTime += duration;
	LOG(INFO) << "Thread[" << index << "] MT5 [SymbolGet] ExeTime: " << duration.count() << "ms";
	LOG(INFO) << "Thread[" << index << "] Time Taken for " << totalRequests << " Concurrent Requests ExeTime: " << totolTime.count() / totalRequests << "ms";
	//LOG(INFO) << "Thread[" << index << "] MT5 [DealCreateArray & DealRequest] ExeTime: ";
}

void MT5Extension::processSPositionRequest(IMTManagerAPI* m_manager, UINT64 account, int index) {
	// begin time
	auto start = std::chrono::high_resolution_clock::now();

	// 假设 m_manager 是全局或传递的管理对象
	IMTPositionArray* pa = m_manager->PositionCreateArray();
	MTAPIRES err = m_manager->PositionRequest(account, pa);

	//std::this_thread::sleep_for(std::chrono::milliseconds(200));

	if (err == MT_RET_OK) {
		wprintf_s(L"Thread[%d]: manager call DealRequest success [%u]\n", index, err);
	}
	else {
		wprintf_s(L"Thread[%d]: Fail to call DealRequest [%u]\n", index, err);
	}
	// end time
	auto end = std::chrono::high_resolution_clock::now();
	// duration
	std::chrono::duration<double, std::milli> duration = end - start;
	LOG(INFO) << "Thread[" << index << "] MT5 [PositionRequest] ExeTime: " << duration.count() << "ms";
	LOG(INFO) << "Thread[" << index << "] Time Taken for " << totalRequests << " Concurrent Requests ExeTime: " << totolTime.count() / totalRequests << "ms";
}

void MT5Extension::processRequestNull(int index) {
	// begin time
	auto start = std::chrono::high_resolution_clock::now();

	std::this_thread::sleep_for(std::chrono::milliseconds(200));


	// 保护输出
	//int err = 0;
	std::lock_guard<std::mutex> lock(mtx);
	// end time
	auto end = std::chrono::high_resolution_clock::now();
	// duration
	std::chrono::duration<double, std::milli> duration = end - start;
	LOG(INFO) << "Thread[" << index << "] NULL [delay 200ms] ExeTime: " << duration.count() << "ms";
	LOG(INFO) << "Thread[" << index << "] Time Taken for " << totalRequests << " Concurrent Requests ExeTime: " << totolTime.count() / totalRequests << "ms";

}

IMTUser* MT5Extension::creatDemoUser(const DemoUser& demoUser) {
	IMTUser* user = m_manager->UserCreate();
	user->Login(demoUser.login_user);
	user->Group(Utility::stringToWChar(demoUser.group_name));
	user->Rights(demoUser.rights);
	user->FirstName(Utility::stringToWChar(demoUser.first_name));
	user->LastName(Utility::stringToWChar(demoUser.last_name));
	user->Leverage(demoUser.leverage);

	MTAPIRES err = m_manager->UserAdd(user, demoUser.master_pass, demoUser.investor_pass);

	if (err == MT_RET_OK) {
		std::cout << "UserAdd success" << std::endl;
	}
	else {
		std::cout << "UserAdd failed: " << err << std::endl;
		return nullptr;
	}
	return user;
}

void MT5Extension::getSymbol() {
	UINT count = m_manager->SymbolTotal();
	std::cout << "SymbolTotal: " << count << std::endl;

	for (int i = 0; i < count; i++) {
		IMTConSymbol* symbolptr = m_manager->SymbolCreate();
		m_manager->SymbolNext(i, symbolptr);
		std::cout << "SymbolName: " << Utility::to_byte_string(symbolptr->Symbol()).c_str() << std::endl;
	}
}

bool MT5Extension::orderAddBatch(UINT64 countOrders, MTAPIRES* results)
{
	IMTOrderArray* array = m_manager->OrderCreateArray();

	for (int i = 0; i < countOrders; ++i) {
		IMTOrder* order = m_manager->OrderCreate();
		order->Login(login);        //设置订单所属客户的登录名。
		order->Symbol(L"XAU");      //设置订单所属品种的名称。
		order->Type(IMTOrder::EnOrderType::OP_BUY); //设置订单类型
		order->Digits(2);           //设置订单价格中的小数位数。
		order->DigitsCurrency(2);   //设置已下单的客户存款货币的小数位数。
		order->ContractSize(100);   //设置下单所针对的交易品种的合约大小
		order->VolumeInitial(100);  //设置每个订单的初始交易量
		order->VolumeCurrent(1000);    //设置订单的当前未成交交易量。
		order->PriceOrder(2948.78); //下订单时的价格。
		//order->State();             //设置订单的状态。
		order->TimeSetupMsc(Utility::GetMillisecondsSinceEpoch() + 28800);      //设置下单时间（以毫秒为单位）。

		order->Comment(Utility::StringToLPCWSTR("test for batch order " + std::to_string(i + 1))); // 每个订单的注释
		//order->PriceSL(0);
		//order->PriceTP(0);
		order->StateSet(0);

		array->Add(order); // 添加订单到数组
		Sleep(1000);
	}

	std::cout << "orders total: " << array->Total() << std::endl;

	for (int i = 0; i < array->Total(); ++i) {
		IMTOrder* order = array->Next(i);
		std::cout << "order " << i << " login: " << order->Login() << std::endl;
		std::cout << "order " << i << " symbol: " << Utility::to_byte_string(order->Symbol()).c_str() << std::endl;
		std::cout << "order " << i << " type: " << order->Type() << std::endl;
		std::cout << "order " << i << " digits: " << order->Digits() << std::endl;
		std::cout << "order " << i << " digitsCurrency: " << order->DigitsCurrency() << std::endl;
		std::cout << "order " << i << " contractSize: " << order->ContractSize() << std::endl;
		std::cout << "order " << i << " volumeInitial: " << order->VolumeInitial() << std::endl;
		std::cout << "order " << i << " volumeCurrent: " << order->VolumeCurrent() << std::endl;
		std::cout << "order " << i << " priceOrder: " << order->PriceOrder() << std::endl;
		std::cout << "order " << i << " timeSetupMsc: " << order->TimeSetupMsc() << std::endl;
		std::cout << "order " << i << " comment: " << Utility::to_byte_string(order->Comment()).c_str() << std::endl;
		std::cout << "order " << i << " priceSL: " << order->PriceSL() << std::endl;
		std::cout << "order " << i << " priceTP: " << order->PriceTP() << std::endl;
	}

	MTAPIRES err = m_manager->OrderAddBatch(array, results);

	if (err == MT_RET_OK) {
		std::cout << "OrderAddArray success" << std::endl;
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
		std::cout << "OrderAddArray failed: " << err << std::endl;
		return false;
	}
	return true;
}