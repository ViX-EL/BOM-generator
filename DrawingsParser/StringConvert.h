#pragma once

#ifndef StringConvert_h__
#define StringConvert_h__

#include <string>

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string& text);
std::string utf8_encode(std::wstring_view wstr);
std::string utf8_encode(const std::wstring& wstr);
std::string narrow(const std::wstring& wide, const std::locale& loc);
std::wstring widen(const std::string& narrow, const std::locale& loc);

#endif // StringConvert_h__