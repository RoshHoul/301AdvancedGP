#include "Bat.h"

Bat::Bat(float startX, float startY)
{
	position.x = startX;
	position.y = startY;

	batShape.setSize(sf::Vector2f(100, 15));
	batShape.setPosition(position);
}



sf::RectangleShape Bat::getShape()
{
	return batShape;
}

sf::Vector2f Bat::getPosition()
{
	return position;
}

void Bat::update(sf::Vector2f updatePos)
{
	position = updatePos;
	batShape.setPosition(updatePos);
}

Bat::~Bat()
{
}
