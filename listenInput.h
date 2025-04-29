#include <iostream>
#include "object.h"
#include "saveSystem.h"
#include "render_screen.h"
#include <cstdlib>

using namespace std;

void buttonIndexChange(short& buttonIndex,char input,bool &isPressed, unsigned short &turn);
void listenInput(unsigned short &i /*i is gamestate*/ , Player &player, unsigned short enemyNumber, Position* enterance, Position* exit, unsigned short x, unsigned short y,Enemy &enemy, short &buttonIndex,itemCatalog &inventory, Chest &chest, bool &isPressed,unsigned short roomNum,string &message, unsigned short &turn);
Position* ushort_to_pos(unsigned short roomNum,unsigned short x, unsigned short);
void setBegPos(Player& player,unsigned short roomNum,unsigned short x, unsigned short y);
void gameWin();

void roomChanged(unsigned short &gameState, unsigned short change, unsigned short x, unsigned short y, Player &player)
{
	//battle sequence turn
	unsigned short turn = 0;
	//sent render screen by reference to render a message at the bottom of the screen
	string message = "";
	//battle sequence button index
	short buttonIndex = 0;
	//check if 'i' is pressed in battle sequence
	bool isPressed = 0;
	//exits of each room coded in an array as u_short, then decoded into position arrays by ushort_to_pos
	unsigned short rooms[] = {0, 3, 2, 3, 0, 1, 2, 0};
	//when collided with entrance go to the previous room with -1 change exit +1 with 0 change at start of the game
	unsigned short roomNum = readWhichRoom() + change;

	unsigned short roomExitNum; 
	unsigned short roomEntranceNum;
	//check if all enemies have died
	gameWin();

	//entrance and exits are the index of roomNum array by its roomNum
	if(roomNum > 0)
		roomEntranceNum = rooms[roomNum - 1];

	if(roomNum < 7)
		roomExitNum = rooms[roomNum];



	//right wall of a room, is the left wall to the near room. the exit of a room's symmetry is the enterance of the next room 
	if(roomEntranceNum == 3)
		roomEntranceNum = 1;

	else if(roomEntranceNum == 2)
		roomEntranceNum = 0;

	else if(roomEntranceNum == 1)
		roomEntranceNum = 3;

	else if(roomEntranceNum == 0)
		roomEntranceNum = 2;


	message = "Room is: " + to_string(roomNum);
	message += "\n'w' 'a' 's' 'd' for movement. 'i' displaying for inventory";

	//reading the save and check which enemy is on this room
	unsigned short enemyNumber = readEnemyLoc(roomNum);
	Enemy enemy(enemyNumber,x,y);
	//read inventory as a ushort array by readInventory
	unsigned short* inventoryRead = readInventory();

	itemCatalog inventory;
	//populate the inventory
	for(unsigned short i = 0; i < 7; i++)
		inventory.setAmount(i, inventoryRead[i]);
	if(enemyNumber == 0)
		message = "Froggie is strong against water, weak against air.";
	else if(enemyNumber == 1)
		message = "Tucan is strong against earth, weak against water.";
	else if(enemyNumber == 2)
		message = "Orca is strong against air, weak against earth.";
	
	delete[] inventoryRead;
	saveRoom(roomNum);
	Chest chest;
	//check if this chest is opened before
	if(isChestOpened(roomNum))
		chest.setFlag();
	chest.pos.setPos(x/2,y/2);
	chest.adjustImagePos();
	short health = readHealth();
	player.setHealth(health);
	player.pos.setPos(15,15);
	//defining two position array for entrance and exits 
	Position* entrance; 
	Position* exit; 

	//set players position to the near of the entrance/exit
	if(change == 1 || change == 0)
		setBegPos(player,roomEntranceNum,x,y);
	else
		setBegPos(player,roomExitNum,x,y);

	//check if the enemy on this room has already died
	if(isDead(enemyNumber) && enemyNumber < 3)
		enemy.die();

	if(roomNum != 0)
		entrance = ushort_to_pos(roomEntranceNum, x, y);
	else
		entrance = nullptr;

	if(roomNum != 8)
		exit = ushort_to_pos(roomExitNum, x, y);
	else
		exit = nullptr;

	
        listenInput(gameState,player,enemyNumber,entrance,exit,x,y,enemy, buttonIndex,inventory, chest,isPressed,roomNum,message,turn);
	render_screen(x,y,player,entrance,exit,enemy,gameState, buttonIndex, inventory, chest, enemyNumber,isPressed,message,turn);
	if(entrance != nullptr)
		delete[] entrance;
	if(exit != nullptr)
		delete[] exit;

}

