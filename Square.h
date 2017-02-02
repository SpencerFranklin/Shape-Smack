/*
TEAM FRANKLIN
*/
#pragma once
#include "IShape.h"

class Square : public IShape {

public:
	virtual ~Square();
	Square();
	virtual int IShape::getX();// get x pos
	virtual int IShape::getY();// get y pos
	virtual bool IShape::doesShExist();
	virtual int IShape::getXV();// get x vel
	virtual int IShape::getYV();// get y vel
};
