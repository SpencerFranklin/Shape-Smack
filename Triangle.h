/*
TEAM FRANKLIN
*/
#pragma once
#include "IShape.h"
 
class Triangle: public IShape{

public:
	virtual ~Triangle();
	Triangle();
	virtual int IShape::getX();// get x pos
	virtual int IShape::getY();// get y pos
	virtual bool IShape::doesShExist();
	virtual int IShape::getXV();// get x vel
	virtual int IShape::getYV();// get y vel
};