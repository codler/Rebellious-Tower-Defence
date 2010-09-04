/*
 * PlayState.cpp
 *
 *  Created on: 24 nov 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#include "PlayState.h"

#ifndef M_PI
#define M_PI 3.14159265358
#endif

using namespace std;

PlayState::PlayState() {
	this->background = IMG_Load(Setting::BACKGROUND_PLAY.c_str());
	if (!this->background) {
		cerr << "Image could not load!";
	}

	// Set map size
	this->map = new Map(26, 23);

	imageAcid = IMG_Load(Map::getImage(2).c_str());
	imageIce = IMG_Load(Map::getImage(3).c_str());
	imageStone = IMG_Load(Map::getImage(4).c_str());
	imageSuperior = IMG_Load(Map::getImage(5).c_str());

	imageMightyOutcast = IMG_Load(Map::getImage(MIGHTY_OUTCAST).c_str());
	imageOutlaw = IMG_Load(Map::getImage(OUTLAW).c_str());
	imageRoyalKatanaMen = IMG_Load(Map::getImage(ROYAL_KATANA_MEN).c_str());

	// Set default properties on each rebel.
	Round mightyOutcast = Round();
	mightyOutcast.bonus = 10;
	mightyOutcast.count = 10;
	mightyOutcast.health = 0;
	mightyOutcast.frequent = 60;
	mightyOutcast.value = 10;
	mightyOutcast.speed = 1;
	mightyOutcast.type = MIGHTY_OUTCAST;

	Round outlaw = Round();
	outlaw.bonus = 10;
	outlaw.count = 10;
	outlaw.health = 0;
	outlaw.frequent = 45;
	outlaw.value = 15;
	outlaw.speed = 2;
	outlaw.type = OUTLAW;

	Round royalKatanaMen = Round();
	royalKatanaMen.bonus = 15;
	royalKatanaMen.count = 7;
	royalKatanaMen.health = 10;
	royalKatanaMen.frequent = 40;
	royalKatanaMen.value = 20;
	royalKatanaMen.speed = 2;
	royalKatanaMen.type = ROYAL_KATANA_MEN;

	// Set properties for rebels on each round.
	for (unsigned int i = 0; i < Setting::ROUNDS; i++) {
		// Every 5 round
		if (i % 5 == 4) {
			royalKatanaMen.count += 3;
			royalKatanaMen.health += 150 + i * 10;
			royalKatanaMen.speed += 0.1;
			this->roundItem.push_back(royalKatanaMen);
			// Every odd round
		} else if (i % 2 == 0) {
			mightyOutcast.count += 4;
			mightyOutcast.health += 40 + i * 30;
			mightyOutcast.speed += 0.1;
			this->roundItem.push_back(mightyOutcast);
			// Every even round
		} else {
			outlaw.count += 4;
			outlaw.health += 50 + i * 30;
			outlaw.speed += 0.1;

			this->roundItem.push_back(outlaw);
		}
	}

	// Default starting values
	this->holding = Setting::WEALTH;
	this->life = Setting::LIFE;
	this->round = 0;
	this->nextRebel = Setting::TIME_START;

	// Don't touch!
	this->buyTower = NULL;
	this->myScore = 0;
	this->pause = false;
}

/**
 * Deallocating memory
 */
PlayState::~PlayState() {
	if (NULL != this->background) {
		SDL_FreeSurface(this->background);
		this->background = NULL;
	}

	if (NULL != this->buyTower) {
		delete this->buyTower;
		this->buyTower = NULL;
	}

	delete map;
	map = NULL;

	for (vector<Shot*>::iterator itShot = this->shots.begin(); itShot
			!= this->shots.end();) {
		delete *itShot;
		itShot = this->shots.erase(itShot);
	}

	for (vector<Rebel*>::iterator itRebel = this->rebels.begin(); itRebel
			!= this->rebels.end();) {
		delete *itRebel;
		itRebel = this->rebels.erase(itRebel);
	}

	for (vector<Tower*>::iterator itTower = this->towers.begin(); itTower
			!= this->towers.end();) {
		delete *itTower;
		itTower = this->towers.erase(itTower);
	}

	SDL_FreeSurface(imageAcid);
	SDL_FreeSurface(imageIce);
	SDL_FreeSurface(imageStone);
	SDL_FreeSurface(imageSuperior);

	SDL_FreeSurface(imageMightyOutcast);
	SDL_FreeSurface(imageOutlaw);
	SDL_FreeSurface(imageRoyalKatanaMen);
}

