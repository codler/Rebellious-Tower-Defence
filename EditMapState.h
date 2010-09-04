/*
 * EditMapState.h
 *
 *  Created on: 27 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef EDITMAPSTATE_H_
#define EDITMAPSTATE_H_

#include "AbstractGameState.h"
#include "IntroState.h"
#include "Map.h"

/**
 * Allow you to create map.
 */
class EditMapState: public AbstractGameState {
public:
	EditMapState();
	virtual ~EditMapState();

    void draw(SDL_Surface* surface);
    void keyPressed(SDLKey key);
    void mouseMoved(const int button,const  int mouseX,const  int mouseY);
    void mousePressed(const int button, const int mouseX, const int mouseY);

private:
    void save();
    Map* map;
	std::string mapName;
	int choice;
	bool saveMap;
};

#endif /* EDITMAPSTATE_H_ */
