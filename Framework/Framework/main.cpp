#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <cstring>

#include "Bat.h"
#include "Ball.h"

using namespace std;

bool isAnyKeyPressed() {
	for (int k = -1; k < sf::Keyboard::KeyCount; ++k) {
		if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
			return true;
	}
	return false;
}

int main() {

	int windowWidth = 1024;
	int windowHeight = 768;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Client Pong");
	int score = 0;
	int lives = 3;

	Bat bat(windowWidth / 2, windowHeight - 20);
	Ball ball(windowWidth / 2, 1);

	//HUD todo
	sf::Text hud;
	sf::Event e;

	sf::TcpSocket socket;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	int port = 2000;

	sf::Int32 sendBatState = -1;
	sf::Int32 recieveBatState = -1;

	socket.setBlocking(false);
	socket.connect(ip, port);

	window.setFramerateLimit(60); // Frame Limit

	bool update = false;

	while (window.isOpen()) {

		while (window.pollEvent(e)) {
			if ((e.type == sf::Event::Closed) ||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { 
				window.close();
			}
			else if (e.type == sf::Event::GainedFocus) {
				cout << "GainedFocus" << endl;
				update = true;
			}
			else if (e.type == sf::Event::LostFocus) {
				update = false;
			} 
		}


		sf::Int32 prevBatState = sendBatState;
		if (update) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				//bat.moveLeft();
				sendBatState = 0;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				//bat.moveRight();
				sendBatState = 1;
			}
			else if (!isAnyKeyPressed()) {
				sendBatState = -1;
					
			}
		}

		sf::Packet packet;
		
		//cout << "cl BAT STATE: " << sendBatState << endl;
		if (prevBatState != sendBatState) {
			packet << sendBatState;
		//	cout << "CL PACKET: " << sendBatState << endl;
			socket.send(packet);
		}

		sf::Vector2f newPos;
		socket.receive(packet);
		
		
		if (packet >> newPos.x >> newPos.y) {
			cout << "newPos" << newPos.x << " " << newPos.y << endl;
			cout << "oldPos" << bat.getPosition().x << " " << bat.getPosition().y << endl;
			if (!(abs(newPos.x) - abs(bat.getPosition().x) > 200)) 
				bat.update(newPos);
		}

		if (ball.getPosition().top > windowHeight) {
			ball.hitBottom();
			lives--;

			if (lives < 1) {
				score = 0;
				lives = 3;
			}
		}

		if (ball.getPosition().top < 0) {
			ball.rebountBatOrTop();
			score++;
		}

		if (ball.getPosition().left < 0 || ball.getPosition().left + 10 > windowWidth) {
			ball.reboundSides();
		}

		if (ball.getPosition().intersects(bat.getBounds())) {
			ball.rebountBatOrTop();
		}



		ball.update();
		//bat.update();

		window.clear(sf::Color(26, 128, 281, 255));
		window.draw(bat.getShape());
		window.draw(ball.getShape());

		window.display();
	}

	socket.disconnect();
	return 0;
}