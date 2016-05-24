#pragma once
#include <string>
#include <windows.h>
#include <iostream>
//Poco lib header
#include <Poco/Path.h>

static struct config
{
	WCHAR* getNonConstWstr(const std::wstring& ws);
	static std::wstring makeDirectory(std::string fileDir);
};

static const std::wstring VS_Directory	= config::makeDirectory("shipmanwon\\color.vs");
static const std::wstring PS_Directory	= config::makeDirectory("shipmanwon\\color.ps");
static const std::wstring FxFile		= config::makeDirectory("shipmanwon\\texture\\stage.fx");
static const std::wstring StageFxFile	= config::makeDirectory("shipmanwon\\texture\\newstage.fx");
static const std::wstring SkyFxFile		= config::makeDirectory("shipmanwon\\texture\\skydome.fx");

static const std::wstring textureDefault	= config::makeDirectory("shipmanwon\\texture\\default.jpg");
static const std::wstring texturestage		= config::makeDirectory("shipmanwon\\texture\\profile.png");
static const std::wstring textureTitle		= config::makeDirectory("shipmanwon\\texture\\title.png");
static const std::wstring textureStage		= config::makeDirectory("shipmanwon\\texture\\stage.png");
static const std::wstring texturePress		= config::makeDirectory("shipmanwon\\texture\\spacebar.png");
static const std::wstring textureSky		= config::makeDirectory("shipmanwon\\texture\\grasscube1024.dds");

static const std::wstring resultTextureDefault = config::makeDirectory("shipmanwon\\texture\\resultP");

static const std::wstring soundCollision = config::makeDirectory("shipmanwon\\sound\\collision.wav");
static const std::wstring soundStart = config::makeDirectory("shipmanwon\\sound\\start.wav");
static const std::wstring soundResult = config::makeDirectory("shipmanwon\\sound\\result.wav");
static const std::wstring soundGame = config::makeDirectory("shipmanwon\\sound\\game.wav");