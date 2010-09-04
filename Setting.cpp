/*
 * Setting.cpp
 *
 *  Created on: 13 dec 2009
 *      Author: codler
 */

#include "Setting.h"

using namespace std;

Setting::Setting() {
}

Setting::~Setting() {
}

/**
 * Folders
 */
const string Setting::IMAGE_DIR = "game/images/";
const string Setting::MAP_DIR = "game/maps/";

/**
 * Files / Assets
 */
const char* Setting::FONT = "game/fonts/AARDC.TTF";
const string Setting::BACKGROUND = Setting::IMAGE_DIR + "background.png";
const string Setting::BACKGROUND_PLAY = Setting::IMAGE_DIR + "playstate.png";
const string Setting::HIGHSCORE = "game/files/highscore.txt";
const string Setting::DEFAULT_MAP = "game/files/defaultMap.rtd";

/**
 * Game settings
 */
const unsigned int Setting::LIFE = 3;
const unsigned int Setting::ROUNDS = 20;
const unsigned int Setting::TIME_START = 120;
const unsigned int Setting::WEALTH = 200;
