/*
 * Draw.h
 *
 *  Created on: 1 dec 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef DRAW_H_
#define DRAW_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <string>
#include "Setting.h"

/**
 * Handy draw class
 */
class Draw {
public:
	Draw();
	virtual ~Draw();

	/**
	 * Draw a scrollbar
	 */
	static void scroll(SDL_Surface* surface, SDL_Color color, const int x = 0,
			const int y = 0, const int w = 30, const int h = 320,
			const int scrollOffset = 0, const int size = 10, const int alpha =
					255);
	/**
	 * Print text on screen.
	 * Set color, position, size, alpha.
	 */
	static void text(SDL_Surface* surface, const std::string text,
			SDL_Color color, const int x = 0, const int y = 0, const int size =
					30, const int alpha = 255);

};

#endif /* DRAW_H_ */
