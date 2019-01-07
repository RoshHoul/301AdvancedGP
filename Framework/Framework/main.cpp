#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <cstring>

#include "Bat.h"
#include "Ball.h"
#include "ReadFromFile.h"
#include "Sound.h"

using namespace std;

//Check if no key is pressed;
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

	Sound music("background.wav");
	music.setLoop(true);
	
	//HUD todo
	sf::Text hud;
	sf::Event e;

	sf::Clock packetClock;
	float tickRate = 60;
	float interval = 1 / 60;

	sf::TcpSocket socket;
	sf::IpAddress ip = getFromFileString("ip.txt", 1);//sf::IpAddress::getLocalAddress();
	cout << "IP IS " << ip << endl;
	int port = getFromFileInt("port.txt", 1);
	cout << "PORT IS " << port << endl;
	sf::Int32 sendBatState = -1;
	sf::Int32 recieveBatState = -1;

	//Setting setBlocking to false, so the client doesn't wait for packets.
	socket.setBlocking(false);
	socket.connect(ip, port);

	window.setFramerateLimit(60); // Frame Limit

	bool update = false;

	music.play();

	while (window.isOpen()) {

		//Variable to check if packets are ready to send according to the fixed timestep
		bool updateNetwork = false;

		if (packetClock.getElapsedTime().asSeconds() >= interval) {
			updateNetwork = true;
			packetClock.restart();
		}

		while (window.pollEvent(e)) {
			if ((e.type == sf::Event::Closed) ||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { 
				window.close();
			}
			//Take user input only if the current window is selected
			else if (e.type == sf::Event::GainedFocus) {
				cout << "GainedFocus" << endl;
				update = true;
			}
			else if (e.type == sf::Event::LostFocus) {
				update = false;
			} 
		}

		//Update BatState on userInput
		sf::Int32 prevBatState = sendBatState;
		if (update) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				sendBatState = 0;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				sendBatState = 1;
			}
			else if (!isAnyKeyPressed()) {
				sendBatState = -1;
					
			}
		}

		sf::Packet packet;
		
		//Send the state only if it's different than the last one and the fixed time step allows it
		if (prevBatState != sendBatState && updateNetwork) {
			packet << sendBatState;
			socket.send(packet);
		}

		//Recieve bat and ball states
		sf::Vector2f batPos;
		sf::Vector2f ballPos;
		socket.receive(packet);
		
		//Update the positions
		if (packet >> batPos.x >> batPos.y >> ballPos.x >> ballPos.y) {
			cout << "newPos" << batPos.x << " " << batPos.y << endl;
			cout << "oldPos" << bat.getPosition().x << " " << bat.getPosition().y << endl;
			bat.update(batPos);
			ball.update(ballPos);
		}

		//Draw
		window.clear(sf::Color(26, 128, 281, 255));
		window.draw(bat.getShape());
		window.draw(ball.getShape());

		window.display();
	}

	//Safely disconnect the socket
	socket.disconnect();
	return 0;
}