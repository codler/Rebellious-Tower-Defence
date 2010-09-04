/*
 * AbstractGameState.h
 *
 *  Created on: 24 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef ABSTRACTGAMESTATE_H_
#define ABSTRACTGAMESTATE_H_

#include <dirent.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "Draw.h"
#include "Map.h"

/**
 * Scene handler
 */
class AbstractGameState {
public:
	AbstractGameState();
	virtual ~AbstractGameState();

	virtual void clockTick();
	virtual void draw(SDL_Surface* surface);
	virtual void keyPressed(SDLKey key);
	/**
	 * Check if mouse hit a specific area.
	 */
	bool mouseHit(const int mouseX, const int mouseY, const int x,
			const int y, const int w, const int h);
	virtual void mouseMoved(const int button, const int mouseX,
			const int mouseY);
	virtual void mousePressed(const int button, const int mouseX,
			const int mouseY);
	/**
	 * Convert Int to String.
	 */
	static const std::string intToString(const int number);
	/**
	 * Get a list of files in a directory.
	 */
	static const std::vector<std::string> getFilesInDir(const std::string dir);

	AbstractGameState* state;
	SDL_Event event;
	Map* map;
	std::string mapLoaded;
	int myScore;
	unsigned int round;
	bool done;

protected:
	SDL_Surface* background;
	bool reDraw;
};

#endif /* ABSTRACTGAMESTATE_H_ */
