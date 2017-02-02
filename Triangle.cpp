/*
TEAM FRANKLIN
*/
#include "Triangle.h"

Triangle::~Triangle() {}

Triangle::Triangle() {
	setX(rand() % 787 + 1);
	setY(rand() % 587 + 1);
	setExist(true);
	setVel((rand() % 7) - 3, (rand() % 7) - 3);
	isTri = true;
}

// get x pos
int Triangle::getX() {
	return x;
}

// get y pos
int Triangle::getY() {
	return y;
}

bool Triangle::doesShExist() {
	return doesExist;
}

// get x velocity
int Triangle::getXV() {
	return xVel;
}

// get y velocity
int Triangle::getYV() {
	return yVel;
}