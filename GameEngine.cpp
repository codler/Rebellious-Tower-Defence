/*
 * gameEngine.cpp
 *
 *  Created on: 23 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "GameEngine.h"

using namespace std;

/**
 * Initiate SDL and GUI
 */
GameEngine::GameEngine(const int width, const int height, const bool fullscreen) {
	Uint32 flags;

	if (-1 == SDL_Init(SDL_INIT_VIDEO)) {
		printf("Can't init SDL:  %s\n", SDL_GetError());
		exit(1);
	}

	if (fullscreen) {
		flags = SDL_SWSURFACE | SDL_FULLSCREEN;
	} else {
		flags = SDL_SWSURFACE;
	}

	this->screen = SDL_SetVideoMode(width, height, 32, flags);
	if (NULL == screen) {
		printf("Can't set video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	// Title
	SDL_WM_SetCaption("Rebellious Tower Defence", NULL);

	SDL_EnableUNICODE(1);

	// Initiate font
	TTF_Init();

	this->state = NULL;
	this->tickInterval = 20;
}

GameEngine::~GameEngine() {
	delete this->state;
	this->state = NULL;

	atexit(TTF_Quit);
	SDL_Quit();
}

/**
 * Available states: IntroState, PlayState, GameOverState, EditMapState
 */
void GameEngine::setState(AbstractGameState* state) {
	delete this->state;
	this->state = state;
}

/**
 * TickInterval is 1000 / tick = FPS (frame per second)
 */
void GameEngine::setTickInterval(int interval) {
	this->tickInterval = interval;
}

/**
 * Run the game
 */
void GameEngine::loop() {
	SDL_Event event;
	int button;
	Uint32 nextTick;

	if (NULL == this->state) {
		return;
	}

	this->done = false;
	while (!this->done) {
		nextTick = SDL_GetTicks() + this->tickInterval;

		// Listen to key or mouse events
		while (SDL_PollEvent(&event)) {
			this->state->event = event;

			Uint8 *keystates = SDL_GetKeyState(NULL);
			// Increase game speed while pressing space and moving mouse.
			if (keystates[SDLK_SPACE]) {
				this->state->clockTick();
			}

			switch (event.type) {
			case SDL_QUIT:
				this->done = true;
				break;

			case SDL_KEYDOWN:
				this->state->keyPressed(event.key.keysym.sym);
				break;

			case SDL_MOUSEMOTION:
				button = 0;

				if (SDL_BUTTON(1) & event.motion.state) {
					button = 1;
				} else if (SDL_BUTTON(2) & event.motion.state) {
					button = 2;
				} else if (SDL_BUTTON(3) & event.motion.state) {
					button = 3;
				}

				this->state->mouseMoved(button, event.motion.x, event.motion.y);
				break;

			case SDL_MOUSEBUTTONDOWN:
				button = 0;

				if (SDL_BUTTON_LEFT == event.button.button) {
					button = 1;
				} else if (SDL_BUTTON_MIDDLE == event.button.button) {
					button = 2;
				} else if (SDL_BUTTON_RIGHT == event.button.button) {
					button = 3;
				}

				this->state->mousePressed(button, event.button.x,
						event.button.y);

				break;
			}
		}

		// Game tick
		this->state->clockTick();
		// Redraw screen
		this->state->draw(screen);
		// print out to screen
		SDL_Flip(screen);

		// if state have set a state inside state, change state.
		if (NULL != this->state->state) {
			this->setState(this->state->state);
		}

		if (this->state->done) {
			this->done = true;
		}

		// To correct deley time.
		if (SDL_GetTicks() < nextTick) {
			SDL_Delay(nextTick - SDL_GetTicks());
		}
	}

}
