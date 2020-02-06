#pragma once

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

class DLL_API IDLL
{
public:
	virtual ~IDLL() = default;
	virtual void DoStuff() = 0;
};

class DLL : public IDLL
{
public:
	DLL() = default;
	virtual void DoStuff() override;

private:
	int _myInt{0};
};

extern "C" DLL_API IDLL* CreateDLL();