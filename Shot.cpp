/*
 * Shot.cpp
 *
 *  Created on: 1 dec 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "Shot.h"

#ifndef M_PI
#define M_PI 3.14159265358
#endif

using namespace std;

Shot::Shot(Tower* tower, const double angle) {
	this->angle = angle;
	this->areaOfEffect = tower->areaOfEffect;
	this->damage = tower->damage;
	this->fuel = tower->fuel;
	// ammo image
	this->image = IMG_Load(Map::getImage(100).c_str());
	this->posX = tower->posX;
	this->posY = tower->posY;
	this->speciality = tower->type;
	this->speed = tower->speed;
	this->hasHit = false;

	int bulletSize = 1;
	if (this->speciality == STONE) {
		bulletSize = 2;
	}
	//Rotate image
	this->image = rotozoomSurface(this->image, 180 - this->angle * 180 / M_PI,
			bulletSize, 1);
}

Shot::~Shot() {
	SDL_FreeSurface(this->image);
}

void Shot::move() {
	this->posX += cos(this->angle) * this->speed;
	this->posY += sin(this->angle) * this->speed;

	this->fuel -= 1 * this->speed;
}

void Shot::draw(SDL_Surface* surface) {
	SDL_Rect rect;
	rect.x = this->posX;
	rect.y = this->posY;
	SDL_BlitSurface(this->image, NULL, surface, &rect);
}
