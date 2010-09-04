/*
 * IntroState.h
 *
 *  Created on: 24 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef INTROSTATE_H_
#define INTROSTATE_H_

#include "AbstractGameState.h"
#include "EditMapState.h"
#include "GameEngine.h"
#include "PlayState.h"

/**
 * Intro Scene
 */
class IntroState: public AbstractGameState {
public:
	IntroState();
	virtual ~IntroState();

	void draw(SDL_Surface* surface);
	void keyPressed(SDLKey key);
	void mousePressed(const int button, const int mouseX, const int mouseY);

private:
	/**
	 * Scroll map list
	 */
	int scrollOffset;
	MenuType menuChoice;
};

#endif /* INTROSTATE_H_ */