/**
 *  Makes the game to go forward.
 */
void PlayState::clockTick() {
	if (this->pause) {
		return;
	}

	// Move Rebel
	for (vector<Rebel*>::iterator itRebel = this->rebels.begin(); itRebel
			!= this->rebels.end();) {
		// get available roads and move.
		(*itRebel)->move(this->map->getEnvironment(this->map->pixelToPos(
				(*itRebel)->posX, (*itRebel)->posY)));

		if (this->map->pixelToPos((*itRebel)->posX, (*itRebel)->posY)
				== this->map->endPoint) {

			delete *itRebel;
			itRebel = this->rebels.erase(itRebel);
			this->life--;
		} else {
			// check if shots are near.
			for (vector<Shot*>::iterator itShot = this->shots.begin(); itShot
					!= this->shots.end();) {
				int add = 1;
				if (PlayState::intersect((*itShot)->posX, (*itShot)->posY,
						(*itRebel)->posX, (*itRebel)->posY, 10, 20)) {

					// Reduce rebel hp
					if ((*itShot)->speciality == SUPERIOR) {
						(*itRebel)->health -= (*itShot)->damage;
					}

					// Poison
					if ((*itShot)->speciality == ACID) {
						(*itRebel)->speed -= 0.05;
						(*itRebel)->health -= (*itShot)->damage * 0.4;
						(*itRebel)->poison = (*itShot)->damage * 0.6;
					}

					// Reduce rebel speed and some areaofeffect
					if ((*itShot)->speciality == ICE) {
						(*itRebel)->speed -= 0.5;
						(*itShot)->hasHit = true;
						// For areaofeffect;
					} else if ((*itShot)->speciality == STONE) {
						(*itShot)->hasHit = true;
					} else {

						// delete shot
						delete *itShot;
						itShot = this->shots.erase(itShot);
						add = 0;
					}
				}
				if (add) {
					itShot++;
				}
			}

			// check hp if dead.
			if ((*itRebel)->health <= 0) {
				this->holding += (*itRebel)->value;

				delete *itRebel;
				itRebel = this->rebels.erase(itRebel);
			} else {
				itRebel++;
			}
		}
	}

	// Tower is tracking rebels
	for (vector<Tower*>::iterator itTower = this->towers.begin(); itTower
			!= this->towers.end(); itTower++) {
		(*itTower)->clockTick();

		for (vector<Rebel*>::iterator itRebel = this->rebels.begin(); itRebel
				!= this->rebels.end(); itRebel++) {

			// if rebel found , shoot
			if ((*itTower)->fire((*itRebel)->posX, (*itRebel)->posY)) {
				double angle = atan2(static_cast<double> ((*itRebel)->posY
						- (*itTower)->posY),
						static_cast<double> ((*itRebel)->posX
								- (*itTower)->posX));

				this->shots.push_back(new Shot((*itTower), angle));
			}
		}
	}

	for (vector<Shot*>::iterator itShot = this->shots.begin(); itShot
			!= this->shots.end();) {
		if (!(*itShot)->hasHit) {
			(*itShot)->move();
			if ((*itShot)->fuel <= 0) {
				// delete shot
				delete *itShot;
				itShot = this->shots.erase(itShot);
			} else {
				itShot++;
			}
		} else {

			for (vector<Rebel*>::iterator itRebel = this->rebels.begin(); itRebel
					!= this->rebels.end();) {
				if (PlayState::intersect((*itShot)->posX, (*itShot)->posY,
						(*itRebel)->posX, (*itRebel)->posY,
						(*itShot)->areaOfEffect, 20)) {

					(*itRebel)->health -= (*itShot)->damage;

					if ((*itRebel)->health <= 0) {
						this->holding += (*itRebel)->value;

						delete *itRebel;
						itRebel = this->rebels.erase(itRebel);
					} else {
						itRebel++;
					}
				} else {
					itRebel++;
				}
			}
			// delete shot
			delete *itShot;
			itShot = this->shots.erase(itShot);

		}
	}

	// On game over
	if (this->life == 0 || (this->round >= Setting::ROUNDS
			&& this->rebels.size() == 0)) {

		AbstractGameState* state = new GameOverState();
		this->state = state;
		this->state->myScore = (this->round + 1) * this->holding;
		this->state->round = this->round;
		this->state->mapLoaded = this->map->mapLoaded;
	}

	if (this->round < Setting::ROUNDS) {
		for (unsigned int i = 0; i < this->round; i++) {
			if (this->roundItem[i].count > 0 && rand() % this->map->squareSize
					== 0) {
				int x = this->map->posToPixelX(this->map->startPoint);
				int y = this->map->posToPixelY(this->map->startPoint);

				this->rebels.push_back(new Rebel(this->roundItem[i], x, y));
				this->roundItem[i].count--;
				break;
			}
		}

		// Initiate rebels on each round.
		if (this->roundItem[this->round].count > 0 && this->nextRebel <= 0) {
			int x = this->map->posToPixelX(this->map->startPoint);
			int y = this->map->posToPixelY(this->map->startPoint);

			this->rebels.push_back(
					new Rebel(this->roundItem[this->round], x, y));
			this->roundItem[this->round].count--;
			this->nextRebel = this->roundItem[this->round].frequent / (rand()
					% 2 + 1);
		}
		this->nextRebel--;

		// Set new round when all rebels spawned in the current round.
		if (roundItem[this->round].count == 0) {
			this->holding += roundItem[this->round].bonus;
			this->round++;
			this->nextRebel = 90;
		}
	}
}

