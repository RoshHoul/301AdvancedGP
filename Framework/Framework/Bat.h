#pragma once
#include <SFML\Graphics.hpp>

class Bat
{
	sf::Vector2f position;
	sf::RectangleShape batShape;
	float batSpeed = 3.0f;

public:
	Bat(float startX, float startY);

	sf::RectangleShape getShape();
	sf::Vector2f getPosition();
	void update(sf::Vector2f updatePos);
	~Bat();
};

