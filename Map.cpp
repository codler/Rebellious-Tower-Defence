/*
 * Map.cpp
 *
 *  Created on: 26 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "Map.h"

using namespace std;

Map::Map(int x, int y) {
	this->sizeX = x;
	this->sizeY = y;

	this->squareSize = 30;

	this->imageStartPoint = IMG_Load(this->getImage(8).c_str());
	this->imageEndPoint = IMG_Load(this->getImage(9).c_str());

	this->rock = IMG_Load(Map::getImage(101).c_str());

	// Set default image to tile
	for (int i = 0; i < (this->sizeX * this->sizeY); i++) {
		this->data.push_back(0);
	}
	for (int i = 0; i < 18; i++) {
		this->tile.push_back(IMG_Load(this->getImage(i).c_str()));
		if (!this->tile[i]) {
			cerr << "Image could not load!";
		}
	}

	// Put rocks on map
	SDL_Rect rect;
	for (int i = 0; i < (this->sizeX * this->sizeY) / 100; i++) {
		rect.x = rand() % (this->sizeX * this->squareSize);
		rect.y = rand() % (this->sizeY * this->squareSize);
		this->rocks.push_back(rect);
	}
}

Map::~Map() {
	for (int i = 0; i < 18; i++) {
		SDL_FreeSurface(this->tile[i]);
	}
	SDL_FreeSurface(this->imageStartPoint);
	SDL_FreeSurface(this->imageEndPoint);
	SDL_FreeSurface(this->rock);
}

void Map::draw(SDL_Surface* surface) {
	SDL_Rect rect;
	// Put rocks on map
	for (vector<SDL_Rect>::iterator itRock = rocks.begin(); itRock
			!= rocks.end(); itRock++) {
		rect.x = (*itRock).x;
		rect.y = (*itRock).y;
		SDL_BlitSurface(this->rock, NULL, surface, &rect);
	}

	// Draw tiles
	for (int i = 0; i < (this->sizeX * this->sizeY); i++) {
		rect.x = this->posToPixelX(i);
		rect.y = this->posToPixelY(i);
		SDL_BlitSurface(this->tile[this->getData(i)], NULL, surface, &rect);
	}

	// startPoint
	rect.x = this->posToPixelX(this->startPoint);
	rect.y = this->posToPixelY(this->startPoint);
	SDL_BlitSurface(imageStartPoint, NULL, surface, &rect);

	// endPoint
	rect.x = this->posToPixelX(this->endPoint);
	rect.y = this->posToPixelY(this->endPoint);
	SDL_BlitSurface(imageEndPoint, NULL, surface, &rect);

}

/**
 * Load map to game.
 */
void Map::loadFile(string filename) {

	this->mapLoaded = filename;
	ifstream openFile(filename.c_str());
	int row = 0;
	int column = 1;
	string number;
	string line;
	int choice;

	while (!openFile.eof()) {
		getline(openFile, line);
		stringstream ss(line);
		if (row == 0) {
			size_t findPos = line.find_last_of(',');
			stringstream intStartPoint(line.substr(0, findPos));
			intStartPoint >> this->startPoint;
			this->startPoint--;
			stringstream intEndPoint(line.substr(findPos + 1));
			intEndPoint >> this->endPoint;
			this->endPoint--;
		} else {
			while (getline(ss, number, ',')) {
				stringstream ss(number);
				ss >> choice;
				this->setTile(choice, column, row);
				column++;
			}
		}
		column = 1;
		row++;
	}
	openFile.close();

	// set end point as road
	this->setTile(1, this->tileToPosX(this->endPoint) + 1, this->tileToPosY(
			this->endPoint) + 1);
}

/**
 * Save map to a file.
 */
void Map::save(string filename) {
	ofstream writeFile(filename.c_str());
	writeFile << this->startPoint + 1 << "," << this->endPoint + 1 << endl;
	for (int i = 0; i < (this->sizeX * this->sizeY); i++) {
		if (i % this->sizeX == this->sizeX - 1) {
			writeFile << this->data[i];
			writeFile << endl;
		} else {
			writeFile << this->data[i] << ",";
		}
	}
	writeFile.close();
}

void Map::setTile(int type, int x, int y) {
	this->data[this->tileToPos(x, y)] = type;
}

/**
 * Get tile-type in all four direction.
 */
