#include <windows.h>
#include <string>
#include <cassert>

std::wstring utf8_decode(const std::string& text)
{
	if (text[0] == 0)
		return std::wstring();

	int size_needed = ::MultiByteToWideChar(CP_UTF8, 0, text.c_str(),
		static_cast<int>(text.size()), NULL, 0);

	assert(size_needed > 0);
	const size_t cnt = static_cast<size_t>(size_needed);
	std::wstring wstrTo(cnt, 0);

	MultiByteToWideChar(CP_UTF8, 0, text.c_str(),
		static_cast<int>(text.size()), &wstrTo[0], size_needed);

	return wstrTo;
}

std::string utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0],
		(int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(),
		&strTo[0], size_needed, NULL, NULL);
	return strTo;
}


