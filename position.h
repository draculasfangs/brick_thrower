#include <iostream>
using namespace std;

#ifndef POSITION_H
#define POSITION_H

class Position
{
	unsigned short x, y;

	public:
		Position(); 
		Position(unsigned short,unsigned short);
		unsigned short getX();
		unsigned short getY();
		

		void setX(unsigned short);
		void setY(unsigned short);
		

		Position& operator = (const Position&);
		Position operator + (const Position&);	
		Position operator - (const Position&);	
		void operator += (const Position&);

	//	bool operator == (int, int);
		bool isEqual(unsigned short,unsigned short);
		void setPos(unsigned short, unsigned short);

};

Position::Position()
{
	x = 0;
	y = 0;

}

Position::Position(unsigned short x,unsigned short y)
{
	this->x = x;
	this->y = y;

}

void Position::setPos(unsigned short x, unsigned short y)
{
	this->x = x;
	this->y = y;
}

unsigned short Position::getX()
{
	return this->x;
}

unsigned short Position::getY()
{
	return this->y;
}

void Position::setX(unsigned short x) 
{
	this->x = x;
}
void Position::setY(unsigned short y)
{
	this->y = y;
}
	
Position& Position::operator = (const Position& pos)
{
	this->x = pos.x;
	this->y = pos.y;
	return *this;
}

Position Position::operator + (const Position& pos)
{
	Position temp;
	temp.x = x + pos.x;
	temp.y = y + pos.y;
	return temp;
}

Position Position::operator - (const Position& pos)
{
	Position temp;
	temp.x = x - pos.x;
	temp.y = y - pos.y;
	return temp;
}
		
void Position::operator += (const Position& pos)
{
	this->x += pos.x;
	this->y += pos.y;
}


bool Position::isEqual(unsigned short x, unsigned short y)
{
	if ((this->x == x) && (this->y == y))
		return 1;
	return 0;
}

#endif
