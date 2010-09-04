/*
 * GameOverState.h
 *
 *  Created on: 25 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef GAMEOVERSTATE_H_
#define GAMEOVERSTATE_H_

#include "AbstractGameState.h"
#include "PlayState.h"
#include "IntroState.h"
#include <map>

/**
 * GameOver Scene
 */
class GameOverState: public AbstractGameState {
public:
	GameOverState();
	virtual ~GameOverState();

	void draw(SDL_Surface* surface);
	void keyPressed(SDLKey key);
	void mousePressed(const int button, const int mouseX, const int mouseY);
	/**
	 * Highscore list
	 */
	std::map<int, std::map<int, std::string> > highscore;
	/**
	 * Username
	 */
	std::string saveName;

private:
	/**
	 * Save highscore
	 */
	void save();
	/**
	 * User highscore
	 */
	int lowestPoints;
};

#endif /* GAMEOVERSTATE_H_ */
