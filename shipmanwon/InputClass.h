#pragma once

class CInputClass
{
public:
	CInputClass()					= default;
	~CInputClass()					= default;
	CInputClass(const CInputClass&) = delete;

	void initialize();

	void keyDown(unsigned int);
	void keyUp(unsigned int);

	bool isKeyDown(unsigned int);

private:
	bool Keys[256];
};

