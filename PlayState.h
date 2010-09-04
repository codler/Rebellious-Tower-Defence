/*
 * PlayState.h
 *
 *  Created on: 24 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef PLAYSTATE_H_
#define PLAYSTATE_H_

#include "AbstractGameState.h"
#include "Rebel.h"
#include "Tower.h"
#include "Round.h"
#include "GameEngine.h"
#include "GameOverState.h"
#include "Shot.h"
/**
 * Play scene
 */
class PlayState: public AbstractGameState {
public:
	PlayState();
	/**
	 * Deallocating memory
	 */
	virtual ~PlayState();

	/**
	 *  Makes the game to go forward.
	 */
	void clockTick();
	/**
	 * Draw all objects on screen.
	 */
	void draw(SDL_Surface* surface);
	void keyPressed(SDLKey key);
	void mouseMoved(const int button, const int mouseX, const int mouseY);
	void mousePressed(const int button, const int mouseX, const int mouseY);
	/**
	 * Collision detection
	 */
	static const bool intersect(const double x1, const double y1, const int x2,
			const int y2, const int aoe1, const int aoe2);

private:
	std::vector<Rebel*> rebels;
	std::vector<Round> roundItem;
	std::vector<Shot*> shots;
	std::vector<Tower*> towers;
	Tower* buyTower;
	/**
	 * User wealth (stone)
	 */
	int holding;
	int life;
	/**
	 * Ticks before next round begins
	 */
	int nextRebel;
	bool pause;
	TowerType marketChoice;

	SDL_Surface* imageAcid;
	SDL_Surface* imageIce;
	SDL_Surface* imageStone;
	SDL_Surface* imageSuperior;

	SDL_Surface* imageMightyOutcast;
	SDL_Surface* imageOutlaw;
	SDL_Surface* imageRoyalKatanaMen;

};

#endif /* PLAYSTATE_H_ */