vector<int> Map::getEnvironment(int position) {
	vector<int> environment;

	if (position > this->sizeX) {
		// up
		environment.push_back(this->data[position - this->sizeX]);

		// right
		if (position % this->sizeX < (position + 1) % this->sizeX) {
			environment.push_back(this->data[position + 1]);
		} else {
			// right
			environment.push_back(0);
		}
	} else {
		// up
		environment.push_back(0);

		// right
		environment.push_back(this->data[position + 1]);
	}

	// down
	if (position / this->sizeX < this->sizeY) {
		environment.push_back(this->data[position + this->sizeX]);
	} else {
		environment.push_back(0);
	}

	if (position > 0 && position % this->sizeX != 0) {
		// left
		environment.push_back(this->data[position - 1]);
	} else {
		environment.push_back(0);
	}
	return environment;
}

int Map::getData(int position) {
	return this->data[position];
}

/**
 * Get position (tile unit) from X-pixel and Y-pixel
 */
int Map::pixelToPos(int pixelX, int pixelY) {
	return static_cast<int> (ceil(static_cast<double> (pixelX
			/ this->squareSize))) + (static_cast<int> (ceil(
			static_cast<double> (pixelY / this->squareSize)))) * this->sizeX;
}

/**
 * Get X-pixel from position (tile unit) .
 */
int Map::posToPixelX(int position) {
	return position % this->sizeX * this->squareSize;
}

/**
 *  Get Y-pixel from position (tile unit).
 */
int Map::posToPixelY(int position) {
	return static_cast<int> (floor(static_cast<double> (position / this->sizeX)))
			* this->squareSize;
}

/**
 * Get position (tile unit) from X-tile and Y-tile.
 */
int Map::tileToPos(int x, int y) {
	return x - 1 + (y - 1) * this->sizeX;
}

/**
 * Get X-tile from position (tile unit)
 */
int Map::tileToPosX(int position) {
	return position % this->sizeX;
}

/**
 * Get Y-tile from position (tile unit)
 */
int Map::tileToPosY(int position) {
	return position / this->sizeX;
}

/**
 * Check if mouse is over the map or outside map.
 */
bool Map::mouseOver(int x, int y) {
	if (this->sizeX * this->squareSize > x && this->sizeY * this->squareSize
			> y) {
		return true;
	}
	return false;
}

string Map::getImage(int type) {
	switch (type) {
	case 0: // Default image
		return Setting::IMAGE_DIR + "defaulttile.png";
	case 1: // road
		return Setting::IMAGE_DIR + "road.png";
	case 2: // acid tower
		return Setting::IMAGE_DIR + "acidtower.png";
	case 3: // ice tower
		return Setting::IMAGE_DIR + "icetower.png";
	case 4: // stone tower
		return Setting::IMAGE_DIR + "stonetower.png";
	case 5: // superior tower
		return Setting::IMAGE_DIR + "superiortower.png";
	case 6: // chistmastree
		return Setting::IMAGE_DIR + "christmastree.png";
	case 7: // grass
		return Setting::IMAGE_DIR + "flyinggrass.png";
	case 8: // startpoint
		return Setting::IMAGE_DIR + "starting.png";
	case 9: // endpoint
		return Setting::IMAGE_DIR + "ending.png";
	case 10: // tree
		return Setting::IMAGE_DIR + "tree.png";
	case 11: // box
		return Setting::IMAGE_DIR + "box.png";
	case 12: // fire
		return Setting::IMAGE_DIR + "fire.png";
	case 13: // water
		return Setting::IMAGE_DIR + "water.png";
	case 14: // stone
		return Setting::IMAGE_DIR + "stone.png";
	case 15: // RIP stone
		return Setting::IMAGE_DIR + "ripstone.png";
	case 16: // tree 2
		return Setting::IMAGE_DIR + "tree2.png";
		/**
		 * Special default tile
		 */
	case 17: // land tile
		return Setting::IMAGE_DIR + "landtile.png";
		/**
		 * Ammo
		 */
	case 100:
		return Setting::IMAGE_DIR + "ammo.png";
	case 101:
		return Setting::IMAGE_DIR + "stonebox.png";
		/**
		 * Rebels
		 */
	case 1001:
		return Setting::IMAGE_DIR + "mightyoutcast.png";
	case 1002:
		return Setting::IMAGE_DIR + "outlaw.png";
	case 1003:
		return Setting::IMAGE_DIR + "royalkatanamen.png";
	default:
		return Setting::IMAGE_DIR + "defaulttile.png";
	}
}
