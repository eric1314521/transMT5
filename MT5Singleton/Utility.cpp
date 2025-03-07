#include "Utility.h"

wchar_t* Utility::stringToWChar(const std::string& str) {
	// 计算需要的宽字符数组大小
	size_t len = 0;
	mbstowcs_s(&len, nullptr, 0, str.c_str(), _TRUNCATE);

	if (len == static_cast<size_t>(-1)) {
		return nullptr; // 转换失败
	}

	// 分配宽字符数组
	wchar_t* wstr = new wchar_t[len + 1];

	// 实际转换
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wstr, len + 1, str.c_str(), _TRUNCATE);

	return wstr; // 返回指向宽字符数组的指针
}

std::string Utility::wcharToString(const wchar_t* wstr) {
	// 计算需要的字符数组大小
	size_t len = 0;
	wcstombs_s(&len, nullptr, 0, wstr, _TRUNCATE);
	if (len == static_cast<size_t>(-1)) {
		return ""; // 转换失败
	}

	// 分配字符数组
	std::string str(len, '\0');
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, &str[0], str.size(), wstr, _TRUNCATE);

	return str; // 返回转换后的字符串
}

std::wstring Utility::StringToWString(const std::string& str) {
	// 获取转换所需的缓冲区大小
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstr[0], size_needed);
	return wstr;
}

std::string Utility::to_byte_string(const std::wstring& input) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(input);
}

void Utility::initLogger(const char* logFilePath) {
	google::InitGoogleLogging(logFilePath);


	// create logpath
	std::string str_des;
	str_des.append("mkdir log");
	system(str_des.c_str());

	FLAGS_alsologtostderr = false;
	FLAGS_logtostderr = 0;

	FLAGS_stop_logging_if_full_disk = true;
	FLAGS_stderrthreshold = 0;

	google::SetLogDestination(google::GLOG_INFO, "log\\INFO_");

	// stop glog
	google::ShutdownGoogleLogging();
}

LPCWSTR Utility::StringToLPCWSTR(const std::string& str) {
	// 将 std::string 转换为 std::wstring
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstr[0], size_needed);

	// 返回 LPCWSTR
	return wstr.c_str();
}

uint64_t Utility::GetMillisecondsSinceEpoch() {
	// 获取当前系统时间
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	// 将 SYSTEMTIME 转换为 FILETIME
	FILETIME fileTime;
	SystemTimeToFileTime(&systemTime, &fileTime);

	// 将 FILETIME 转换为 64 位整数
	uint64_t time = *reinterpret_cast<uint64_t*>(&fileTime);

	// FILETIME 是自 1601 年 1 月 1 日以来的 100 纳秒间隔
	// 计算自 1970 年 1 月 1 日以来的毫秒数

	const uint64_t FILETIME_TO_EPOCH_DIFF = 116444736000000000ULL; // 1970-01-01 00:00:00 UTC - 1601-01-01 00:00:00 UTC
	const uint64_t HUNDRED_NANOSECONDS_PER_MILLISECOND = 10000ULL;

	// 计算自 1970 年 1 月 1 日以来的毫秒数
	return (time - FILETIME_TO_EPOCH_DIFF) / HUNDRED_NANOSECONDS_PER_MILLISECOND;
}