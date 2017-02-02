/*
TEAM FRANKLIN
*/
#pragma once
#include "IShape.h"

class Circle: public IShape {

public:
	virtual ~Circle();
	Circle();
	virtual int IShape::getX();// Get x position
	virtual int IShape::getY();// Get y position
	virtual bool IShape::doesShExist();
	virtual int IShape::getXV();// Get x velocity
	virtual int IShape::getYV();// Get y velocity
};