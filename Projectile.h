#pragma once
#include "IShape.h"
class Projectile {

public:

	Projectile();
	Projectile(IShape *  s);
	int getVX();
	int getVY();
	int getPX();
	int getPY();
	void setVX(int n);
	void setVY(int n);
	void setPX(int n);
	void setPY(int n);


private:
	int vx, vy, px, py;





};