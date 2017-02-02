/*
TEAM FRANKLIN
*/
#include "IShape.h"

IShape::IShape()
{
	x = rand() % 787 + 1;
	y = rand() % 587 + 1;
	doesExist = true;
	xVel = (rand() % 7) - 3;
	yVel = (rand() % 7) - 3;
	isCirc = false;
	isTri = false;
	isSqr = false;
}

IShape::~IShape() {}
void IShape::setX(int n) { x = n; }// set x pos
void IShape::setY(int n) { y = n; }// set y pos
void IShape::setExist(bool b) { doesExist = b; }// set existance
void IShape::setVel(int xv, int yv) {// set velocity
	if (xv == 0)
		xv = 1;
	if (yv == 0)
		yv == 1;
	xVel = xv;
	yVel = yv;
}

// Sets new velocity for collision with walls
void IShape::updateVel(int n)
{
	srand(time(NULL));
	if (n == 0)
		setVel((rand() % 3 + 1), (rand() % 7) - 3);
	if (n == 1)
		setVel((rand() % 7) - 3, (rand() % 3 + 1));
	if (n == 2)
		setVel(-1 * (rand() % 3 + 1), (rand() % 7) - 3);
	if (n == 3)
		setVel((rand() % 7) - 3, -1 * (rand() % 3 + 1));
}


