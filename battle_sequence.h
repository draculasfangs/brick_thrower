#include <iostream>
#include <string>

using namespace std;

//string align(string enemyImage, unsigned short x);
string centerName(unsigned short x ,string name);
string healthBar(short health);
void fightSeq(Player &player, Enemy &enemy, short &buttonIndex, Button* buttons, unsigned short itemAmount, itemCatalog &inventory,string &fightString, bool &isPressed, unsigned short &turn);

string fight(Player &player, Enemy &enemy,unsigned short x, unsigned short y, short &buttonIndex,itemCatalog &inventory, bool &isPressed, unsigned short &turn)
{
	string fightString = "";
	fightString += centerName(x,enemy.getName());
	short health = enemy.getHealth();
	fightString += "Health ";
	fightString += healthBar(enemy.getHealth());
	fightString += to_string(health);
	

	if (enemy.getType() == 'w')
		fightString += "\nEnemy specialized in WATER Spells\n";
	else if (enemy.getType() == 'a')
		fightString += "\nEnemy specialized in AIR Spells\n";
	else if (enemy.getType() == 'e')
		fightString += "\nEnemy specialized in EARTH Spells\n";



	fightString += enemy.getFightImage();

	
	unsigned short itemAmount = 1;

	for(unsigned short i = 1; i < 7; i++)
		itemAmount += inventory.getAmount(i);

	//creates 8 button as the size of inventory
	Button* itemButtons = new Button[8];
	unsigned short counter = 1;
	Button brickButton(15,6,0);
	itemButtons[0] = brickButton;

	//starts from 1 because 0 is the brick and its amount is infinite already
	for(unsigned short i = 1; i < 7; i++)
	{
		//fills buttons with the items in inventory
		unsigned short amountofAnItem = inventory.getAmount(i);
		while(amountofAnItem > 0)
		{
			Button itemButton(15,6,i);
			itemButtons[counter] = itemButton;
			counter++;
			amountofAnItem--;
		}
	}
	
	for(unsigned short i = 0; i < 8 - counter;i++)
	{
		Button itemButton(15,6,7);
		itemButtons[counter + i] = itemButton;
	}

		
	fightSeq(player, enemy, buttonIndex, itemButtons, itemAmount, inventory,fightString,isPressed,turn); 
	string playerHealthBar = healthBar(player.getHealth());
	fightString += "Your Health is: ";
	fightString += playerHealthBar;
	fightString += to_string(player.getHealth());
	fightString +=  "\n Press I to use an item!";
	delete[] itemButtons;
	return fightString;
}

string centerName(unsigned short x, string name)
{
	string str;
	unsigned short gap = (x - name.size()) / 2;
	for(unsigned short i = 0; i < gap; i++)
		str += "-";
	str += name;
	for(unsigned short i = 0; i < gap; i++)
		str += "-";
	str += '\n';
	return str;
}
string healthBar(short health)
{
	string str = "[";
	for(int i = 0; i < 100; i++)
		if(health  > i)
			str += "#";
		else 
			str += " ";
	str += "]";
	return str;
}

void fightSeq(Player &player, Enemy &enemy, short &buttonIndex, Button* buttons, unsigned short itemAmount , itemCatalog &inventory,string &fightString, bool &isPressed, unsigned short &turn)
{
	//preventing multiple if statements to work at the same call
	bool flag = 0;
	
	//keep buttonIndex in a range
	if (buttonIndex < 0)
	{
		buttonIndex = itemAmount -1;
	}

	else if (buttonIndex >= itemAmount)
	{
		buttonIndex = 0; 
	}


	string message;

	//turn 0 is introduction
	if (turn == 0 && flag == false)
	{
		message = "Enemy appears! Perform an attack before it kills you!";
		turn++;
 		flag = 1;
	}

	for (unsigned short i = 0; i < itemAmount; i++)
	{
		if (i == buttonIndex)
		{
			buttons[i].setisOn();
			unsigned short itemNo = buttons[i].getitemNum();
			
			Item temp(itemNo);
			unsigned short amountOfAnItem = inventory.getAmount(itemNo);
			//on turn 1 player attacks
			if (turn == 1 && flag == 0)
			{	
				message = temp.getDescription();	
				if(isPressed)								
				{
					// if the player presses i, than the button is pressed
	   
					message = "Using " + temp.getName();
					temp.UsingItem(enemy, player, inventory);
					amountOfAnItem--; 
					inventory.setAmount(itemNo, amountOfAnItem);

					isPressed = false;
					turn++;
				}
				flag = 1;
					
			}
			//on turn 2 enemy attacks and turn goes back to 1
			else if(turn == 2 && flag == 0)
			{
				message = "Taking damage.. Oh Noooooo!!!";
				enemy.attackPattern(player);		
				turn = 1;
				flag = 1;
			}		
			

			
		}
		else
		{
			buttons[i].setisOff();	
		}

	}

	Box messageBox(120,10,message ,false);
	fightString += messageBox.getBox();
	//align all the buttons in a line by UIManagment.h
	string alignedStr = alignUIElements(buttons,8,2,true,1);
	fightString += alignedStr;
	if (player.getHealth() <= 0)
	{
		player.die();
	}

	if (enemy.getHealth() <= 0)
	       enemy.die();		
}		
