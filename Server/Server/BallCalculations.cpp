#include "BallCalculations.h"

BallCalculations::BallCalculations(float startX, float startY)
{
	position.x = startX;
	position.y = startY;

	ballShape.setSize(sf::Vector2f(10, 10));
	ballShape.setPosition(position);
}

sf::FloatRect BallCalculations::getPosition()
{
	return ballShape.getGlobalBounds();
}

sf::RectangleShape BallCalculations::getShape()
{
	return ballShape;
}

sf::Vector2f BallCalculations::getVecPosition()
{
	return position;
}

float BallCalculations::getXVelocity()
{
	return xVelocity;
}

void BallCalculations::reboundSides()
{
	xVelocity = -xVelocity;
}

void BallCalculations::rebountBatOrTop()
{
	//position.y -= (yVelocity * 30);1
	yVelocity = -yVelocity;
}

void BallCalculations::hitBottom()
{
	position.y = 1;
	position.x = 500;
}

void BallCalculations::update(sf::Time time)
{
	position.y += yVelocity * time.asMilliseconds();
	position.x += xVelocity * time.asMilliseconds();
	ballShape.setPosition(position);
}
