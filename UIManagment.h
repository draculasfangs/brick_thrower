#include <iostream>
#include <string>
using namespace std;

string* parseString(string message, unsigned short &wordCount);
char charOnLine(string aligned,unsigned short &counter);
string alignedString(unsigned short w, unsigned short h, string* words,unsigned short wordCount, bool isCentered);

class Box
{
	//attributes
	protected:
	unsigned short width;
	unsigned short heigth;
	string message;
	string box_string;
	public:
	//Constucter
		Box(unsigned short, unsigned short,string,bool);
		Box();
	//getter and setter
		string getBox();
		unsigned short getWidth();
		unsigned short getHeigth();
};

Box::Box()
{}

Box::Box(unsigned short width, unsigned short heigth, string message, bool isCentered)
{
	this->width = width;
	this->heigth = heigth;
	string str;

 	unsigned short wordCount;
	unsigned short counter = 0;
	//parse the string into words
	string* words = new string[32];
	words = parseString(message, wordCount);
	//align words to the center
	string aligned = alignedString(width,heigth,words,wordCount,isCentered);

	for(int i = 0; i < heigth; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(i == 0)
					str += "-";

			else if( i != 0 && (j == 0 || j == width - 1) && (i != heigth - 1))
					str += "|";

			else if (i == heigth - 1)
					str += "-";	
			else
			{
				str += charOnLine(aligned,counter);	
			}
		}
	
		str += '\n';
	}

	box_string = str;
	
}

string Box::getBox()
{
	return this->box_string;
}

unsigned short Box::getWidth()
{
	return this->width;
}
		
unsigned short Box::getHeigth()
{
	return this->heigth;
}

string* parseString(string message, unsigned short &wordCount)
{
	string* ans = new string[32];
 	unsigned short size = message.size();
	wordCount = 0;
	string emptyString = "";

	for(int i = 0; i < size; i++)
	{
		if(message[i] == ' ')
		{
			emptyString += ' ';
			ans[wordCount] = emptyString;
			emptyString = "";
			wordCount++;
		}
		else
		{
			emptyString += message[i];
		}
	}
	if(emptyString.size() != 0)
	{
		ans[wordCount] = emptyString;
		wordCount++;
	}
	return ans;
}

string alignedString(unsigned short w, unsigned short h, string* words,unsigned short wordCount, bool isCentered)
{
	string aligned = "";
	unsigned short line = 0;
	unsigned short wordCounter = 0;
	unsigned short size = wordCount;
	unsigned short wordsLength = 0;
	unsigned short lineCount = 0;

	unsigned short wordsOnAline[16] = {0};

	for(int i = 0; i < size; i++)
	{
		if(wordsLength + words[i].size() < w-2)
		{
			aligned += words[i];
			wordsLength += words[i].size();
			wordsOnAline[lineCount]++;
		}	
		else
		{
			lineCount++;
			wordsLength = words[i].size();
			aligned += words[i];
			wordsOnAline[lineCount]++;
		}
	}

	if(lineCount == 0)
		lineCount++;

	int counter = 0;
	unsigned short lineLength = 0;
	
	if(isCentered)
	{	
		//alligning words to center
		for(int i = 0; i < lineCount; i++)
		{
			lineLength = 0;
			for(int j = 0; j < wordsOnAline[i]; j++)
			{
				lineLength += words[counter].size();
				counter++;
			}

			unsigned short beginLine = (i)*(w-2);
			unsigned short lineSpaces =  ((w-2) - lineLength) / 2; 
			unsigned short lineGaps =  ((h-2) - lineCount) / 2 * (w-2);
			//beginning spaces
			aligned.insert(beginLine, lineSpaces,' ');
			//height spaces
			aligned.insert(0, lineGaps,' ');
		}
	}
	return aligned;
}

char charOnLine(string aligned,unsigned short &counter)
{
	char c;
	if(aligned.size() <= counter)
		return ' ';

	c = aligned[counter];
	
	counter++;
	return c;
}

