#include "BatCalculations.h"
#include <iostream>


BatCalculations::BatCalculations(float x, float y)
{
	position.x = x;
	position.y = y;
}

sf::Vector2f BatCalculations::getPosition()
{
	return position;
}

void BatCalculations::moveLeft(sf::Time time)
{
	float direction = -1;
	position.x -= batSpeed * time.asMilliseconds();
}


void BatCalculations::moveRight(sf::Time time)
{
	position.x += batSpeed * time.asMilliseconds();
}

BatCalculations::~BatCalculations()
{
}