/**
 * Draw all objects on screen.
 */
void PlayState::draw(SDL_Surface* surface) {
	if (this->pause) {
		if (!this->reDraw) {
			return;
		} else {
			this->reDraw = false;
		}
	}

	// Text color white
	SDL_Color whiteColor = { 255, 255, 255, 0 };
	// Text color black
	SDL_Color blackColor = { 0, 0, 0, 0 };

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	SDL_BlitSurface(this->background, NULL, surface, &rect);

	// Draw map
	this->map->draw(surface);

	// Draw rebels
	for (vector<Rebel*>::iterator itRebel = this->rebels.begin(); itRebel
			!= this->rebels.end(); itRebel++) {
		//(*itRebel)->draw(surface);

		if ((*itRebel)->type == MIGHTY_OUTCAST) {
			(*itRebel)->draw(surface, this->imageMightyOutcast);
		} else if ((*itRebel)->type == OUTLAW) {
			(*itRebel)->draw(surface, this->imageOutlaw);
		} else if ((*itRebel)->type == ROYAL_KATANA_MEN) {
			(*itRebel)->draw(surface, this->imageRoyalKatanaMen);
		}
	}

	// Draw towers
	for (vector<Tower*>::iterator itTower = this->towers.begin(); itTower
			!= this->towers.end(); itTower++) {
		if (this->map->mousePosition == this->map->pixelToPos((*itTower)->posX,
				(*itTower)->posY)) {
			if ((*itTower)->type == ACID) {
				(*itTower)->draw(surface, this->imageAcid, true);
			} else if ((*itTower)->type == ICE) {
				(*itTower)->draw(surface, this->imageIce, true);
			} else if ((*itTower)->type == SUPERIOR) {
				(*itTower)->draw(surface, this->imageSuperior, true);
			} else if ((*itTower)->type == STONE) {
				(*itTower)->draw(surface, this->imageStone, true);
			}
		} else {
			if ((*itTower)->type == ACID) {
				(*itTower)->draw(surface, this->imageAcid);
			} else if ((*itTower)->type == ICE) {
				(*itTower)->draw(surface, this->imageIce);
			} else if ((*itTower)->type == SUPERIOR) {
				(*itTower)->draw(surface, this->imageSuperior);
			} else if ((*itTower)->type == STONE) {
				(*itTower)->draw(surface, this->imageStone);
			}
		}

	}

	// Draw shots
	for (vector<Shot*>::iterator itShot = this->shots.begin(); itShot
			!= this->shots.end(); itShot++) {
		(*itShot)->draw(surface);
	}

	// If user have selected a tower.
	if (this->buyTower != NULL) {
		if (this->buyTower->type == ACID) {
			this->buyTower->draw(surface, this->imageAcid, true);
		} else if (this->buyTower->type == ICE) {
			this->buyTower->draw(surface, this->imageIce, true);
		} else if (this->buyTower->type == SUPERIOR) {
			this->buyTower->draw(surface, this->imageSuperior, true);
		} else if (this->buyTower->type == STONE) {
			this->buyTower->draw(surface, this->imageStone, true);
		}
	}

	// Pause text
	if (this->pause) {
		boxRGBA(surface, 200, 120, 540, 330, 150, 200, 150, 255);
		Draw::text(surface, "Pause", blackColor, 250, 110, 110);

		Draw::text(surface, "Go to start", whiteColor, 250, 220, 32);
		Draw::text(surface, "Continue", whiteColor, 250, 250, 32);
		Draw::text(surface, "Exit", whiteColor, 250, 280, 32);
		return;
	}

	// Print out information
	// At upper right
	Draw::text(surface, "Information", whiteColor, 825, 23, 32, 100);
	Draw::text(surface, "Round: " + AbstractGameState::intToString(this->round
			+ 1), whiteColor, 825, 60, 20);
	Draw::text(surface, "Life: " + AbstractGameState::intToString(this->life),
			whiteColor, 825, 90, 20);
	Draw::text(surface, "Wealth: " + AbstractGameState::intToString(
			this->holding) + " stone", whiteColor, 825, 120, 20);
	Draw::text(surface, "Next wave", blackColor, 825, 200, 30);

	// At middle right
	Draw::text(surface, "Market", blackColor, 830, 290, 50, 100);
	int marketTowerX = 800;
	rect.x = marketTowerX;
	rect.y = 350;
	SDL_BlitSurface(imageAcid, NULL, surface, &rect);
	rect.x = marketTowerX + 50;
	SDL_BlitSurface(imageIce, NULL, surface, &rect);
	rect.x = marketTowerX + 100;
	SDL_BlitSurface(imageSuperior, NULL, surface, &rect);
	rect.x = marketTowerX + 150;
	SDL_BlitSurface(imageStone, NULL, surface, &rect);

	// When mouse hover over tower. Display information about it.
	if (this->marketChoice == ACID) {
		Draw::text(surface, "Information", blackColor, 800, 390, 30);
		Draw::text(surface, "Name: Acid Tower", blackColor, 800, 430, 22);
		Draw::text(surface, "Price: 80 stone", blackColor, 800, 460, 22);
		Draw::text(surface, "Specialty:", blackColor, 800, 490, 22);
		Draw::text(surface, "The tower will break down", blackColor, 810, 510,
				16);
		Draw::text(surface, "the rebels to a painful death.", blackColor, 810,
				530, 16);
	} else if (this->marketChoice == ICE) {
		Draw::text(surface, "Information", blackColor, 800, 390, 30);
		Draw::text(surface, "Name: Ice Tower", blackColor, 800, 430, 22);
		Draw::text(surface, "Price: 120 stone", blackColor, 800, 460, 22);
		Draw::text(surface, "Specialty:", blackColor, 800, 490, 22);
		Draw::text(surface, "Preventing the rebels rapid", blackColor, 810,
				510, 16);
		Draw::text(surface, "storm attack.", blackColor, 810, 530, 16);
	} else if (this->marketChoice == SUPERIOR) {
		Draw::text(surface, "Information", blackColor, 800, 390, 30);
		Draw::text(surface, "Name: Superior Tower", blackColor, 800, 430, 22);
		Draw::text(surface, "Price: 200 stone", blackColor, 800, 460, 22);
		Draw::text(surface, "Specialty:", blackColor, 800, 490, 22);
		Draw::text(surface, "It has none, but it focus", blackColor, 810, 510,
				16);
		Draw::text(surface, "on the firepower.", blackColor, 810, 530, 16);
	} else if (this->marketChoice == STONE) {
		Draw::text(surface, "Information", blackColor, 800, 390, 30);
		Draw::text(surface, "Name: Stone Tower", blackColor, 800, 430, 22);
		Draw::text(surface, "Price: 300 stone", blackColor, 800, 460, 22);
		Draw::text(surface, "Specialty:", blackColor, 800, 490, 22);
		Draw::text(surface, "It's fire power can hit", blackColor, 810, 510, 16);
		Draw::text(surface, "several rebels in one shot.", blackColor, 810,
				530, 16);
	}
}

