#include <string>

#pragma once
class Stringer
{
public:
	static std::string getString(BSTR bstr);
	static std::string fromWstring(std::wstring str);
};

