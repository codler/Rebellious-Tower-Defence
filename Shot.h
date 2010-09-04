/*
 * Shot.h
 *
 *  Created on: 1 dec 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef SHOT_H_
#define SHOT_H_

#include "Tower.h"

/**
 * Properties on the bullet.
 */
class Shot {
public:
	Shot(Tower* tower, const double angle);
	virtual ~Shot();

	void move();
	void draw(SDL_Surface* surface);

	double posX;
	double posY;
	int areaOfEffect;
	int damage;
	int fuel;
	bool hasHit;
	TowerType speciality;

private:
	SDL_Surface* image;
	double angle;
	int speed;
};

#endif /* SHOT_H_ */
