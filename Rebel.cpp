/*
 * Rebel.cpp
 *
 *  Created on: 27 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "Rebel.h"

using namespace std;

Rebel::Rebel(Round prop, int x, int y) {
	this->posX = x;
	this->posY = y;

	this->nextDirection = 0;
	this->direction = 1;
	this->poison = 0;
	this->type = prop.type;

	this->value = prop.value;
	this->health = prop.health;
	this->maxHealth = prop.health;
	this->speed = prop.speed;
}

Rebel::~Rebel() {
}

void Rebel::draw(SDL_Surface* surface, SDL_Surface* imageRebel) {
	SDL_Rect rect;
	rect.x = this->posX;
	rect.y = this->posY;
	SDL_BlitSurface(imageRebel, NULL, surface, &rect);

	// Healthbar
	if (this->poison > 0) {
		// green color
		boxRGBA(surface, this->posX, this->posY, this->posX + (20
				/ this->maxHealth * this->health), this->posY + 2, 100, 255,
				50, (rand() % 200 + 50));
	} else {
		// red color
		boxRGBA(surface, this->posX, this->posY, this->posX + (20
				/ this->maxHealth * this->health), this->posY + 2, 255, 50, 50,
				255);
	}
}

void Rebel::move(vector<int> environment) {
	// minimum speed
	if (this->speed < 1) {
		this->speed = 1;
	}

	if (this->direction == 1) {
		environment[2] = 0;
	} else if (this->direction == 2) {
		environment[3] = 0;
	} else if (this->direction == 3) {
		environment[0] = 0;
	} else if (this->direction == 4) {
		environment[1] = 0;
	}

	if (environment[0] != 1 && environment[1] != 1 && environment[2] != 1
			&& environment[3] != 1) {
		if (this->direction == 1) {
			this->direction = 3;
		} else if (this->direction == 2) {
			this->direction = 4;
		} else if (this->direction == 3) {
			this->direction = 1;
		} else if (this->direction == 4) {
			this->direction = 2;
		}

	}

	if (floor(static_cast<double> (this->posX % 30)) == 0 && floor(
			static_cast<double> (this->posY % 30)) == 0) {
		vector<int> availableDirection;
		for (unsigned int i = 0; i < 4; i++) {
			if (environment[i] == 1) {
				availableDirection.push_back(i + 1);
			}
		}
		if (availableDirection.size() > 1) {
			int randomDirection = rand() % availableDirection.size();
			this->direction = availableDirection[randomDirection];
		} else {
			// up
			if (environment[0] == 1) {
				this->direction = 1;
				// right
			} else if (environment[1] == 1) {
				this->direction = 2;
				// down
			} else if (environment[2] == 1) {
				this->direction = 3;
				// left
			} else if (environment[3] == 1) {
				this->direction = 4;
			}
		}
	}

	if (this->direction == 2) {
		this->posX += this->speed;
	} else if (this->direction == 4) {
		this->posX -= this->speed;
	} else if (this->direction == 3) {
		this->posY += this->speed;
	} else if (this->direction == 1) {
		this->posY -= this->speed;
	}

	// poison
	if (this->poison > 0) {
		this->health -= 0.20;
		this->poison -= 0.20;
	}

}
