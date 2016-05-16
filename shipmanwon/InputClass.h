#pragma once
#include <vector>
#include "Singleton.h"

class CInputClass : public CSingleton<CInputClass>
{
public:
	CInputClass()					= default;
	~CInputClass()					= default;
	CInputClass(const CInputClass&) = delete;

	void initialize();

	void keyDown(unsigned int);
	void keyUp(unsigned int);

	bool isKeyDown(unsigned int);

	void downkeySet(unsigned int set)
	{downkey.push_back(set);}

	void downkeyUnset(unsigned int unset);
	
	
	

private:
	bool Keys[256];
	std::vector<unsigned int> downkey;


};

