/*
 * Tower.h
 *
 *  Created on: 30 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef TOWER_H_
#define TOWER_H_

#include <cmath>
#include "Draw.h"
#include "Map.h"

/**
 * Tower (ACID, ICE, STONE, SUPERIOR)
 */
class Tower {
public:
	Tower(TowerType type);
	virtual ~Tower();

	void clockTick();
	void draw(SDL_Surface* surface, SDL_Surface* imageTower, bool circle =
			false);
	bool fire(int x, int y);
	void setPos(int x, int y);
	int posX;
	int posY;
	int price;
	int coolDown;
	int damage;
	int scope;
	int fuel;
	int areaOfEffect;
	int speed;
	TowerType type;

private:
	int recoil;
	bool fired;
};

#endif /* TOWER_H_ */
