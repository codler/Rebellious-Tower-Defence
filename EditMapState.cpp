/*
 * EditMapState.cpp
 *
 *  Created on: 27 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "EditMapState.h"

using namespace std;

EditMapState::EditMapState() {
	this->background = IMG_Load(Setting::BACKGROUND_PLAY.c_str());
	if (!this->background) {
		cerr << "Image could not load!";
	}
	this->map = new Map(26, 23);
	this->map->loadFile(Setting::DEFAULT_MAP);

	this->choice = 0;
	this->saveMap = false;
}

EditMapState::~EditMapState() {
	if (NULL != this->background) {
		SDL_FreeSurface(this->background);
		this->background = NULL;
	}
	delete map;
	map = NULL;
}

void EditMapState::draw(SDL_Surface* surface) {
	// Text color black
	SDL_Color blackColor = { 0, 0, 0, 0 };
	// Text color white
	SDL_Color whiteColor = { 255, 255, 255, 0 };
	// input color , red
	SDL_Color inputColor = { 159, 16, 16, 0 };

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	SDL_BlitSurface(this->background, NULL, surface, &rect);

	// when save map
	if (this->saveMap) {
		boxRGBA(surface, 200, 120, 540, 300, 150, 200, 150, 255);
		Draw::text(surface, "Write map name", whiteColor, 200, 110, 50);
		Draw::text(surface, this->mapName, inputColor, 210, 170, 30);
		Draw::text(surface, "save map", whiteColor, 200, 200, 50);
		return;
	}

	// At upper right
	Draw::text(surface, "Information", whiteColor, 825, 23, 32, 100);
	Draw::text(surface, "Your choice:", whiteColor, 825, 80, 20);
	rect.x = 930;
	rect.y = 80;
	if (this->choice == 0) {
		SDL_BlitSurface(this->map->tile[17], NULL, surface, &rect);
	} else {
		SDL_BlitSurface(this->map->tile[this->choice], NULL, surface, &rect);
	}
	Draw::text(surface, "Save Map", blackColor, 840, 150);
	Draw::text(surface, "Go to Startmenu", blackColor, 825, 220, 24);
	// At middle right

	Draw::text(surface, "Choose", blackColor, 830, 290, 50, 100);
	int marketTowerX = 800;
	int marketTowerY = 350;
	rect.x = marketTowerX;
	rect.y = marketTowerY;
	//road
	SDL_BlitSurface(this->map->tile[1], NULL, surface, &rect);
	//Defalt image
	rect.x = marketTowerX + 50;
	SDL_BlitSurface(this->map->tile[17], NULL, surface, &rect);
	//Startpoint
	rect.x = marketTowerX + 100;
	SDL_BlitSurface(this->map->tile[8], NULL, surface, &rect);
	//endpoint
	rect.x = marketTowerX + 150;
	SDL_BlitSurface(this->map->tile[9], NULL, surface, &rect);

	//box
	rect.x = marketTowerX;
	rect.y = marketTowerY + 40;
	SDL_BlitSurface(this->map->tile[11], NULL, surface, &rect);
	//Christmastree
	rect.x = marketTowerX + 50;
	SDL_BlitSurface(this->map->tile[6], NULL, surface, &rect);
	//Tree
	rect.x = marketTowerX + 100;
	SDL_BlitSurface(this->map->tile[10], NULL, surface, &rect);
	//grass
	rect.x = marketTowerX + 150;
	SDL_BlitSurface(this->map->tile[7], NULL, surface, &rect);

	//fire
	rect.x = marketTowerX;
	rect.y = marketTowerY + 80;
	SDL_BlitSurface(this->map->tile[12], NULL, surface, &rect);
	//water
	rect.x = marketTowerX + 50;
	SDL_BlitSurface(this->map->tile[13], NULL, surface, &rect);
	//stone
	rect.x = marketTowerX + 100;
	SDL_BlitSurface(this->map->tile[14], NULL, surface, &rect);
	//RIP stone
	rect.x = marketTowerX + 150;
	SDL_BlitSurface(this->map->tile[15], NULL, surface, &rect);

	//tree2
	rect.x = marketTowerX;
	rect.y = marketTowerY + 120;
	SDL_BlitSurface(this->map->tile[16], NULL, surface, &rect);

	this->map->draw(surface);
}

void EditMapState::keyPressed(SDLKey key) {
	if (this->saveMap) {
		int ascii = this->event.key.keysym.unicode;
		// Only allow A-Z,a-z letters and number 0-9
		if ((ascii >= 48 && ascii <= 57) || (ascii >= 65 && ascii <= 90)
				|| (ascii >= 97 && ascii <= 122) || SDLK_SPACE == key) {
			this->mapName += (char) ascii;
		} else if (SDLK_BACKSPACE == key) {
			// Erase a letter in name
			this->mapName = this->mapName.substr(0, mapName.length() - 1);
		} else if (SDLK_RETURN == key) {
			// Save name when press return
			save();
		}
	}

	if (SDLK_ESCAPE == key) {
		this->done = true;
	}
}

void EditMapState::mouseMoved(const int button, const int mouseX,
		const int mouseY) {
	if (button == 1) {
		mousePressed(button, mouseX, mouseY);
	}
}

void EditMapState::mousePressed(const int button, const int mouseX,
		const int mouseY) {

	if (!this->saveMap) {
		if (this->map->mouseOver(mouseX, mouseY)) {
			if (this->choice != 8 && this->choice != 9) {
				int mapPosition = this->map->pixelToPos(mouseX, mouseY);
				this->map->setTile(this->choice, this->map->tileToPosX(
						mapPosition) + 1, this->map->tileToPosY(mapPosition) + 1);

			} else if (this->choice == 8) {
				this->map->startPoint = this->map->pixelToPos(mouseX, mouseY);
			} else if (this->choice == 9) {
				this->map->endPoint = this->map->pixelToPos(mouseX, mouseY);
			}
		}
	}

	if (mouseHit(mouseX, mouseY, 800, 350, 30, 30)) {
		this->choice = 1;
	} else if (mouseHit(mouseX, mouseY, 850, 350, 30, 30)) {
		this->choice = 0;
	} else if (mouseHit(mouseX, mouseY, 900, 350, 30, 30)) {
		this->choice = 8;
	} else if (mouseHit(mouseX, mouseY, 950, 350, 30, 30)) {
		this->choice = 9;

	} else if (mouseHit(mouseX, mouseY, 800, 390, 30, 30)) {
		this->choice = 11;
	} else if (mouseHit(mouseX, mouseY, 850, 390, 30, 30)) {
		this->choice = 6;
	} else if (mouseHit(mouseX, mouseY, 900, 390, 30, 30)) {
		this->choice = 10;
	} else if (mouseHit(mouseX, mouseY, 950, 390, 30, 30)) {
		this->choice = 7;

	} else if (mouseHit(mouseX, mouseY, 800, 430, 30, 30)) {
		this->choice = 12;
	} else if (mouseHit(mouseX, mouseY, 850, 430, 30, 30)) {
		this->choice = 13;
	} else if (mouseHit(mouseX, mouseY, 900, 430, 30, 30)) {
		this->choice = 14;
	} else if (mouseHit(mouseX, mouseY, 950, 430, 30, 30)) {
		this->choice = 15;

	} else if (mouseHit(mouseX, mouseY, 800, 470, 30, 30)) {
		this->choice = 16;
	}

	if (mouseHit(mouseX, mouseY, 840, 150, 150, 33)) {
		this->saveMap = true;
	} else if (mouseHit(mouseX, mouseY, 825, 220, 200, 26)) {
		AbstractGameState* state = new IntroState();
		this->state = state;
	}

	if (this->saveMap) {
		if (mouseHit(mouseX, mouseY, 200, 200, 200, 50)) {
			save();
		}
	}
}

void EditMapState::save() {
	// Save the new map
	string map = Setting::MAP_DIR + this->mapName + ".txt";
	this->map->save(map);
	//return to IntroState
	AbstractGameState* state = new IntroState();
	this->state = state;
}
