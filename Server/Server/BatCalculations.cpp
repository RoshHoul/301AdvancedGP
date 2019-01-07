#include "BatCalculations.h"
#include <iostream>


BatCalculations::BatCalculations(float x, float y)
{
	position.x = x;
	position.y = y;

	batShape.setSize(sf::Vector2f(50,5));
	batShape.setPosition(position);
}

sf::Vector2f BatCalculations::getPosition()
{
	return position;
}

sf::FloatRect BatCalculations::getBounds()
{
	return batShape.getGlobalBounds();
}

void BatCalculations::moveLeft(sf::Time time)
{
	position.x -= batSpeed * time.asMilliseconds();
}


void BatCalculations::moveRight(sf::Time time)
{
	position.x += batSpeed * time.asMilliseconds();
}

BatCalculations::~BatCalculations()
{
}
