/*
 * GameOverState.cpp
 *
 *  Created on: 25 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "GameOverState.h"

using namespace std;

GameOverState::GameOverState() {
	this->background = IMG_Load(Setting::BACKGROUND.c_str());
	if (!this->background) {
		cerr << "Image could not load!";
	}

	// Read highscore
	ifstream openFile(Setting::HIGHSCORE.c_str());
	string line;
	while (!openFile.eof()) {
		getline(openFile, line);
		size_t findPos = line.find_last_of(',');
		stringstream ss(line.substr(findPos + 1));
		ss >> this->lowestPoints;
	}
	openFile.close();

	this->saveName = "";
}

GameOverState::~GameOverState() {
	if (NULL != this->background) {
		SDL_FreeSurface(this->background);
		this->background = NULL;
	}
}

void GameOverState::draw(SDL_Surface* surface) {

	if (!this->reDraw) {
		return;
	}

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	SDL_BlitSurface(this->background, NULL, surface, &rect);
	// white
	SDL_Color whiteColor = { 255, 255, 255, 0 };

	// input color , red
	SDL_Color inputColor = { 159, 16, 16, 0 };

	// Victorious , gold
	SDL_Color victoriousColor = { 218, 189, 52, 0 };

	if (this->round == Setting::ROUNDS) {
		Draw::text(surface, "Victorious", victoriousColor, 100, 60, 90);
	} else {
		Draw::text(surface, "Game Over", whiteColor, 100, 60, 90);
	}
	if (this->myScore != -1) {
		Draw::text(surface, "Your score: " + AbstractGameState::intToString(
				this->myScore), whiteColor, 100, 150, 40);
	}
	if (this->myScore > this->lowestPoints) {
		// Ask users name
		Draw::text(surface, "Enter your name", whiteColor, 100, 200, 90);
		Draw::text(surface, this->saveName, inputColor, 100, 300, 60);
		Draw::text(surface, "Save", whiteColor, 100, 400, 90);
	} else {
		// Print out menu
		Draw::text(surface, "Play Again", whiteColor, 100, 400, 90);
		Draw::text(surface, "Go to Startmenu", whiteColor, 100, 500, 90);

		Draw::text(surface, "Highscore list", whiteColor, 650, 200);

		// background for highscore
		boxRGBA(surface, 520, 180, 950, 450, 0, 0, 0, 100);
		// Print out highscore
		ifstream openFile(Setting::HIGHSCORE.c_str());
		string line;
		int posY = 250;
		while (!openFile.eof()) {
			getline(openFile, line);
			size_t findPos = line.find_last_of(',');
			Draw::text(surface, line.substr(0, findPos), whiteColor, 550, posY,
					24);
			Draw::text(surface, line.substr(findPos + 1), whiteColor, 850,
					posY, 24);
			posY = posY + 30;
		}
		openFile.close();
	}

	this->reDraw = false;
}

void GameOverState::keyPressed(SDLKey key) {

	this->reDraw = true;

	if (this->myScore >= this->lowestPoints) {
		int ascii = this->event.key.keysym.unicode;
		// Only allow A-Z,a-z letters and numbers 1-9
		if ((ascii >= 48 && ascii <= 57) || (ascii >= 65 && ascii <= 90)
				|| (ascii >= 97 && ascii <= 122) || SDLK_SPACE == key) {
			this->saveName += (char) ascii;
		} else if (SDLK_BACKSPACE == key) {
			// Erase a letter in name
			this->saveName = this->saveName.substr(0, saveName.length() - 1);
		} else if (SDLK_RETURN == key) {
			// Save name when press return
			save();
		}
	}

	if (SDLK_ESCAPE == key) {
		this->done = true;
	}
}

void GameOverState::mousePressed(const int button, const int mouseX,
		const int mouseY) {

	this->reDraw = true;

	if (this->myScore > this->lowestPoints) {
		if (mouseHit(mouseX, mouseY, 100, 400, 350, 90)) {
			save();
		}
	} else {
		// Play again
		if (mouseHit(mouseX, mouseY, 100, 400, 350, 90)) {
			AbstractGameState* state = new PlayState();
			this->state = state;
			this->state->map->loadFile(this->mapLoaded.c_str());
			// go to start
		} else if (mouseHit(mouseX, mouseY, 100, 500, 600, 90)) {
			AbstractGameState* state = new IntroState();
			this->state = state;
		}
	}
}

/**
 * Save highscore
 */
void GameOverState::save() {
	// Write highscore
	ifstream openFile(Setting::HIGHSCORE.c_str());
	vector<string> lines;
	while (!openFile.eof()) {
		string line;
		getline(openFile, line);
		lines.push_back(line);
	}
	openFile.close();

	ofstream writeFile(Setting::HIGHSCORE.c_str());
	int points = 0;
	for (unsigned int i = 0; i <= 4; i++) {
		size_t findPos = lines[i].find_last_of(',');
		stringstream ss(lines[i].substr(findPos + 1));
		ss >> points;
		if (this->myScore >= points) {
			writeFile << this->saveName << "," << this->myScore << endl;
			this->myScore = -1;
		}
		if (i != 4) {
			writeFile << lines[i] << endl;
		}
	}
	this->myScore = -1;
	writeFile.close();
}
