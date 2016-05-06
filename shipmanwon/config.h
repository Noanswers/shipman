#pragma once
#include <string>
#include <windows.h>
#include <iostream>
//////////////////////////////////////////////////////////////////////////
//Poco lib header
#include <Poco/Path.h>
//////////////////////////////////////////////////////////////////////////

std::wstring makeVS_Directory(std::string fileDir)
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

const std::wstring VS_Directory = makeVS_Directory("x64\\Release\\color.vs");
const std::wstring PS_Directory = makeVS_Directory("x64\\Release\\color.ps");