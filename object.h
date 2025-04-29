#include <iostream>
#include "position.h"
#include <ctime>
#include <random>
#include <cstdlib>
#include <climits>

#ifndef OBJECT_H
#define OBJECT_H


using namespace std;

class Object
{


	protected:
		string image;
		string copyImage;
		unsigned short center_x;
		unsigned short center_y;
	public:
		//attributes
		Position pos;

		//constructors
		Object(const string&, Position&);
		Object();
		//Object(Object&);
		~Object();
		//setters
		void setImage(const string&);
		string getImage();  

		//utility functions
		//this array holds every characters relevant position to the center to render them in screen one by one
		Position* imagePos;
		//assigns positions to imagePos array
		void adjustImagePos();
};

Object::Object()
{
	this->image = "";
	imagePos = nullptr;
	center_x = 0;
	center_y = 0;

}


Object::Object(const string &image, Position &pos)  
{
	this->image = image;
	copyImage = image;
	this->pos = pos;
	center_x = 0;
	center_y = 0;
	adjustImagePos();
}

Object::~Object()
{
	if(imagePos != nullptr)
		delete[] imagePos;
	imagePos = nullptr;
}

void Object::setImage(const string &img)
{
	this->image = img;
	image = copyImage;
}

string Object::getImage()
{
	return this->image;
}


class Player: public Object

{
	using Object::Object; //inheriting construct 
	short health;
	unsigned short brick_amount;
	public:
		Player(const string&,Position&);
		Player();
		Player(Player&);
		void setHealth(short); //for decreasing and increasing the health on battle
		short getHealth();
		void setItems(unsigned short*);
		void takeDamage(unsigned short);
		void die();
		void restore(unsigned short);
		unsigned short getItems(); 
};

void Player::restore(unsigned short value)
{
	if(health < 70)
		health += value;
	else
		health = 100;
}

Player::Player(const string &image, Position &pos)  
{
	this->health = 100;
	this->image = image;
	copyImage = image;
	this->pos = pos;
	center_x = 0;
	center_y = 0;
	adjustImagePos();
}


Player::Player()
{
	this->image = "";
	imagePos = nullptr;
	center_x = 0;
	center_y = 0;

	this->health = 100;
	

}

Player::Player(Player &obj)
{

	this->image = obj.image;
	this->pos = obj.pos;
	this->copyImage = this->image;
	this->adjustImagePos();
}

void Player::die()
{
	cout << "YOU DIED!" << endl;
	exit(0);	
}

void Player::takeDamage(unsigned short damage)
{
	health -= damage;
}

void Player::setHealth(short h)
{
	this->health = h;
}

short Player::getHealth()
{
	return this->health;
}


class Enemy: public Object
{
	string name;
	string fightImage;
	char type;
	bool isBoosted;
	bool isDefending;

	bool isEnemyDead;
	short health;
	public:	
		char getType();
		Enemy(unsigned short,unsigned short, unsigned short);
		short getHealth();
		string getName();
		string getFightImage();
		void takeDamage(unsigned short);
		void attackPattern(Player&);
		void die();

		void Boost();
		void Defend();


		bool getIsEnemyDead();


};

void Enemy::Boost()
{
	isBoosted = true;
}
void Enemy::Defend()
{
	isDefending = true;
}
	

bool Enemy::getIsEnemyDead()
{
	return this->isEnemyDead;
}

void Enemy::die()
{
	isEnemyDead = true;
	//set image to skull
	this->image = R"( b  _____b /     \b| () () |b \  ^  /b  |||||)";
	this->copyImage = image;
	center_x = 0;
	center_y = 0;
	health = 0;
	this->adjustImagePos();	
}


char Enemy::getType()
{
	return this->type;
}

void Enemy::attackPattern(Player &player)
{
	if(isDefending)	
	{
		player.takeDamage(5);
		isDefending = false;
	}
	else
		player.takeDamage(12);
}

