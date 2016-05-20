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
static const std::wstring SkyFxFile = config::makeDirectory("shipmanwon\\texture\\skydome.fx");


static const std::wstring textureDefault = config::makeDirectory("shipmanwon\\texture\\default.jpg");
static const std::wstring texturestage = config::makeDirectory("shipmanwon\\texture\\profile.png");
static const std::wstring textureTitle = config::makeDirectory("shipmanwon\\texture\\title.png");
static const std::wstring textureStage = config::makeDirectory("shipmanwon\\texture\\stage.png");
static const std::wstring texturePress = config::makeDirectory("shipmanwon\\texture\\spacebar.png");

static const std::wstring textureSky = config::makeDirectory("shipmanwon\\texture\\grasscube1024.dds");