void gameWin()
{
	bool flag = true;
	for(int i = 0; i < 3; i++)
		if(!isDead(i))
			flag = false;
	if(flag == true)
	{
		cout << "\n\n\n\n\n\n\n\n\n\n" <<"CONGRATSS!! YOU BEAT BRICK THROWER!!!";
		exit(0);
	}	
}

void setBegPos(Player& player,unsigned short roomNum,unsigned short x, unsigned short y)
{

	if(roomNum == 0)
	{
		player.pos.setPos(10,y/2);

	}
	
	if(roomNum == 1)
	{
		player.pos.setPos(x/2,y-10);

	}
	
	if(roomNum == 2)
	{
		player.pos.setPos(x-10,y/2);

	}

	if(roomNum == 3)
	{
	
		player.pos.setPos(x/2,10);

	}	
	player.adjustImagePos();
}

Position* ushort_to_pos(unsigned short roomNum,unsigned short x, unsigned short y)
{
	//size of the gates
	unsigned short size = 10;
	Position* pos = new Position[size];
	if(roomNum == 0)
	{
		for(int i = 0; i < size; i++)
		{
			pos[i].setX(0);
			pos[i].setY(((y-1)/2) + (4 - i));	
		}
	}
	
	if(roomNum == 1)
	{
		for(int i = 0; i < size; i++)
		{
			pos[i].setX(((x-1)/2) + (4 - i));
			pos[i].setY(y-1);	
		}
	}
	
	if(roomNum == 2)
	{
		for(int i = 0; i < size; i++)
		{
			pos[i].setX(x-1);
			pos[i].setY(((y-1)/2) + (4 - i));	
		}
	}

	if(roomNum == 3)
	{
		for(int i = 0; i < size; i++)
		{
			pos[i].setX(((x-1)/2) + (4 - i));
			pos[i].setY(0);	
		}
	}
	return pos;
}

//LISTEN_INPUT

bool colliderCheck(Player &player, unsigned short &i, unsigned short enemyNumber, Position* enterance, Position* exit, unsigned short x, unsigned short y, Enemy &enemy,short &buttonIndex, itemCatalog &inventory, Chest &chest, bool &isPressed,unsigned short roomNum,string &message, unsigned short &turn);
void posChange(Player &player, char input, itemCatalog &inventory);


void listenInput(unsigned short &i /*i is gamestate*/ , Player &player, unsigned short enemyNumber, Position* enterance, Position* exit, unsigned short x, unsigned short y, Enemy &enemy,short &buttonIndex, itemCatalog &inventory, Chest &chest, bool &isPressed,unsigned short roomNum, string &message, unsigned short &turn)
{

	char input;
	render_screen(x,y,player,enterance,exit,enemy,i, buttonIndex,inventory, chest, enemyNumber,isPressed,message,turn);	

	while(1)
	{
		
		cin >> input;
		
		if (i == 0) 		
		{
			render_screen(x,y,player,enterance,exit,enemy,i, buttonIndex,inventory, chest, enemyNumber,isPressed,message,turn);	

			//saving the position (ogPos) so if the player collides into a wall it doesnt go forward and reverts back to the og.
			Position ogPos = player.pos;
			posChange(player, input,inventory);

			if (colliderCheck(player, i, enemyNumber, enterance, exit, x, y,enemy, buttonIndex,inventory, chest, isPressed,roomNum,message,turn) == 1)
			{
				player.pos = ogPos;		
			}
		}	


		else if (i == 1)
		{
			//gamestate is 1 so its the battle screen. so the inputs change the button index.
			buttonIndexChange(buttonIndex,input,isPressed,turn);
			render_screen(x,y,player,enterance,exit,enemy,i, buttonIndex,inventory, chest, enemyNumber,isPressed,message,turn);	

			if (enemy.getHealth() == 0)
			{
				saveDeadEnemies(enemyNumber);
				saveHealth(player.getHealth());
				i = 0;
				saveInventory(inventory);
			}
		}
	}

}



void posChange(Player &player, char input, itemCatalog &inventory)
{
	unsigned short updateX;
	unsigned short updateY;

	
	if (input == 'a' || input == 'A')
	{
		updateX = player.pos.getX();
		updateX -= 3;
		player.pos.setX(updateX);
		player.adjustImagePos();
	
	}
	
	else if (input == 'd' || input == 'D')
	{
		updateX = player.pos.getX();
		updateX += 3;
		player.pos.setX(updateX);
		player.adjustImagePos();
	
	}

	else if (input == 'w' || input == 'W')
	{
		updateY = player.pos.getY();
		updateY -= 3;
		player.pos.setY(updateY);
		player.adjustImagePos();
	
	}

	else if (input == 's' || input == 'S')
	{
		updateY = player.pos.getY();
		updateY += 3;
		player.pos.setY(updateY);
		player.adjustImagePos();
	
	}

	else if(input == 'i')
	{
		cout << "\n<INVENTORY>\n";
		for(int i = 0; i < 7; i++)
		{
			Item item(i);
			if(i != 0)
				cout << item.getName() << " x " << inventory.getAmount(i) << endl;
			else
				cout << item.getName() << ": " << "there are plenty on floor." << endl;
		}
	}	
		return;
}

