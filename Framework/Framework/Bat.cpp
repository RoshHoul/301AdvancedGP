#include "Bat.h"

Bat::Bat(float startX, float startY)
{
	position.x = startX;
	position.y = startY;

	batShape.setSize(sf::Vector2f(50, 5));
	batShape.setPosition(position);
}

sf::FloatRect Bat::getBounds()
{
	return batShape.getGlobalBounds();
}

sf::RectangleShape Bat::getShape()
{
	return batShape;
}

sf::Vector2f Bat::getPosition()
{
	return position;
}

void Bat::moveLeft()
{
	position.x -= batSpeed;
}

void Bat::moveRight()
{
	position.x += batSpeed;
}

void Bat::update(sf::Vector2f updatePos)
{
	position = updatePos;
	batShape.setPosition(updatePos);
	
}

Bat::~Bat()
{
}
