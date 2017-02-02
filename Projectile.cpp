# include "Projectile.h"

Projectile::Projectile()
{
	px = 0;
	py = 0;
	vx = 0;
	vy = 0;
}

Projectile::Projectile(IShape * s)
{
	px = s->getX() + 5;
	py = s->getY() + 5;
	vx = s->getXV() * -1;
	vy = s->getYV() * -1;
}

int Projectile::getVX()
{
	return vx;
}

int Projectile::getVY()
{
	return vy;
}

int Projectile::getPX()
{
	return px;
}

int Projectile::getPY()
{
	return py;
}

void Projectile::setVX(int n)
{
	vx = n;
}

void Projectile::setVY(int n)
{
	vy = n;
}

void Projectile::setPX(int n)
{
	px = n;
}

void Projectile::setPY(int n)
{
	py = n;
}
