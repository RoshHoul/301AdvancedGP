#include "Ball.h"

Ball::Ball(float startX, float startY)
{
	position.x = startX;
	position.y = startY;

	ballShape.setSize(sf::Vector2f(10, 10));
	ballShape.setPosition(position);
}

sf::FloatRect Ball::getPosition()
{
	return ballShape.getGlobalBounds();
}

sf::RectangleShape Ball::getShape()
{
	return ballShape;
}

void Ball::update(sf::Vector2f newPos)
{
	position = newPos;
	ballShape.setPosition(position);
}

