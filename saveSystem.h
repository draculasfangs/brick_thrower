#include <iostream>
#include <fstream>

using namespace std;

//for writing:
void saveRoom(unsigned short roomnum);
void saveHealth(short h);
void saveEnemyLoc(unsigned short* enemies);
void saveDeadEnemies(unsigned short enemyNumber); 

//for reading:

unsigned short readEnemyLoc(unsigned short roomnum); //check if its dead inside the func   		
unsigned short readWhichRoom(unsigned short roomnum);
short readHealth();
bool isNewGame();
void readItems(itemCatalog &itemcat, Player player); 

//utility functions
unsigned short* assignEnemies();
bool isUnique(unsigned short* enemies, unsigned short randNum, unsigned short size);
bool isDead(unsigned short enemyNumber);

//save order
//room *2 - enemyloc *6 - health *2 - deadEnemies *6 - inventory *12 - chestOpened *16

void allocateFile();
void saveInventory(itemCatalog &inventory);
unsigned short* readInventory();
bool isChestOpened(unsigned short roomNum);
void chestOpened(unsigned short roomNum);


//creates a file with 44 bytes of 0
void allocateFile()
{


	ofstream file("saves.bin", ios::binary);
	char nullbyte = 0;

	for(unsigned short i = 0; i < 44; i++)
		file.write(&nullbyte, sizeof(char));

	file.close();
	//assigns enemy locations randomly
	unsigned short* enemies = new unsigned short[3];
	enemies = assignEnemies();
	saveEnemyLoc(enemies);
	saveRoom(0);
	saveHealth(100); 
	delete[] enemies;
}
void chestOpened(unsigned short roomNum)
{
	fstream file("saves.bin" , ios::in | ios::out | ios::binary);
	//10 is the start of isDead bytes, moving to enemy itself
	file.seekp(28 + (roomNum * 2));

	unsigned short openedByte = 1;

	if (file.is_open())
	{
		file.write((char*)&openedByte, sizeof(unsigned short)); 	
	}

	file.close(); 
}

bool isChestOpened(unsigned short roomNum)
{
	if(isNewGame())
		return 0;

	ifstream file("saves.bin", ios::binary | ios::ate);
	if(file.is_open())
	{
			
		streampos fileSize = file.tellg();
		char* mBlock = new char[fileSize];
		file.seekg(0,ios::beg);
		file.read(mBlock, fileSize);
		file.close();
		char* p = mBlock;
		unsigned short value;
		
		//going to the byte where we saved enemy locs
		for(unsigned short i = 0; i < 14 + roomNum; i++)
			p += sizeof(unsigned short);
		value = *((unsigned short*)p);
		delete[] mBlock;
		return value;
	}

	return 0;	
}

void saveInventory(itemCatalog &inventory)
{
	fstream file("saves.bin", ios::out | ios::in | ios::binary);

	file.seekp(16);
	if (file.is_open())
	{
		for (unsigned short i = 0; i < 6; i++)
		{
			unsigned short amount = inventory.getAmount(i+1);
			//plus one because the first item is brick
			file.write((char*)&amount, sizeof(unsigned short));
		}
		file.close();
	}

}

unsigned short* readInventory()
{
	unsigned short* inventory = new unsigned short[7];
	
	ifstream file("saves.bin", ios::binary | ios::ate);
	if(file.is_open())
	{
		streampos fileSize = file.tellg();
		char* mBlock = new char[fileSize];
		file.seekg(0, ios::beg);
		file.read(mBlock, fileSize);

		char* p = mBlock;
		file.close();
		//brick is infinite practically
		inventory[0] = 500;

		//moving cursor to inventory
		for(unsigned short i = 0; i < 8; i++)
			p += sizeof(unsigned short);

		for (int i = 0; i < 6; i++)
		{
			unsigned short amountOf = *((unsigned short*)p);
			inventory[i+1] = amountOf;
			p += sizeof(unsigned short);
			
		}
		delete[] mBlock;
	}
	return inventory;

}

short readHealth()
{
	if(isNewGame())
		return 100;


	ifstream file("saves.bin", ios::binary | ios::ate);
	if(file.is_open())
	{
		streampos fileSize = file.tellg();
		char* mBlock = new char[fileSize];
		file.seekg(0, ios::beg);
		file.read(mBlock, fileSize);

		char* p = mBlock;
		file.close();
		unsigned short value;
		for(unsigned short i = 0; i < 4; i++)
			p += sizeof(short);

		value = *((short*) p);  
		delete[] mBlock;
		return value;
	}

	return 100;	

}


