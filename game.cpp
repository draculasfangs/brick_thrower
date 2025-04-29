#include "object.h"
#include "listenInput.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
	srand(time(NULL));
	unsigned short roomNumber = 0;
	Position pos(15,15); 
	Player player(R"(  /\_/\b  (:3 )b(_/   \_)b//\___/\\b||/   \||b  |_._|b   |||b  /_|_\)", pos); 
	//keeping track of gameState to render right string 
	unsigned short gameState = 0;
	unsigned int x = 120, y = 40;
	//calls roomChanged by 0 because we want load initial state of the room
	roomChanged(gameState,0, x , y, player);

}

