/*
TEAM FRANKLIN
*/
#include "Square.h"

Square::~Square() {}

Square::Square() {
	setX(rand() % 787 + 1);
	setY(rand() % 587 + 1);
	setExist(true);
	setVel((rand() % 7) - 3, (rand() % 7) - 3);
	isSqr = true;;
}

// get x pos
int Square::getX() {
	return x;
}

// get y pos
int Square::getY() {
	return y;
}

bool Square::doesShExist() {
	return doesExist;
}

// get x vel
int Square::getXV() {
	return xVel;
}

// get y vel
int Square::getYV() {
	return yVel;
}
