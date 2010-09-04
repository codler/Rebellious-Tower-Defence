/*
 * main.cpp
 *
 *  Created on: 23 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "GameEngine.h"
#include "IntroState.h"
#include <time.h>

using namespace std;

int main(int argc, char **argv) {
	// To get random number
	srand((unsigned) time(NULL));

	// Start game in 1000x700 windows-mode
	GameEngine* engine = new GameEngine(1000, 700, false);

	// Begin IntroState
	AbstractGameState* state = new IntroState();
	engine->setState(state);
	engine->setTickInterval(30);

	// Keep alive
	engine->loop();

	delete engine;
	engine = NULL;

	return 0;
}
