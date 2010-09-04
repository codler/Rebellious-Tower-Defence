/*
 * IntroState.cpp
 *
 *  Created on: 24 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "IntroState.h"

using namespace std;

IntroState::IntroState() {
	this->background = IMG_Load(Setting::BACKGROUND.c_str());
	if (!this->background) {
		cerr << "Image could not load!";
	}

	this->menuChoice = EMPTY_MENU;
}

IntroState::~IntroState() {
	if (NULL != this->background) {
		SDL_FreeSurface(this->background);
		this->background = NULL;
	}
}

void IntroState::draw(SDL_Surface* surface) {

	// To avoid redrawing every tick when nothing have actually changed.
	if (!this->reDraw) {
		return;
	} else {
		this->reDraw = false;
	}

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	SDL_BlitSurface(this->background, NULL, surface, &rect);

	// white
	SDL_Color color = { 255, 255, 255, 0 };

	// Print out menu
	Draw::text(surface, "Rebellious Tower Defence", color, 50, 50, 100, 100);
	Draw::text(surface, "Play Game", color, 100, 200, 90, 100);
	Draw::text(surface, "Highscore", color, 100, 300, 90, 100);
	Draw::text(surface, "Help", color, 100, 400, 90, 100);
	Draw::text(surface, "Information", color, 100, 500, 90, 100);
	Draw::text(surface, "Exit", color, 100, 600, 90, 100);
	Draw::text(surface, "Make your own map", color, 740, 640, 30, 100);

	if (this->menuChoice == PLAYGAME) {
		// background
		boxRGBA(surface, 520, 180, 950, 590, 0, 0, 0, 100);

		// menu active
		Draw::text(surface, "Play Game", color, 100, 200, 90);
		Draw::text(surface, "Select map to play", color, 650, 200);

		// lookup maps
		vector<string> files = AbstractGameState::getFilesInDir(
				Setting::MAP_DIR);

		// scroll
		if (-this->scrollOffset >= static_cast<int> (files.size()) * 30 - 300
				&& static_cast<int> (files.size()) > 10) {
			this->scrollOffset = -(files.size() * 30 - 300);
		}
		Draw::scroll(surface, color, 890, 250, 20, 320, this->scrollOffset,
				static_cast<int> (files.size()));

		// Filter and show maps
		int x = 250 + this->scrollOffset;
		for (unsigned int i = 0; i < files.size(); i++) {
			if (x >= 250 && x <= 520) {
				size_t mapname_pos = files[i].find_last_of('.');
				string map = files[i].substr(0, mapname_pos);
				Draw::text(surface, map, color, 550, x);
			}
			x += 30;
		}

		//		// upper scroll-arrow
		//		filledTrigonRGBA(surface, 900, 250, 890, 260, 910, 260, 255, 255, 255,
		//				255);
		//
		//		// lower scroll-arrow
		//		filledTrigonRGBA(surface, 900, 570, 890, 560, 910, 560, 255, 255, 255,
		//				255);

		// scroll
		//		if (static_cast<int> (files.size()) > 10) {
		//			boxRGBA(surface, 895, 265 + 300 / files.size()
		//					* ((-this->scrollOffset) / 30), 905, 265 + (300
		//					/ files.size() * 10) + 300 / files.size()
		//					* ((-this->scrollOffset) / 30), 255, 255, 255, 255);
		//		} else {
		//			boxRGBA(surface, 895, 265, 905, 550, 255, 255, 255, 255);
		//		}

	}

	if (this->menuChoice == HIGHSCORE) {
		// background
		boxRGBA(surface, 520, 180, 950, 450, 0, 0, 0, 100);

		// menu active
		Draw::text(surface, "Highscore", color, 100, 300, 90);
		Draw::text(surface, "Highscore list", color, 650, 200);

		ifstream openFile(Setting::HIGHSCORE.c_str());
		string line;
		int pos_y = 250;
		while (!openFile.eof()) {
			getline(openFile, line);
			size_t find_pos = line.find_last_of(',');
			Draw::text(surface, line.substr(0, find_pos), color, 550, pos_y, 24);
			Draw::text(surface, line.substr(find_pos + 1), color, 850, pos_y,
					24);
			pos_y = pos_y + 30;
		}
		openFile.close();
	}

	if (this->menuChoice == HELP) {
		// background
		boxRGBA(surface, 520, 180, 950, 450, 0, 0, 0, 100);

		// menu active
		Draw::text(surface, "Help", color, 100, 400, 90);
		Draw::text(surface, "Help", color, 650, 200);

		Draw::text(surface, "Defend your kingdom from delirious rebels", color,
				550, 250, 24);
		Draw::text(surface, "by building tower around your country!", color,
				550, 280, 24);
		Draw::text(surface, "Each tower have different specialty", color, 550,
				310, 24);
		Draw::text(surface, "You must choose your tower wisely to", color, 550,
				340, 24);
		Draw::text(surface, "be able to kill them!", color, 550, 370, 24);
	}

	if (this->menuChoice == INFORMATION) {
		// background
		boxRGBA(surface, 520, 180, 950, 450, 0, 0, 0, 100);

		// menu active
		Draw::text(surface, "Information", color, 100, 500, 90);
		Draw::text(surface, "Information", color, 650, 200);

		Draw::text(surface, "Proudly made by ", color, 550, 250, 24);
		Draw::text(surface, "Mikeal Andersson and Han Lin Yap", color, 550,
				290, 24);
		Draw::text(surface, "Release: 2009 December", color, 550, 330, 24);
		Draw::text(surface, "Got a great map? Feel free to submit to us!",
				color, 550, 390, 24);
	}

}

void IntroState::keyPressed(SDLKey key) {
	this->reDraw = true;

	if (SDLK_1 == key) {
		this->menuChoice = PLAYGAME;
	}
	if (SDLK_2 == key) {
		this->menuChoice = HIGHSCORE;
	}
	if (SDLK_3 == key) {
		this->menuChoice = HELP;
	}
	if (SDLK_4 == key) {
		this->menuChoice = INFORMATION;
	}
	if (SDLK_5 == key || SDLK_ESCAPE == key) {
		this->done = true;
	}
}

void IntroState::mousePressed(const int button, const int mouseX,
		const int mouseY) {
	this->reDraw = true;

	if (mouseHit(mouseX, mouseY, 100, 200, 350, 90)) {
		this->menuChoice = PLAYGAME;
		this->scrollOffset = 0;
	} else if (mouseHit(mouseX, mouseY, 100, 300, 350, 90)) {
		this->menuChoice = HIGHSCORE;
	} else if (mouseHit(mouseX, mouseY, 100, 400, 250, 90)) {
		this->menuChoice = HELP;
	} else if (mouseHit(mouseX, mouseY, 100, 500, 400, 90)) {
		this->menuChoice = INFORMATION;
	} else if (mouseHit(mouseX, mouseY, 100, 600, 250, 90)) {
		// exit
		this->done = true;
	}

	// Edit map
	if (mouseHit(mouseX, mouseY, 740, 640, 260, 50)) {
		AbstractGameState* state = new EditMapState();
		this->state = state;
	}

	if (this->menuChoice == PLAYGAME) {
		// Map list
		vector<string> files = AbstractGameState::getFilesInDir(
				Setting::MAP_DIR);
		int x = 250 + this->scrollOffset;
		for (unsigned int i = 0; i < files.size(); i++) {
			string map = Setting::MAP_DIR + files[i];
			if (x >= 250 && x <= 520) {
				if (mouseHit(mouseX, mouseY, 550, x, 250, 30)) {
					AbstractGameState* state = new PlayState();
					this->state = state;
					this->state->map->loadFile(map.c_str());
				}
			}
			x += 30;
		}

		// scrollbar
		if (mouseHit(mouseX, mouseY, 890, 250, 20, 10)) {
			if (this->scrollOffset < 0) {
				this->scrollOffset += 30;
			}
		} else if (mouseHit(mouseX, mouseY, 890, 560, 20, 10)) {
			this->scrollOffset -= 30;
		}
	}
}
