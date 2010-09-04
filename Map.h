/*
 * Map.h
 *
 *  Created on: 26 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef MAP_H_
#define MAP_H_

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "Setting.h"

/**
 * Tile-system
 */
class Map {
public:
	Map(int x, int y);
	virtual ~Map();

	void draw(SDL_Surface* surface);
	/**
	 * Load map to game.
	 */
	void loadFile(std::string filename);
	/**
	 * Save map to a file.
	 */
	void save(std::string filename);
	void setTile(int type, int x, int y);

	/**
	 * Get tile-type in all four direction.
	 */
	std::vector<int> getEnvironment(int position);
	int getData(int position);
	/**
	 * Get position (tile unit) from X-pixel and Y-pixel
	 */
	int pixelToPos(int pixelX, int pixelY);
	/**
	 * Get X-pixel from position (tile unit).
	 */
	int posToPixelX(int position);
	/**
	 *  Get Y-pixel from position (tile unit).
	 */
	int posToPixelY(int position);
	/**
	 * Get position (tile unit) from X-tile and Y-tile.
	 */
	int tileToPos(int x, int y);
	/**
	 * Get X-tile from position (tile unit)
	 */
	int tileToPosX(int position);
	/**
	 * Get Y-tile from position (tile unit)
	 */
	int tileToPosY(int position);
	/**
	 * Check if mouse is over the map or outside map.
	 */
	bool mouseOver(int x, int y);

	static std::string getImage(int type);

	std::string mapLoaded;
	int mousePosition;
	int sizeX;
	int sizeY;
	int startPoint;
	int endPoint;
	int squareSize;
	std::vector<SDL_Surface*> tile;

private:
	std::vector<SDL_Rect> rocks;

	std::vector<int> data;
	SDL_Surface* imageStartPoint;
	SDL_Surface* imageEndPoint;
	SDL_Surface* rock;
};

#endif /* MAP_H_ */