void saveRoom(unsigned short roomnum)
{
	fstream file("saves.bin" , ios::in | ios::out | ios::binary);
	file.seekp(0);
	
	if (file.is_open())
	{
		file.write((char*)&roomnum, sizeof(unsigned short));
	}

	file.close(); 
}


		
void saveHealth(short h)
{

	fstream file("saves.bin" , ios::out | ios::in | ios::binary);
	file.seekg(0, ios::end);
	file.seekp(8);

	short health = h; 

	if (file.is_open())
	{
		
		file.write((char*)&health, sizeof(unsigned short));
	}

	file.close(); 	
}






void saveDeadEnemies(unsigned short enemyNumber)
{
	fstream file("saves.bin" , ios::in | ios::out | ios::binary);
	//10 is the start of isDead bytes, moving to enemy itself
	file.seekp(10 + (enemyNumber * 2));

	unsigned short deadByte = 1;

	if (file.is_open())
	{
		file.write((char*)&deadByte, sizeof(unsigned short)); 	
	}

	file.close(); 
}

bool isDead(unsigned short enemyNumber)
{
	if(isNewGame())
		return 0;

	ifstream file("saves.bin", ios::binary | ios::ate);
	if(file.is_open())
	{
		//going to the byte where we saved enemy locs
		
		streampos fileSize = file.tellg();
		char* mBlock = new char[fileSize];
		file.seekg(0,ios::beg);
		file.read(mBlock, fileSize);
		file.close();
		char* p = mBlock;
		unsigned short value;


		for(unsigned short i = 0; i < 5 + enemyNumber; i++)
			p += sizeof(unsigned short);

		value =  *((unsigned short*)p);
		delete[] mBlock;
		return value;
	}

	return 0;	
}



void saveEnemyLoc(unsigned short* enemies)
{
	fstream file("saves.bin", ios::out | ios::in | ios::binary);

	file.seekp(2);
	if (file.is_open())
	{
		for (unsigned short i = 0; i < 3; i++)
		{
			file.write((char*)&enemies[i], sizeof(unsigned short));
		}
		file.close();
	}
}


unsigned short readEnemyLoc(unsigned short roomnum)
{
	if(isNewGame())
		return 4;

	ifstream file("saves.bin", ios::binary | ios::ate);
	if(file.is_open())
	{
		//going to the byte where we saved enemy locs
		
		streampos fileSize = file.tellg();
		char* mBlock = new char[fileSize];
		file.seekg(0,ios::beg);
		file.read(mBlock, fileSize);
		file.close();
		char* p = mBlock;

		unsigned short enemyRoomNumber;
		p += sizeof(unsigned short);

		for (int i = 0; i < 3; i++)
		{
			enemyRoomNumber = *((unsigned short*)p);
			p += sizeof(unsigned short);
			if(enemyRoomNumber == roomnum)
			{
				file.close();
				return i;
			}
		}
		file.close();
	}

	return 3; //no enemy	
}	


unsigned short readWhichRoom()
{
	if(isNewGame())
		return 0;


	ifstream file("saves.bin", ios::binary | ios::ate);
	if(file.is_open())

	{
		streampos fileSize = file.tellg();
		char* mBlock = new char[fileSize];
		file.seekg(0, ios::beg);
		file.read(mBlock, fileSize);

		char* p = mBlock;
		file.close();
		return *((unsigned short*) p);  
	
	
		delete[] mBlock;
	}

	return 8; //total 7 rooms	
}

//
bool isNewGame()
{
	//if file does not exist allocate it
	ifstream file("saves.bin", ios::in);
	if(file.is_open())
	{
		file.close();
		return false;
	}
	else
	{
		cout << "Welcome to BRICK THROWER. Loot items from chests and use them against ENEMIES. Items and enemies have TYPES. You should decide wheter to fight or loot before fighting. You can avoid enemies by simply walking around them. HAVE FUN!!" << endl;
		allocateFile();
		return true;
	}
}
/*Randomly assigns enemies' position*/
unsigned short* assignEnemies()
{
	//dynamically allocating, because the array itself deallocated after return but pointers are not!
	unsigned short* enemies = new unsigned short[3];
	srand(time(NULL));
	unsigned short randNum;

	for(unsigned short i = 0; i < 3; i++)
	{
		do{
			//generates a number between 1 to 7
			randNum = (rand() % 6) + 1;
			//if the number is not already in array
			
		}while(!isUnique(enemies, randNum, 3));

		enemies[i] = randNum;
	}
	return enemies;
}

/*This function checks if the array contains the random number already or not*/
bool isUnique(unsigned short* enemies, unsigned short randNum, unsigned short size)

{
	for(unsigned short i =0; i < size; i++)
		if(enemies[i] == randNum)
			return false;
	return true;
}







