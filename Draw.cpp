/*
 * Draw.cpp
 *
 *  Created on: 1 dec 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "Draw.h"

using namespace std;

Draw::Draw() {
}

Draw::~Draw() {
}

void Draw::scroll(SDL_Surface* surface, SDL_Color color, const int x,
		const int y, const int w, const int h, const int scrollOffset,
		const int size, const int alpha) {

	const int rowHeight = 30;
	const int scrollbarHeight = h - 20;

	// upper scroll-arrow
	filledTrigonRGBA(surface, x + w / 2, y, x, y + 10, x + w, y + 10, color.r,
			color.g, color.b, alpha);

	// lower scroll-arrow
	filledTrigonRGBA(surface, x + w / 2, y + h, x, y + h - 10, x + w, y + h
			- 10, color.r, color.g, color.b, alpha);

	// scrollbar
	if (size > scrollbarHeight / rowHeight) {
		boxRGBA(surface, x + w / 4, (y + 15) + scrollbarHeight / size
				* ((-scrollOffset) / rowHeight), x + w - w / 4, (y + 15) + (scrollbarHeight / size
				* 10) + scrollbarHeight / size * ((-scrollOffset) / rowHeight), color.r, color.g,
				color.b, alpha);
	} else {
		boxRGBA(surface, x + w / 4, y + 15, x + w - w / 4, y + scrollbarHeight, color.r,
				color.g, color.b, alpha);
	}

}
/**
 * Write Text on screen.
 */
void Draw::text(SDL_Surface* surface, const string text, SDL_Color color,
		const int x, const int y, const int size, const int alpha) {
	TTF_Init();
	// Load font
	TTF_Font *font = TTF_OpenFont(Setting::FONT, size);
	// Set font, text and color
	SDL_Surface *surfaceText;
	if (alpha < 255) {
		surfaceText = TTF_RenderText_Solid(font, text.c_str(), color);
	} else {
		surfaceText = TTF_RenderText_Blended(font, text.c_str(), color);
	}
	// Set position
	SDL_Rect rectText = { x, y, 0, 0 };
	// Set alpha
	if (alpha < 255) {
		SDL_SetAlpha(surfaceText, SDL_RLEACCEL | SDL_SRCALPHA, alpha);
	}
	// Set to surface
	SDL_BlitSurface(surfaceText, NULL, surface, &rectText);
	SDL_FreeSurface(surfaceText);
	TTF_CloseFont(font);

}

