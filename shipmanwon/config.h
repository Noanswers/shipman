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
static const std::wstring FxFile		= config::makeDirectory("texture\\stage.fx");
static const std::wstring StageFxFile	= config::makeDirectory("texture\\newstage.fx");
static const std::wstring SkyFxFile		= config::makeDirectory("texture\\skydome.fx");

static const std::wstring textureDefault	= config::makeDirectory("texture\\default.jpg");
static const std::wstring texturestage		= config::makeDirectory("texture\\profile.png");
static const std::wstring textureTitle		= config::makeDirectory("texture\\title.png");
static const std::wstring textureStage		= config::makeDirectory("texture\\stage.png");
static const std::wstring texturePress		= config::makeDirectory("texture\\spacebar.png");
static const std::wstring textureSky		= config::makeDirectory("texture\\grasscube1024.dds");

static const std::wstring resultTextureDefault = config::makeDirectory("texture\\resultP");
static const std::wstring playerTexture_1 = config::makeDirectory("texture\\p1.png");
static const std::wstring playerTexture_2 = config::makeDirectory("texture\\p2.png");

static const std::wstring soundCollision = config::makeDirectory("sound\\collision.wav");
static const std::wstring soundStart = config::makeDirectory("sound\\start.wav");
static const std::wstring soundResult = config::makeDirectory("sound\\result.wav");
static const std::wstring soundGame = config::makeDirectory("sound\\game.wav");