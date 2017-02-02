/*
TEAM FRANKLIN
*/
#pragma once
#include <stdlib.h>

// Represents the players shape
enum state { CIR, TRI, SQR };

class Player {
public:
	int getX();// get x pos
	int getY();// get y pos
	void setX(int n);// set x pos
	void setY(int n);// set y pos
	int getState();// get current shape state
	void setState(int n);// set current shape state
	void newState();// trigger new shape state
	int getScore();// get score
	void setScore(int n);// set score
	int getShapeW();// get shape Width
	void setShapeW(int n);// set shape width
	int getWave();// get wave number
	void setWave(int n);// set wave number

private:
	int pX, pY;// x and y position
	state pState;// current shape
	int score;// score
	int shapeSize;// shape width
	int wave;// current wave
};




