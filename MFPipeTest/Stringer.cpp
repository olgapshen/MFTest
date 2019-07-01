#include "stdafx.h"
#include <atlstr.h>
#include <comdef.h> 
#include <string>
#include <clocale>
#include <locale>
#include <vector>

#include "Stringer.h"

using namespace std;
using namespace ATL;

string Stringer::getString(BSTR bstr) {
	_bstr_t bstrIntermediate(bstr);
	CString strFinal;
	strFinal.Format(_T("%s"), (LPCTSTR)bstrIntermediate);
	CT2CA pszConvertedAnsiString(strFinal);
	return string(pszConvertedAnsiString);
}

string Stringer::fromWstring(wstring ws) {
	const std::locale locale("");
	typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
	const converter_type& converter = std::use_facet<converter_type>(locale);
	std::vector<char> to(ws.length() * converter.max_length());
	std::mbstate_t state;
	const wchar_t* from_next;
	char* to_next;
	const converter_type::result result = converter.out(state, ws.data(), ws.data() + ws.length(), from_next, &to[0], &to[0] + to.size(), to_next);
	if (result == converter_type::ok || result == converter_type::noconv) {
		std::string s(&to[0], to_next);
		return s;
	}

	return "";
}