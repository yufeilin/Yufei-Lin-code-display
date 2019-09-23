#ifndef _GAMECONTEXT
#define _GAMECONTEXT

#include "YufeiBasic.h"
#include "Project28Basic.h"

class LibraryManager;

class GameContext
{
public:
	GameContext();
	~GameContext();
	inline LibraryManager GetLibraryManager() { return MainLibraryManager; }
	void debug() { cout << "233" << endl; }

private:
	LibraryManager MainLibraryManager;
};

GameContext::GameContext()
{

}

GameContext::~GameContext()
{

}

#endif
