#pragma once
#include <SFML\Network.hpp>
class BatCalculations
{
	sf::Vector2f position;
		float batSpeed = 10.0f;

public:
	BatCalculations(float x, float y);
	sf::Vector2f getPosition();
	void moveLeft(sf::Time time);
	void moveRight(sf::Time time);
	~BatCalculations();
};

