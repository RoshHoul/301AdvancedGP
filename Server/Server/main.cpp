#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <list>

#include "BallCalculations.h"
#include "BatCalculations.h"
/* Note that we are including both the 32-bit and 64-bit versions of SFML
(downloadable from their site) as ENet can be temperamental dependent upon OS.
The 64-bit version (with all library directories and libraries included for you)
seems to work on University computers. If it fails you may need to enable your
IDE to load symbols for DLLs (Tools -> Options -> Debugging -> Symbols, and check
the box 'Microsoft Symbol Servers' before cleaning and rebuilding the project).

REMEMBER: You don't have to use ENet for the coursework, of course, and this
example is purely illustrative. */

using namespace std;

/* We begin by creating a data structure - we're using a Vector2 in our example,
which holds two floats to represent a 2D position, but you should be able to see
how this can be replaced by any abstract data structure. It can even be replaced
with a generic bin, as was discussed in Lecture 2. */





void main()
{

	int windowWidth = 1024;
	int windowHeight = 768;
	sf::RenderWindow window(sf::VideoMode(504, 504), "Server");
	sf::Event e;

	sf::Clock clock;
	sf::Time last_frame;

	sf::TcpListener listener;

	sf::Clock packetClock;
	float tickRate = 60;
	float interval = 1 / tickRate;

	sf::TcpSocket socketPlayer1;
	sf::TcpSocket socketPlayer2;

	std::list<sf::TcpSocket*> clients;

	sf::SocketSelector selector;

	int port = 2000;
	sf::Int32 sendBatState;
	sf::Int32 receiveBatState = -1;


	sf::Packet packet;
	selector.add(listener);

	BatCalculations player1(windowWidth / 2, windowHeight - 20);
	BallCalculations ball(windowWidth / 2, windowHeight / 2);

	window.setFramerateLimit(60);

	socketPlayer1.setBlocking(false);
	if (listener.listen(port) != sf::Socket::Done) {
		std::cout << "Check if port is free" << endl;
		exit(EXIT_FAILURE);
	}

	if (listener.accept(socketPlayer1) != sf::Socket::Done) {
		std::cout << "Error accepting socket1" << endl;
		exit(EXIT_FAILURE);
	}

	while (window.isOpen()) {

		if (selector.wait()) {
			if (selector.isReady(listener)) {
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done) {
					clients.push_back(client);
					selector.add(*client);
				}
				else {
					delete client;
				}
			}
			else {
				for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it) {
					sf::TcpSocket& client = **it;
					if (selector.isReady(client)) {
						sf::Packet packet;
						if (client.receive(packet) == sf::Socket::Done) {

						}
					}
				}
			}
		}

		last_frame = clock.getElapsedTime();
		bool updateNetwork = false;

		if (packetClock.getElapsedTime().asSeconds() >= interval) {
			updateNetwork = true;
			packetClock.restart();
		}

		if (socketPlayer1.getRemotePort() == 0) {
			cout << "Client Disconnected" << endl;
		}
		else {

			//		cout << "client connected" << endl;
		}

		while (window.pollEvent(e)) {

			if (e.type == sf::Event::Closed) {
				window.close();
			}


		}


		socketPlayer1.receive(packet);

		if (packet >> receiveBatState) {

		}

		sf::Time time_diff = clock.getElapsedTime() - last_frame;

		sf::Vector2f pos = player1.getPosition();
		if (receiveBatState == 0) {

			player1.moveLeft(time_diff);
			pos = player1.getPosition();

		}
		else if (receiveBatState == 1) {
			player1.moveRight(time_diff);
			pos = player1.getPosition();
		}

		if (ball.getPosition().top > windowHeight) {
			ball.hitBottom();

		}

		if (ball.getPosition().top < 0) {
			ball.rebountBatOrTop();
		}

		if (ball.getPosition().left < 0 || ball.getPosition().left + 10 > windowWidth) {
			ball.reboundSides();
		}

		if (ball.getPosition().intersects(player1.getBounds())) {
			cout << "TOPKI V BATATA" << endl;
			ball.rebountBatOrTop();
		}

		ball.update(time_diff);
		player1.update();

		sf::Vector2f batPos = player1.getPosition();
		sf::Vector2f ballPos = ball.getVecPosition();

		if (updateNetwork) {
			//cout << "Sending data: " << batPos.x << ", " << batPos.y << " ---- " << ballPos.x << ", " << ballPos.y << endl;
			packet << batPos.x << batPos.y << ballPos.x << ballPos.y;
			socketPlayer1.send(packet);
			packetClock.restart();
		}
	}

	
}