void PlayState::keyPressed(SDLKey key) {
	// Pause and unpause
	if (SDLK_p == key || SDLK_ESCAPE == key) {
		if (!this->pause) {
			delete this->buyTower;
			this->buyTower = NULL;
			this->pause = true;
		} else {
			this->reDraw = true;
			this->pause = false;
		}
	}
}

void PlayState::mouseMoved(const int button, const int mouseX, const int mouseY) {
	// When mouse hover over tower. Display information about it.
	if (mouseHit(mouseX, mouseY, 800, 350, 30, 30)) {
		this->marketChoice = ACID;
	} else if (mouseHit(mouseX, mouseY, 850, 350, 30, 30)) {
		this->marketChoice = ICE;
	} else if (mouseHit(mouseX, mouseY, 900, 350, 30, 30)) {
		this->marketChoice = SUPERIOR;
	} else if (mouseHit(mouseX, mouseY, 950, 350, 30, 30)) {
		this->marketChoice = STONE;
	} else {
		this->marketChoice = EMPTY_TOWER;
	}

	int mapPosition = this->map->pixelToPos(mouseX, mouseY);
	// If user have selected a tower.
	if (this->buyTower != NULL) {
		if (this->map->mouseOver(mouseX, mouseY)) {
			int x = this->map->posToPixelX(mapPosition);
			int y = this->map->posToPixelY(mapPosition);
			this->buyTower->setPos(x, y);
		} else {
			this->buyTower->setPos(mouseX, mouseY);
		}
	}
	if (this->map->mouseOver(mouseX, mouseY)) {
		this->map->mousePosition = mapPosition;
	} else {
		this->map->mousePosition = -1;
	}
}