void Enemy::takeDamage(unsigned short damage)
{
	if(isBoosted)
	{
		isBoosted = false;
		
		health -= (2*damage); 
		
	}
	else
		health -= damage;
}

short Enemy::getHealth()
{
	return health;
}
string Enemy::getName()
{
	return name;
}
string Enemy::getFightImage()
{
	return fightImage;
}


Enemy::Enemy(unsigned short enemyNumber,unsigned short x, unsigned short y)
{
	this->image = "";
	imagePos = nullptr;
	center_x = 0;
	center_y = 0;
	health = 100;

	pos.setPos(x/2,y/2);

	if(enemyNumber == 0)
	{
		this->name = "POISONOUS FROGGIE";
		this->image = R"(       o  o        b      ( -- )       b   /\( ,   ,)/\    b^^   ^^  ^^   ^^)";
		this->copyImage = image;
		this->fightImage = R"(                                              
                      ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⡤⠤⠤⠤⠤⠤⠤⠤⣤⣄⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                      ########## ⠀⠀⠀⠀⠀⠀⢀⣠⣴⣖⡒⠲⢯⡉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣉⡭⠝⠒⣲⣶⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                     #######⠀⠀⠀⠀##⠀⠀⠀⠀⢠⣾⠿⠿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⣾⠿⠿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                    #########⠀⠀⠀⠀#⠀⠀⠀⠀⣿⣏⢀⡰⢻⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡀⣠⠟⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                   ###########⠀⠀⠀#⠀⠀⠀⣴⣿⣿⣿⣷⣾⣿⢿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣷⣾⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                   ##########⠀⠀⠀##⠀⠀⠘⢧⠹⢿⣿⣿⣿⣫⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠿⣿⣿⣿⣻⣿⠟⣹⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                   ##############⠀⠀⠀⢀⡼⠦⠀⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠈⠷⠀⠀⠾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠁⠀⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                   ##########⠀⠀⠀⠀⠀⠀⠸⣏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢳⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                   ############⠀⠀⠀⠀⠀⠻⣏⣲⠦⣄⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣶⡿⠷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                    ############⠀⠀⠀⡴⠊⢀⣿⠉⠒⠿⠯⢭⣁⣒⣒⣒⣒⣒⣒⣒⣒⣶⣶⣶⣖⣒⣫⣹⣗⣒⣒⣲⣖⣽⠯⠿⠗⣿⣧⠀⠀⠘⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                     ############⢠⠞⠁⠀⡼⠀⣧⠀⠀⠀⠀⠀⠉⠙⠉⠉######⠉##⠀⠀⣀⡴⠞⠉⠀⠀⠀⠀⢀⡇⢸⡀⠀⠀⠈⢧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                     ##⠀########⣴##⠀⣸⠃⠀⢹⠀⠀⠀⠀⠀⠀⠀⠀#########⠀⠀⠚⠁⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⢷⡀⠀⠀⠘⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                      ⠀⠀⠀⠀⢀#########⠇⠀⠀⡿⠀⠀⠀⠀⠀⠀⠀⠀#########⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠙⢦⡀⠀⢻⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀              
                      ⠀⠀⣰⠾⠋⠁⠀⠀########⡼⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀########⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠙⢦⣼⠉⠙⠒⠶⢤⣀⠀⠀⠀⠀              
                      ⢀⡾⠃⠀⠀⠀⠀⣠⠾⠋⠀######⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀#####⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡇⠀⠀⠀⢀⡀⠀⠙⣆⠀⠀⠀⠀⠈⠑⢦⡀⠀                                                                                                                                      ⠀⠀⠀     
)";

		this->type = 'e';		
	}

	else if(enemyNumber == 2)
	{
		this->name = "APEX ORCA";
		this->image = R"(              ___      ___b            _ '.'--.--'.-'b.,_-----.__/_|,   \' r'b', '-._::::::::'-' .'b '.    '-'..\:::/b   '--;____'--.'-,b   /..'       ''')";
		this->copyImage = image;
		this->fightImage = R"(                                                     ⠀⠀
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⢀⣀⣄⡀⠰⠴⣶⣶⣤⣤⡀⠀
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⣿⣿⣿⡇⠀⢀⣤⣶⣻⣾⣿⣴⣴⣾⣿⣿⣿⣿⣿⣿⡆
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣥⣾⠿⢿⣿⣽⣾⣿⣿⣿⣿⣿⣿⣿⠿⢿⡿⣧
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⠟⠉⠀⠀⠀⣸⣿⣿⣿⣿⡿⠟⠛⠋⠉⠐⠊⠡⢹⢚
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣠⣤⣤⡴⠂⠐⠒⢨⣿⣿⣿⣿⣿⣿⣤⣆⣤⣠⣴⣾⣿⣷⡿⠋⠁⠀⠀⠀⠀⠀⠐⣁⠎⠀⡘
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢐⣠⣤⣶⣾⣿⣿⣿⣿⣿⣆⡀⡀⣀⣨⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡜⠀⠀⡐⠀
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠄⠀⠄⠀⠀⠀⠀⠀⠂⠀⠀
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⡶⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡠⠂⠀⠀⠀
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣵⣿⣿⣅⠀⠀⠀⠀⢈⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠖⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠂⠀⠀⠀⠀⠀
		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣶⣦⣌⠁⠀⠉⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⡞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠜⠁⠀⠀⠀⠀⠀⠀
		⠀⠀⠀⣀⣀⣤⢤⢤⡴⢶⣾⡿⠿⣛⠩⠀⠉⠉⠙⠛⠻⠿⢏⡀⠀⠀⠀⠙⠻⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢈⡷⠀⠀⠀⠀⠀⠀⠀⠀⣠⣷⣿⡀⠀⠀⠀⠀⠀⠀⠀
		⢠⠖⠋⠉⠀⢀⠀⠂⣌⢇⠀⣰⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⣄⠀⡀⠀⠀⢀⣽⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⣐⠰⠂⠀⠀⠀⠀⡀⣠⣴⣾⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
		⠛⠓⠒⠲⢤⣀⣐⣤⡞⣸⢊⠥⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠀⢀⣤⣿⣿⣿⣿⣿⣿⣿⡿⠟⠋⢄⣀⠀⠠⠤⠴⠂⠈⠁⢰⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
		⠀⠀⠀⠀⠀⠀⠀⠀⢿⠃⠀⠀⠸⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                     
)";

		this->type = 'w';		
	}

	else if(enemyNumber == 1)
	{
		this->name = "DEADLY TOUCAN";
		this->image = R"(         ,-,---.b      __/( ,----`b  _,-'    ;b;;.---..-'b       "")";
		this->copyImage = image;
		this->fightImage = R"(
					_,---._      __,...-----...___
				      _,-:::,,--.`,--''                 `'--._
				    ,':::::/((##)):                           `-.
				  ,':.::::/  `--' :         _____.....______ (:::\
				 /:::::::/        :__,.-''''..- - - - --  -- .`_-:\
				/:,:::.::|        ::.          ____....-----.....`.
			       /,:::::::/          ::::__.--'''
			       |:::::::|           _:'
			       |:.:::::|         ,'
			       |:::::::|         |
			      /::::.:::|         |
			 __,-'::.::::::|         |
		  _,.--''::::_::::::::::\        |
		''::_::,:--''  `'--.:::::\       ;
		-'''::::::::::::::::\:::::`.     ;
		:::::::::::::::::::::|:.::::`-..-
		::;::::::::::::::::::|::::::::::/
		:/::::::/::::::;:::::|::::.::::/
		(::::::/::::::/::::::):.::::::'
)";
		
		this->type = 'a';

	
	}

	this->adjustImagePos();
}



//inventory items class inherited from the object class:
void Object::adjustImagePos()
{

	if(imagePos != nullptr)
	{
		delete[] imagePos;
		imagePos = nullptr;
	}

	unsigned short temp_x = 0;
	unsigned short size = image.size();
	imagePos = new Position[size];

	if(center_x == 0 && center_y == 0)
	{
		//iterating over string to find out its width and height 
		for(int i = 0; i < size; i++)
		{
			if(image[i] == 'b')
			{
				if(temp_x >= center_x)
					center_x = temp_x;
				center_y++;
				temp_x = 0;
			}
			else
				temp_x++;
		}
		//dividing x and y by 2 because we need the middle position
		center_x /= 2;
		center_y /= 2;
	}
	
	//finding every characters distance in y and x terms relevant to middle center, assigning positions to this middle plus the distance between;
	unsigned short iterator_x = 0;
        unsigned short iterator_y = 0;

	for(int i = 0; i < size; i++)
	{
		if(copyImage[i] == 'b')
		{		
			iterator_x = 0;
			iterator_y++;
			image[i] = ' ';
			imagePos[i].setPos(this->pos.getX() + ((iterator_x) - center_x), this->pos.getY() + ((iterator_y) - center_y));
		}
		else
		{
			iterator_x++;	
			imagePos[i].setPos(this->pos.getX() + ((iterator_x) - center_x), this->pos.getY() + ((iterator_y) - center_y));
		}
	}
}

//inventory, keeps all the items and their amount
class itemCatalog
{
	unsigned short size = 7; //there are 7 items
	unsigned short* inventory;
	public:
		itemCatalog(); 
		~itemCatalog(); //deconstructor cuz we dynamically alocate
		void setAmount(unsigned short, unsigned short);
		unsigned short getAmount(unsigned short);
		
		

};

itemCatalog::itemCatalog()
{
	inventory = new unsigned short[size]; 
	for(int i = 0; i < size; i++)
	{
		inventory[i] = 0;
	}

}

itemCatalog::~itemCatalog()
{
	if(inventory != nullptr)	
		delete[] inventory;
	inventory = nullptr; 
}


void itemCatalog::setAmount(unsigned short itemIndex, unsigned short amount)
{
	this->inventory[itemIndex] = amount;
}
 

unsigned short itemCatalog::getAmount(unsigned short itemIndex)
{
	return this->inventory[itemIndex];
}



class Item: public Object
{	
	string name;
	string description; //explanation of the item
	unsigned short amount; //how many items the player has 
	unsigned short itemNumber; 

	public: 
		Item(unsigned short);
		unsigned short getItemIndex();
		void UsingItem(Enemy&,Player&,itemCatalog&);
		string getName();
		string getDescription();
};

string Item::getDescription()
{
	return this->description;
}

string Item::getName()
{
	return this->name;
}

void Item::UsingItem(Enemy &enemy, Player &player, itemCatalog &inventory)
{
	cout << "Using item" << endl;

	if(itemNumber == 0)
	{
		cout << "0" << endl;
		enemy.takeDamage(15);
	}
	else if(itemNumber == 1)
	{
		player.restore(30);
	}	
	else if(itemNumber == 2)
	{
		if(enemy.getType() == 'w')
			enemy.takeDamage(30);
		else if(enemy.getType() == 'a')
			enemy.takeDamage(10);
		else
			enemy.takeDamage(20);
	}		
	else if(itemNumber == 3)
	{
		if(enemy.getType() == 'a')
			enemy.takeDamage(30);
		else if(enemy.getType() == 'e')
			enemy.takeDamage(10);
		else 
			enemy.takeDamage(20);
	}		
	else if(itemNumber == 4)
	{
		if(enemy.getType() == 'e')
			enemy.takeDamage(30);
		else if(enemy.getType() == 'w')
			enemy.takeDamage(10);
		else
			enemy.takeDamage(20);
	}		
	else if(itemNumber == 5)
	{
		enemy.Boost();
	}		
	else if(itemNumber == 6)
	{
		enemy.Defend();
	}		


}

Item::Item(unsigned short itemNo)
{
	this->itemNumber = itemNo;
	if(itemNo == 0)
	{
		this->name = "Brick";
		this->description = "Throw a brick (could be dangerous);)";
	}
	else if(itemNo == 1)
	{
		this->name = "Potion";
		this->description = "Restores 30 HP!";
	}	
	else if(itemNo == 2)
	{
		this->name = "Earth Spell";
		this->description = "Deals damage. Effective against water. Weak against air.";
	}		
	else if(itemNo == 3)
	{
		this->name = "Water Spell";
		this->description = "Deals damage. Effective against air. Weak against earth.";
	}		
	else if(itemNo == 4)
	{
		this->name = "Air Spell";
		this->description = "Deals damage. Effective against earth. Weak against water.";
	}		
	else if(itemNo == 5)
	{
		this->name = "Attack Boost";
		this->description = "Strengthen your attack next turn.";
	}		
	else if(itemNo == 6)
	{
		this->name = "Defend";
		this->description = "Get less damage next turn.";
	}			

}



unsigned short Item::getItemIndex()
{
	return this->itemNumber; 
}




class Chest: public Object
{
	//itemIndex is rows, in each column its gonna be written how much of each item the player has.	
	unsigned short itemIndex = 7; 
	unsigned short cols = 2;
	unsigned short** datamount;
	bool flag = 0; 	
	public:
		Chest(); 
		~Chest();
		void addInventory(itemCatalog &inv, string& message);
		void setFlag();
		bool getFlag();
};

void Chest::setFlag()
{
	this->flag = true;
	this->image = R"(\-----------/b|    |_|    |b|           |b-------------)";	
	this->copyImage = image;
	this->adjustImagePos();

}
bool Chest::getFlag()
{
	return this->flag;
}

Chest::Chest()
{
	this->image = R"( ___________b/-----------\b|    |_|    |b|           |b-------------)";
	this->copyImage = image;		
	this->center_x = 0;
	this->center_y = 0;
	this->imagePos = nullptr;
	this->adjustImagePos();

	datamount = new unsigned short*[itemIndex]; 
	unsigned short* arr = new unsigned short[itemIndex*cols]; 
	for (unsigned short i = 0; i < itemIndex; i++)
	{
		datamount[i] = &arr[i*cols]; 
	}
    
	//first column is the indexes of the items and the second column is their amounts
	for(unsigned short i = 0; i < itemIndex; i++)
	{
		datamount[i][0] = i;
		datamount[i][1] = 0; 
	}
		

	 //it randomly decides how many items from which items there'll be in the chest.
	
	for(unsigned short i = 0; i < 2; i++)
	{
		unsigned short amountOfItems = rand() % 2 + 1;
		unsigned short whichItem = (rand() % 6) + 1;
		datamount[whichItem][1]	= amountOfItems;	
	}
}

Chest::~Chest()
{
	if(datamount != nullptr)
	{
		delete[] datamount[0];
		delete[] datamount;
	}
	datamount = nullptr;
}


void Chest::addInventory(itemCatalog &inv, string &message)
{
	message = "";
	if (flag == 0)
	{
		unsigned short preitemAmount = 0; //counts how many items there are in the inventory before it adds more items
		for(unsigned short i = 1; i < itemIndex; i++)
		{
			preitemAmount += inv.getAmount(i);	
		}
		for(unsigned short i = 1; i < itemIndex; i++)
		{
			//adds the items from the chest (datamount) on the index i(which item)
			unsigned short amountToAdd = datamount[i][1]; 
			unsigned short totalAmount = inv.getAmount(i) + amountToAdd;

			if(preitemAmount + amountToAdd < 8)
			{
				inv.setAmount(i, totalAmount);
				preitemAmount += amountToAdd;
			}
			else
			{
				message = "INVENTORY IS FULL";
				break;
			}
			Item item(i);
			
			if(amountToAdd > 0)	
				message += "Added " + to_string(amountToAdd) + " x " +  item.getName()  + '\n';
			flag = 1;
			this->image = R"(\-----------/b|    |_|    |b|           |b-------------)";
			this->copyImage = image;
			this->adjustImagePos();

		}	
		message += '\n';
	}

}


#endif 

