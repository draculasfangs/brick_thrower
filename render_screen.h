#include <iostream>
#include <fstream>
#include "object.h"
#include "UIManagment.h"
#include "battle_sequence.h"

using namespace std;

void render_screen(unsigned int x,unsigned  int y, Player &player,Position* entrance, Position* exit, Enemy &enemy,unsigned short gameState,short &buttonIndex, itemCatalog &inventory,Chest &chest, unsigned short enemyNumber,string &message);
void print_string(const string &str);
void add_to_string_free_roam(string &str, unsigned int x, unsigned int y,  Position* entrance, Position* exit,Player &player, Enemy &enemy,Chest &chest,unsigned short enemyNumber,string &message);
string charOnPixel(unsigned short x, unsigned short y, Position* entrance, Position* exit, Player &player,Enemy &enemy, Chest &chest,unsigned short enemyNumber);
bool notGate(unsigned short x, unsigned short y, Position* entrance, Position* exit);

void render_screen(unsigned int x,unsigned  int y, Player &player, Position* entrance, Position* exit, Enemy &enemy,unsigned short gameState,short &buttonIndex, itemCatalog &inventory, Chest &chest, unsigned short enemyNumber, bool &isPressed,string &message, unsigned short &turn)
{
	//two strings for two game states
	string* str = new string[2];

	if(gameState == 0)
		add_to_string_free_roam(str[0], x, y, entrance, exit,player,enemy,chest,enemyNumber,message);
	else if(gameState == 1)
		str[1] = fight(player,enemy,x,y,buttonIndex,inventory, isPressed, turn);
	//print the string at the game state
	print_string(str[gameState]);
	delete[] str;
}

/*adds the walls and eveything between to the string that will be printed out*/
void add_to_string_free_roam(string &str, unsigned int x, unsigned int y,  Position* entrance, Position* exit,Player &player, Enemy &enemy,Chest &chest,unsigned short enemyNumber,string &message)
{
	for(int i = 0; i < y; i++)
	{
		for(int j = 0; j < x; j++)
		{
			if(i == 0)
				//checking if there is a wall or not for the edges
				if(notGate(j,i,entrance,exit))
					str += "-";
				else
					str += " ";

			else if( i != 0 && (j == 0 || j == x - 1) && (i != y - 1))
				if(notGate(j,i,entrance,exit))
					str += "|";
				else
					str += " ";


			else if (i == y - 1)
				if(notGate(j,i,entrance,exit))
					str += "-";
				else
					str += " ";

			
			else
			{
				//checking if there is anything on this pixel
				str += charOnPixel(j,i,entrance,exit,player,enemy,chest,enemyNumber);
			}
			
		}
		
		str += '\n';
	}
	//adding the message at the bottom
	str += message;
	str += '\n';
}

bool notGate(unsigned short x, unsigned short y, Position* entrance, Position* exit)
{
	//iterates to find out if there is a gate or not on this pixel
	if(entrance != nullptr)
		for(int i = 0; i < 10; i++)
			if(entrance[i].isEqual(x,y))
			{
				return false;
			}
	if(exit != nullptr)
		for(int i = 0; i < 10; i++)
			if(exit[i].isEqual(x,y))
			{
				return false;
			for(int i = 0; i < 10; i++)
		if(entrance[i].isEqual(x,y) || exit[i].isEqual(x,y))
		{
			return false;
		}
}

	return true;
}


string charOnPixel(unsigned short x, unsigned short y, Position* entrance, Position* exit, Player &player,Enemy &enemy, Chest &chest,unsigned short enemyNumber)
{
	string str = "";
	bool flag = true;
	string playerImage = player.getImage();
	unsigned short playerImageSize = playerImage.size();

	//iterates every char on player image to find out if there is a player image on this pixel
	for(int i = 0; i < playerImageSize; i++)
	{
		if(player.imagePos[i].isEqual(x,y))
		{
			//add that image to pixel instead of blank space
			str += playerImage[i];
			flag = false;
		}
	}
	
	string enemyImage = enemy.getImage();
	unsigned short enemyImageSize = enemyImage.size();

	if(enemyNumber < 3)
	{
		//iterates every char on player image to find out if there is a player image on this pixel
		for(int i = 0; i < enemyImageSize; i++)
		{
			if(enemy.imagePos[i].isEqual(x,y))
			{
				//add that image to pixel instead of blank space
				str += enemyImage[i];
				flag = false;
			}
		}
	}
	string chestImage = chest.getImage();
	unsigned short chestImageSize = chestImage.size();

	//chest condition
	if(enemyNumber >= 3)
	{
		for(int i = 0; i <chestImageSize; i++)
		{
			if(chest.imagePos[i].isEqual(x,y))
			{
				//add that image to pixel instead of blank space
				str += chestImage[i];
				flag = false;
			}
		}

	}

	if(flag == true)
	{
		str += " ";
	}

	return str; 
}


void print_string(const string &str)
{
	cout << str;
}
