/*
TEAM FRANKLIN
*/
#include "Player.h"

// get x position
int Player::getX() {
	return pX;
}

// get y position
int Player::getY() {
	return pY;
}

// set x position
void Player::setX(int n) {
	pX = n;
}

// set y position
void Player::setY(int n) {
	pY = n;
}

// get player shape
int Player::getState() {
	return pState;
}

// set player shape
void Player::setState(int n) {
	pState = (state)n;
}

// trigger new shape
void Player::newState() {
	int t = rand() % 3;
	pState = (state)t;
}

// set score
void Player::setScore(int n){
	if (n < 0)
		n = 0;
	score = n;
}

// get score
int Player::getScore() {
	return score;
}

// get shape width
int Player::getShapeW() {
	return shapeSize;
}

// set shape width
void Player::setShapeW(int n) {
	shapeSize = n;
}

// set wave
void Player::setWave(int n) {
	wave = n;
}

// get wave
int Player::getWave() {
	return wave;
}