//merge all box content to a one string horizontally, 
template <class UIELEMENT>
//used on both boxes and buttons 
string alignUIElements(UIELEMENT* boxes, unsigned short size, unsigned short gap, bool isButtons, unsigned short rows)
{
	string str = "";
	string gap_str;

	for(unsigned short i = 0; i < gap; i++)
		gap_str += ' ';

	unsigned short longestHeigth = 0;

	//comparing heights of boxes and finding the longest one
	for(unsigned short i = 0; i < size; i++)
		if(boxes[i].getHeigth() > longestHeigth)
			longestHeigth = boxes[i].getHeigth();

	unsigned short boxesOnAline = size/rows;

	for(unsigned short l = 0; l < rows; l++)
	{
		//keeping track of the lines horizontally 
		for(unsigned short i = 0; i < longestHeigth; i++)
		{
			//keeping track of same lines of all boxes
			for(unsigned short j = 0; j < boxesOnAline; j++)
			{
				if(boxes[j].getHeigth() >= longestHeigth)
				{
					unsigned short boxWidth = boxes[j].getWidth();
					//keeping track of a line of a box
					for(unsigned short k = 0; k < boxWidth + 1; k++)
					{
						string strForBox;
						unsigned short line_start = ((boxWidth + 1) * i);
	
						if(boxes[j].getisOn() && isButtons)
						{
							strForBox = boxes[j].getButtonString();
						}
						else
							strForBox = boxes[j].getBox();
		
						char char_to_add = strForBox[line_start + k];
	
						if(char_to_add != '\n')
							str += char_to_add;			
					}
					str += gap_str;
				}
			}
			str += '\n';
		}
		str += '\n';
	}

	return str;
}

class Button: public Box
{
	bool isOn;
	string buttonString;
	unsigned short itemNum;
	public:
		Button();
		Button(unsigned short width, unsigned short heigth, unsigned short itemNum);
		void buttonPressed();
		void setisOn();
		bool getisOn();
		void setisOff();
		string getButtonString();
		unsigned short getitemNum();
};

Button::Button()
{
	isOn = 0;
}

void Button::setisOn()
{
	this->isOn = true;
}

bool Button::getisOn()
{
	return this->isOn;
}

void Button::setisOff()
{
	this->isOn = false;
}

unsigned short Button::getitemNum()
{
	return this->itemNum;
}

string Button::getButtonString()
{
	return this->buttonString;
}

Button::Button(unsigned short width, unsigned short heigth, unsigned short itemNum)
{
	this->itemNum = itemNum;
	Item item(itemNum);
	string message;

	if(itemNum != 7)
		message = item.getName();
	else
		message = "";
	this->width = width;
	this->heigth = heigth;
	string str;
	isOn = false;
 	unsigned short wordCount;
	unsigned short counter = 0;
	//boxstring
	string* words = new string[32];
	words = parseString(message, wordCount);
	string aligned = alignedString(width,heigth,words,wordCount,true);

	for(int i = 0; i < heigth; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(i == 0)
					str += "-";

			else if( i != 0 && (j == 0 || j == width - 1) && (i != heigth - 1))
					str += "|";

			else if (i == heigth - 1)
					str += "-";	
			else
			{
				str += charOnLine(aligned,counter);	
			}
		}
	
		str += '\n';
	}

	box_string = str;
	//endofboxstring
	counter = 0;

	string but_str;
	//buttonString
	for(int i = 0; i < heigth; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(i == 0)
					but_str += "=";

			else if( i != 0 && (j == 0 || j == width - 1) && (i != heigth - 1))
					but_str += "X";

			else if (i == heigth - 1)
					but_str += "=";	
			else
			{
				but_str += charOnLine(aligned,counter);	
			}
		}
	
		but_str += '\n';
	}

	buttonString = but_str;
}

