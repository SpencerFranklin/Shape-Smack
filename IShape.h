/*
TEAM FRANKLIN
*/
#pragma once
#include <math.h>
#include <stdlib.h>
#include <ctime>

// Abstract IShape class
class IShape {

public:
	IShape();
	virtual ~IShape() = 0;
	virtual int getX() = 0;
	virtual int getY() = 0;
	void setX(int n);
	void setY(int n);
	virtual bool doesShExist() = 0;
	void setExist(bool b);
	
	void setVel(int xv, int yv);
	virtual int getXV() = 0;
	virtual int getYV() = 0;
	void updateVel(int n);// Used for collisions with sides of window
	bool isCirc, isTri, isSqr;// shape type

protected:
	int x, y;// position
	bool doesExist;// existance
	float xVel, yVel;// velocity
	
};