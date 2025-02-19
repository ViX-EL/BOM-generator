#pragma once

#include <string>

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string& text);
std::string utf8_encode(std::wstring_view wstr);
std::string utf8_encode(const std::wstring& wstr);