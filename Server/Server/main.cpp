#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
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
	sf::RenderWindow window(sf::VideoMode(504, 504), "Server");
	sf::Event e;

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	int port = 2000;
	sf::Int32 sendBatState;
	sf::Int32 receiveBatState = -1;

	sf::TcpListener listener;


	socket.setBlocking(false);
	if (listener.listen(port) != sf::Socket::Done) {
		std::cout << "Check if port is free" << endl;
		exit(EXIT_FAILURE);
	}

	if (listener.accept(socket) != sf::Socket::Done) {
		std::cout << "Error accepting socket" << endl;
		exit(EXIT_FAILURE);
	}

	while (window.isOpen()) {


		if (socket.getRemotePort() == 0) {
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

		sf::Packet packet;
		
		socket.receive(packet);
		packet >> receiveBatState;

		sf::Int32 tempPacket;
		if (packet >> tempPacket) {
			receiveBatState = tempPacket;
		}
		else {
			//cout << "kur" << endl;
		}
		

		cout << "sled BAT STATE: " << receiveBatState << endl;
	}
}