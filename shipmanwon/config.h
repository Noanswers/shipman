#pragma once
#include <string>
#include <windows.h>
#include <iostream>
//////////////////////////////////////////////////////////////////////////
//Poco lib header
#include <Poco/Path.h>
//////////////////////////////////////////////////////////////////////////

static struct config
{
	WCHAR* getNonConstWstr(const std::wstring& ws);
	static std::wstring makeDirectory(std::string fileDir);

	//const std::wstring VS_Directory;
	//const std::wstring PS_Directory;
	//const std::wstring FxFile;
	//const std::wstring textureDefault;
};

static const std::wstring VS_Directory = config::makeDirectory("shipmanwon\\color.vs");
static const std::wstring PS_Directory = config::makeDirectory("shipmanwon\\color.ps");
static const std::wstring FxFile = config::makeDirectory("shipmanwon\\texture\\stage.fx");
static const std::wstring textureDefault = config::makeDirectory("shipmanwon\\texture\\default.jpg");