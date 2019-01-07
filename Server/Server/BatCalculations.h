#pragma once
#include <SFML\Graphics.hpp>
class BatCalculations
{
	sf::Vector2f position;
	float batSpeed = 8.0f;
	sf::RectangleShape batShape;

public:
	BatCalculations(float x, float y);
	sf::Vector2f getPosition();
	sf::FloatRect getBounds();
	void moveLeft(sf::Time time);
	void moveRight(sf::Time time);
	~BatCalculations();
};

