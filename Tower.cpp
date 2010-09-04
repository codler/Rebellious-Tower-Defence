/*
 * Tower.cpp
 *
 *  Created on: 30 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "Tower.h"

using namespace std;
Tower::Tower(TowerType type) {
	this->price = type;
	this->type = type;
	this->fired = false;

	switch (this->type) {
	case ACID:
		this->areaOfEffect = 10;
		this->damage = 40;
		this->recoil = 30;
		this->fuel = 150;
		this->scope
				= static_cast<int> (pow(static_cast<double> (this->fuel), 2));
		this->speed = 16;
		break;
	case ICE:
		this->areaOfEffect = 45;
		this->damage = 50;
		this->recoil = 30;
		this->fuel = 150;
		this->scope
				= static_cast<int> (pow(static_cast<double> (this->fuel), 2));
		this->speed = 20;
		break;
	case STONE:
		this->areaOfEffect = 90;
		this->damage = 40;
		this->recoil = 40;
		this->fuel = 140;
		this->scope
				= static_cast<int> (pow(static_cast<double> (this->fuel), 2));
		this->speed = 10;
		break;
	case SUPERIOR:
		this->areaOfEffect = 10;
		this->damage = 30;
		this->recoil = 10;
		this->fuel = 170;
		this->scope
				= static_cast<int> (pow(static_cast<double> (this->fuel), 2));
		this->speed = 20;
		break;
	default:
		break;
	}
}

Tower::~Tower() {
}

void Tower::clockTick() {
	if (this->fired) {
		// tick cooldown
		this->coolDown--;
		if (this->coolDown <= 0) {
			this->fired = false;
		}
	}
}

void Tower::draw(SDL_Surface* surface, SDL_Surface* imageTower, bool circle) {
	SDL_Rect rect;
	rect.x = this->posX;
	rect.y = this->posY;
	SDL_BlitSurface(imageTower, NULL, surface, &rect);

	if (circle == true) {
		Uint32 color = SDL_MapRGBA(surface->format, 0, 255, 255, 127);
		circleColor(surface, this->posX, this->posY, this->fuel, color);
	}
}

bool Tower::fire(int x, int y) {
	// check range
	if (static_cast<int> (pow(static_cast<double> (this->posX - x), 2))
			+ static_cast<int> (pow(static_cast<double> (this->posY - y), 2))
			<= this->scope) {
		// if not fired , fire!
		if (!this->fired) {
			this->fired = true;
			this->coolDown = this->recoil;
			return true;
		}
	}
	return false;
}

void Tower::setPos(int x, int y) {
	this->posX = x;
	this->posY = y;
}
