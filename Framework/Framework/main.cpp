#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <cstring>

#include "Bat.h"
#include "Ball.h"
#include "ReadFromFile.h"

using namespace std;

bool isAnyKeyPressed() {
	for (int k = -1; k < sf::Keyboard::KeyCount; ++k) {
		if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
			return true;
	}
	return false;
}

int main() {

	int windowWidth = getFromFileInt("windowSize.txt", 1); //1024;
	int windowHeight = getFromFileInt("windowSize.txt", 2); //768

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Client Pong");
	int score = 0;
	int lives = 3;

	Bat bat(windowWidth / 2, windowHeight - 20);
	Ball ball(windowWidth / 2, 1);

	//HUD todo
	sf::Text hud;
	sf::Event e;

	sf::TcpSocket socket;
	sf::IpAddress ip = getFromFileString("ip.txt", 1);//sf::IpAddress::getLocalAddress();
	cout << "IP IS " << ip << endl;
	int port = getFromFileInt("port.txt", 1);
	cout << "PORT IS " << port << endl;
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

		sf::Vector2f batPos;
		sf::Vector2f ballPos;
		socket.receive(packet);
		
		
		if (packet >> batPos.x >> batPos.y >> ballPos.x >> ballPos.y) {
			cout << "newPos" << batPos.x << " " << batPos.y << endl;
			cout << "oldPos" << bat.getPosition().x << " " << bat.getPosition().y << endl;
			bat.update(batPos);
			ball.update(ballPos);
		}



		//ball.update();
		//bat.update();

		window.clear(sf::Color(26, 128, 281, 255));
		window.draw(bat.getShape());
		window.draw(ball.getShape());

		window.display();
	}

	socket.disconnect();
	return 0;
}