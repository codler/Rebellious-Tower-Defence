/*
 *	GameEngine.h
 *
 *  Created on: 23 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef GameEngine_h
#define GameEngine_h

#include "AbstractGameState.h"

/**
 * Initiate SDL, run game and listen to events (keyboard and mouse).
 */
class GameEngine {

public:
	/**
	 * Initiate SDL and GUI
	 */
	GameEngine(const int width, const int height, const bool fullscreen);
	~GameEngine();

	/**
	 * Available states: IntroState, PlayState, GameOverState, EditMapState
	 */
	void setState(AbstractGameState* state);
	/**
	 * TickInterval is 1000 / tick = FPS (frame per second)
	 */
	void setTickInterval(int interval);
	/**
	 * Run the game
	 */
	void loop();

private:
	AbstractGameState* state;
	SDL_Surface* screen;
	bool done;
	int tickInterval;
};

#endif // GameEngine_h
