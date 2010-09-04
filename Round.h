/*
 * Round.h
 *
 *  Created on: 1 dec 2009
 *      Author: Mikael Andersson and Han Lin Yap
 */

#ifndef ROUND_H_
#define ROUND_H_

#include "Setting.h"

/**
 * Properties on rebel each round.
 */
class Round {
public:
	Round();
	virtual ~Round();

	int count;
	RebelType type;
	int health;
	int bonus;
	int frequent;
	int value;
	double speed;
};

#endif /* ROUND_H_ */
