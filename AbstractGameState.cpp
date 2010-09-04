/*
 * AbstractGameState.cpp
 *
 *  Created on: 24 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "AbstractGameState.h"

using namespace std;

AbstractGameState::AbstractGameState() {
	this->done = false;
	this->reDraw = true;
	this->state = NULL;
}

AbstractGameState::~AbstractGameState() {
	SDL_FreeSurface(this->background);
}

void AbstractGameState::clockTick() {
}

void AbstractGameState::draw(SDL_Surface* surface) {
}

void AbstractGameState::keyPressed(SDLKey key) {
}

/**
 * Check if mouse hit a specific area.
 */
bool AbstractGameState::mouseHit(const int mouseX, const int mouseY,
		const int x, const int y, const int w, const int h) {
	if (mouseX >= x && mouseX <= (x + w) && mouseY >= y && mouseY <= (y + h)) {
		return true;
	}
	return false;
}

void AbstractGameState::mouseMoved(const int button, const int mouseX,
		const int mouseY) {
}

void AbstractGameState::mousePressed(const int button, const int mouseX,
		const int mouseY) {
}

/**
 * Convert Int to String.
 */
const string AbstractGameState::intToString(const int number) {
	stringstream ss;
	string str;
	ss << number;
	ss >> str;
	return str;
}

/**
 * Get a list of files in a directory.
 */
const vector<string> AbstractGameState::getFilesInDir(const string dir) {
	DIR *open_dir = NULL;
	struct dirent *dirp;
	vector<string> files;

	if ((open_dir = opendir(dir.c_str())) == NULL) {
		files.push_back("There are no maps");
	} else {
		while ((dirp = readdir(open_dir)) != NULL) {
			string filename = dirp->d_name;
			size_t find_pos = filename.find_last_of('.');
			if (filename.substr(find_pos) == ".txt" && filename != ".txt") {
				files.push_back(filename);
			}
		}
	}
	closedir(open_dir);

	return files;
}