void PlayState::mousePressed(const int button, const int mouseX,
		const int mouseY) {

	if (this->pause) {
		if (mouseHit(mouseX, mouseY, 250, 225, 150, 30)) {
			// Go to start
			AbstractGameState* state = new IntroState();
			this->state = state;
		} else if (mouseHit(mouseX, mouseY, 250, 255, 150, 30)) {
			// Continue
			this->reDraw = true;
			this->pause = false;
		} else if (mouseHit(mouseX, mouseY, 250, 285, 150, 30)) {
			// exit
			this->done = true;
		}
	}

	// next round
	if (mouseHit(mouseX, mouseY, 825, 200, 140, 30)) {
		if (this->round < Setting::ROUNDS - 1) {
			this->round++;
			this->holding += 30;
		}
	}

	// Buy menu
	if (this->buyTower != NULL) {
		// check if mouse it over game map
		if (this->map->mouseOver(mouseX, mouseY)) {
			// check if tile is empty
			if (this->map->getData(this->map->pixelToPos(mouseX, mouseY)) == 0) {

				// check if tower is already on tile
				bool towerExist = false;
				for (vector<Tower*>::iterator itTower = this->towers.begin(); itTower
						!= this->towers.end(); itTower++) {
					if (this->map->pixelToPos((*itTower)->posX,
							(*itTower)->posY) == this->map->pixelToPos(mouseX,
							mouseY)) {
						towerExist = true;
						break;
					}
				}
				if (!towerExist) {
					this->holding -= this->buyTower->price;
					this->towers.push_back(this->buyTower);
					this->buyTower = NULL;
				}
			}
		} else {
			delete this->buyTower;
			this->buyTower = NULL;
		}
	} else {
		if (mouseHit(mouseX, mouseY, 800, 350, 30, 30)) {
			if (this->holding - ACID >= 0) {
				this->buyTower = new Tower(ACID);
				this->buyTower->setPos(mouseX, mouseY);
			}
		} else if (mouseHit(mouseX, mouseY, 850, 350, 30, 30)) {
			if (this->holding - ICE >= 0) {
				this->buyTower = new Tower(ICE);
				this->buyTower->setPos(mouseX, mouseY);
			}
		} else if (mouseHit(mouseX, mouseY, 900, 350, 30, 30)) {
			if (this->holding - SUPERIOR >= 0) {
				this->buyTower = new Tower(SUPERIOR);
				this->buyTower->setPos(mouseX, mouseY);
			}
		} else if (mouseHit(mouseX, mouseY, 950, 350, 30, 30)) {
			if (this->holding - STONE >= 0) {
				this->buyTower = new Tower(STONE);
				this->buyTower->setPos(mouseX, mouseY);
			}
		}
	}
}

const bool PlayState::intersect(const double x1, const double y1, const int x2,
		const int y2, const int aoe1, const int aoe2) {
	if (y1 + aoe1 < y2)
		return false;
	if (y1 > y2 + aoe2)
		return false;

	if (x1 + aoe1 < x2)
		return false;
	if (x1 > x2 + aoe2)
		return false;

	return true;

}
