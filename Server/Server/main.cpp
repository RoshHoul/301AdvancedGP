#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <list>

#include "BallCalculations.h"
#include "BatCalculations.h"

using namespace std;

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
	
	//SetBlocking false, so the client doesn't have to wait for packets
	socketPlayer1.setBlocking(false);

	//Error catching if connections are unsuccessful.
	if (listener.listen(port) != sf::Socket::Done) {
		std::cout << "Check if port is free" << endl;
		exit(EXIT_FAILURE);
	}

	if (listener.accept(socketPlayer1) != sf::Socket::Done) {
		std::cout << "Error accepting socket1" << endl;
		exit(EXIT_FAILURE);
	}

	while (window.isOpen()) {

		//Using selector so multiple clients can connect. Not working
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

		//The length of a signle frame
		last_frame = clock.getElapsedTime();
		//Boolean used for packets fixed time step;
		bool updateNetwork = false;

		//Fixed time step
		if (packetClock.getElapsedTime().asSeconds() >= interval) {
			updateNetwork = true;
			packetClock.restart();
		}

		//Check if client is connected;
		if (socketPlayer1.getRemotePort() == 0) {
			cout << "Client Disconnected" << endl;
		}
		else {
			cout << "Client connected" << endl;
		}
		while (window.pollEvent(e)) {

			if (e.type == sf::Event::Closed) {
				window.close();
			}


		}


		socketPlayer1.receive(packet);
		packet >> receiveBatState;

		//Timer for player movement
		sf::Time time_diff = clock.getElapsedTime() - last_frame;

		//Calculate client position. Case of 0 moves left, case of 1 moves right. Any other case, the client isn't updated
		sf::Vector2f pos = player1.getPosition();
		if (receiveBatState == 0) {

			player1.moveLeft(time_diff);
			pos = player1.getPosition();

		}
		else if (receiveBatState == 1) {
			player1.moveRight(time_diff);
			pos = player1.getPosition();
		}
		else {
			pos = player1.getPosition();
		}

		//Ball movement calculations
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
			ball.rebountBatOrTop();
		}

		//Calculate the ball and client positions
		ball.update(time_diff);
		player1.update();

		//Assign the last calculated position;
		sf::Vector2f batPos = player1.getPosition();
		sf::Vector2f ballPos = ball.getVecPosition();

		//Update ball and clients positions (client side) after all calculations done.
		if (updateNetwork) {
			//cout << "Sending data: " << batPos.x << ", " << batPos.y << " ---- " << ballPos.x << ", " << ballPos.y << endl;
			packet << batPos.x << batPos.y << ballPos.x << ballPos.y;
			socketPlayer1.send(packet);
			packetClock.restart();
		}
	}
}