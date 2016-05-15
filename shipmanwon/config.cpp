#include "stdafx.h"
#include "config.h"

WCHAR* config::getNonConstWstr(const std::wstring& ws)
{
	//return non-constant copy of s.c_str()
	static std::vector<wchar_t> var;
	var.assign(ws.begin(), ws.end());
	var.push_back('\0');
	return &var[0];
}

std::wstring config::makeDirectory(std::string fileDir)
{
	//	Get .exe file name
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	//	Make VSPath
	auto currentPath = Poco::Path(buffer).makeParent();
	auto VSPath = currentPath.popDirectory();
	VSPath = VSPath.popDirectory().append(fileDir);

	//	return type set
	std::string temp = VSPath.toString();
	std::wstring temp2(temp.begin(), temp.end());

	return temp2;
}

//const std::wstring VS_Directory = makeDirectory("shipmanwon\\color.vs");
//
//const std::wstring PS_Directory = makeDirectory("shipmanwon\\color.ps");
//
//const std::wstring FxFile = makeDirectory("shipmanwon\\texture\\stage.fx");
//
//const std::wstring textureDefault = makeDirectory("shipmanwon\\texture\\default.jpg");
