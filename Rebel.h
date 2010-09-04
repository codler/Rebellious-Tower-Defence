/*
 * Rebel.h
 *
 *  Created on: 27 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef REBEL_H_
#define REBEL_H_

#include <cmath>
#include <vector>
#include "Draw.h"
#include "Round.h"

/**
 * Rebel (Mighty Outcast, Outlaw, Royal Katana Men)
 */
class Rebel {
public:
	Rebel(Round prop, int x, int y);
	virtual ~Rebel();

	void draw(SDL_Surface* surface, SDL_Surface* imageRebel);
	void move(std::vector<int> environment);

	int posX;
	int posY;
	int value;
	double health;
	double maxHealth;
	double speed;
	double poison;
	RebelType type;

private:
	SDL_Surface* image;
	int direction;
	int nextDirection;
	int previousDirection;
};

#endif /* REBEL_H_ */
