/*
 * Setting.h
 *
 *  Created on: 13 dec 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef SETTING_H_
#define SETTING_H_

#include <string>

enum MenuType {
	EMPTY_MENU, PLAYGAME, HIGHSCORE, HELP, INFORMATION, EXIT
};
enum RebelType {
	MIGHTY_OUTCAST = 1001, OUTLAW = 1002, ROYAL_KATANA_MEN = 1003
};
enum TowerType {
	EMPTY_TOWER = 0, ACID = 80, ICE = 120, STONE = 300, SUPERIOR = 200
};

/**
 * Global settings for the game and files.
 */
class Setting {
public:
	Setting();
	virtual ~Setting();

	static const std::string IMAGE_DIR;
	static const std::string MAP_DIR;

	static const char* FONT;
	static const std::string BACKGROUND;
	static const std::string BACKGROUND_PLAY;
	static const std::string HIGHSCORE;
	static const std::string DEFAULT_MAP;

	static const unsigned int LIFE;
	static const unsigned int ROUNDS;
	static const unsigned int TIME_START;
	static const unsigned int WEALTH;
};

#endif /* SETTING_H_ */
