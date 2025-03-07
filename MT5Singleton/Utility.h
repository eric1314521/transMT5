#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include "windows.h"
#include <iostream>
#include <string>
#include <vector>
#include <cwchar> // For mbstowcs
#include <codecvt>

#pragma warning(disable:4996)
#define GLOG_USE_GLOG_EXPORT
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>

using namespace google;

class Utility
{
public:
	static wchar_t* stringToWChar(const std::string& str);
	static std::string wcharToString(const wchar_t* wstr);
	static std::wstring StringToWString(const std::string& str);
	static std::string to_byte_string(const std::wstring& input);
	static void initLogger(const char* logFilePath);
	static LPCWSTR StringToLPCWSTR(const std::string& str);
	static uint64_t GetMillisecondsSinceEpoch();
};
#endif // !UTILITY_H

