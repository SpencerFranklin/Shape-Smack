/*
TEAM FRANKLIN
*/
#include "Circle.h"

Circle::~Circle() {}

Circle::Circle() {
	setX( rand() % 787 + 1);
	setY( rand() % 587 + 1);
	setExist(true);
	setVel((rand() % 7) - 3, (rand() % 7) - 3 );
	isCirc = true;
}

//Get X position
int Circle::getX() {
	return x;
}

// Get Y position
int Circle::getY() {
	return y;
}

bool Circle::doesShExist() {
	return doesExist;
}

// Get X velocity
int Circle::getXV() {
	return xVel;
}

// Get Y velocity
int Circle::getYV() {
	return yVel;
}

