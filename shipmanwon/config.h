#pragma once
#include <string>
#include <windows.h>
#include <iostream>
//////////////////////////////////////////////////////////////////////////
//Poco lib header
#include <Poco/Path.h>
//////////////////////////////////////////////////////////////////////////

std::wstring makeVS_Directory()
{
//	Get .exe file name
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
//	Make VSPath
	auto currentPath = Poco::Path(buffer).makeParent();
	auto VSPath = currentPath.popDirectory();
	VSPath = VSPath.popDirectory().append("Release\\color.vs");

//	return type set
	std::string temp = VSPath.toString();
	std::wstring temp2(temp.begin(), temp.end());

	return temp2;
}


const std::wstring VS_Directory = makeVS_Directory();
const std::wstring PS_Directory = L"C:/Users/Çö¿ì/Documents/Visual Studio 2015/Projects/shipman/x64/Release/color.ps";