void buttonIndexChange(short& buttonIndex,char input, bool &isPressed, unsigned short &turn)
{
	if(turn == 1)
	{
		if (input == 'a')
		{
			buttonIndex--;
			
		}
		
		else if (input == 'd')
		{
	
			buttonIndex++;
		
		}
		else if(input == 'i')
			isPressed = true;
	}

}

bool colliderCheck(Player &player, unsigned short &i, unsigned short enemyNumber, Position* enterance, Position* exit, unsigned short x, unsigned short y,Enemy &enemy,short &buttonIndex,itemCatalog &inventory, Chest &chest, bool &isPressed,unsigned short roomNum,string &message, unsigned short &turn)
{

	unsigned short colDistance = 5;

	//calculating the distance to the middle according to the players' position.
	if ((player.pos.getX() > (x/2)-(colDistance *3)&& player.pos.getX() < (x/2) + (colDistance * 3)) && (player.pos.getY() > (y/2)-colDistance && player.pos.getY() < (y/2) + colDistance))
	{
	
			if (enemyNumber < 3) //this means there is an enemy.
			{
				if(!enemy.getIsEnemyDead())
				{
					i++;
				}
				return 1;
			}
	
			else if(enemyNumber >= 3)  //if its not on of the 3 enemies, it is a chest.
			{	
				if(!chest.getFlag())
				{
					chest.addInventory(inventory,message);
					chestOpened(roomNum);
					saveInventory(inventory); 
				}
				render_screen(x, y, player, enterance, exit, enemy, i, buttonIndex,inventory, chest, enemyNumber,isPressed,message,turn);
				
				return 1;
			}
	
	}


	
					

	
//for enterance and exit colliders:

	for (unsigned short j = 0; j < 5; j++)
	{ 	
		if(enterance != nullptr)
		{
			//for enterance
			if (enterance[j].getX() == x - 1) //right wall
			{
		
				if (player.pos.getX() + colDistance >= enterance[j].getX() && player.pos.getY() == enterance[j].getY()) 
				{
			
					roomChanged(i, -1, x, y, player);
					return 1;
				}
			}

			else if (enterance[j].getX() == 0) //left wall
			{
				if (player.pos.getX() - colDistance <= enterance[j].getX() && player.pos.getY() == enterance[j].getY())
				{
					roomChanged(i, -1, x, y, player);
					return 1;
				}
	
		
			}

			else if (enterance[j].getY() == 0) //top wall
			{	
				if (player.pos.getY() - colDistance <= enterance[j].getY() && player.pos.getX() == enterance[j].getX())
				{
					roomChanged(i, -1, x, y, player);
					return 1;
	
				}
		
			}

			else if (enterance[j].getY() == y - 1) //bottom wall
			{
				if (player.pos.getY() + colDistance >= enterance[j].getY() && player.pos.getX() == enterance[j].getX())
				{
					roomChanged(i, -1, x, y, player);
					return 1;
	
				}
		
			}
		}

	
		
		//for exit
		if(exit != nullptr)
		{		
			if (exit[j].getX() == x-1) //right wall
			{
				if (player.pos.getX() + colDistance >= exit[j].getX() && player.pos.getY() == exit[j].getY()) 
				{
					roomChanged(i, 1, x, y, player);
					return 1;
				}
			}

			else if (exit[j].getX() == 0) //left wall
			{
				if (player.pos.getX() - colDistance <= exit[j].getX() && player.pos.getY() == exit[j].getY())
				{
					roomChanged(i, 1, x, y, player);
					return 1;
				}
		
			}

			else if (exit[j].getY() == 0) //top wall
			{
				if (player.pos.getY() - colDistance <= exit[j].getY() && player.pos.getX() == exit[j].getX())
				{
					roomChanged(i, 1, x, y, player);
					return 1;
	
				}
		
			}

			else if (exit[j].getY() == y-1) //bottom wall
			{
				if (player.pos.getY() + colDistance >= exit[j].getY() && player.pos.getX() == exit[j].getX())
				{
					roomChanged(i, 1, x, y, player);
					return 1;
	
				}
		
			}
		}
	}

	//wall crashes:	
	if(player.pos.getX() + colDistance >= x || player.pos.getX() - colDistance <= 0 || player.pos.getY() + colDistance >= y  || player.pos.getY() - colDistance <= 0)
	{
		return 1;
	}
			
	return 0;		
}
