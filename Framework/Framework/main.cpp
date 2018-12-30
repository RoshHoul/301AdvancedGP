#include <iostream>
#include <SFML\Graphics.hpp>
#include <enet\enet.h>
#include <cstring>

using namespace std;

/* If you haven't read through the Server sample code and comments yet, DO THAT FIRST! */

/* So, we recreate our data structure. This step isn't strictly necessary, if we're using
an advanced scheme for defining data elements within our packet (as discussed in Lecture 2)
but for this example it helps keep things clear. A 'Vector2' means the same thing to both
programs. */

struct Vector2 {
	float x;
	float y;
};

void main()
{
	

	/* SFML preamble, defining our Client avatar (avatar) and our Enemy avatar (enemy),
	along with the necessary Window generation/event declaration. */

	sf::RenderWindow window(sf::VideoMode(800, 600), "Client Window");
	sf::Event e;

	sf::CircleShape avatar(20.0f);
	avatar.setFillColor(sf::Color::Blue);
	avatar.setPosition(sf::Vector2f(200.0f, 300.0f));

	sf::CircleShape enemy(20.0f);
	enemy.setFillColor(sf::Color::Red);
	enemy.setPosition(sf::Vector2f(600.0f, 300.0f));

	if (enet_initialize() != 0)
	{
		cout << "Enet failed to initialise!" << "\n\n";
	}

	/* Much of this will be familiar from the server code. What differs
	is the inclusion of an Peer. This is basically ENet's way of tracking
	what you're connected to. Since our Client will be initialising the
	connection, they need a peer to store information about the server they're
	connecting to. */

	ENetAddress address;
	ENetHost* client;
	ENetPeer* peer;
	ENetEvent enetEvent;

	client = enet_host_create(NULL,	1, 2, 0, 0);

	if (client == NULL)
	{
		cout << "Client failed to initialise!" << "\n\n";
	}

	enet_address_set_host(&address, "localhost");
	address.port = 1234;

	/* Now we're trying to connect to the server. We include who we are,
	the address to establish the connection to, the number of channels, and a
	generic data variable we can use to say something to the server (currently 0). */

	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL) {
		cout << "No available peers for initializing an ENet connection.\n";
	}

	/* To make our lives a little easier, we declare this newPosition variable on
	the heap. It's where the data from our received packet is going to be stored,
	if it reaches us. */

	Vector2* newPosition = new Vector2;

	while (window.isOpen()) {

		window.pollEvent(e);

		/* Usual SFML event polling stuff. Note that we disconnect from the server
		when we close down, but we don't wait for confirmation of the disconnection
		in this example. */

		if (e.type == sf::Event::Closed) {
			
			if (peer != NULL)
			{
				enet_peer_disconnect_now(peer, 0);
			}

			window.close();
		}

		while (enet_host_service(client, &enetEvent, 0) > 0)
		{
			switch (enetEvent.type) {

			/* Again, we're reacting based on the detected event type. In this case, it's
			ENET_EVENT_TYPE_RECEIVE, which means our client has recieved a packet from a
			peer (in this case, the server). After quickly outputting some debug text to
			console to confirm packet receipt, what happens next is the key part.
			
			Our packet has some intrinsic variables - its data (what it's storing) and its
			dataLength (how much data it's storing). In this case, since we KNOW the packet
			is a Vector2, we can use the memcpy function fairly easily. This is a basic C++
			function which copies a given amount of data from one location to another. In
			this case, it copies TO newPosition FROM data, and it copies an amount dataLength.
			
			Given what we know about pointer arithmetic, it should be obvious to us that we
			can make these packets more sophisticated. We can make huge packets which hold
			many different kinds of data. We simply include an enumerator at the beginning of
			each data segment, saying what data type it is, and either copy that much data over
			into a variable of that type, or include as the next element of a packet the amount
			of data this variable type needs to copy. This is particularly useful when it comes
			to Part 2 of the coursework, where 'level data' is likely very different to the
			'physics data' you'll have been transmitting for Part 1. */

			case ENET_EVENT_TYPE_RECEIVE:
				cout << "Packet received!\n";
				memcpy(newPosition, enetEvent.packet->data, enetEvent.packet->dataLength);
				cout << newPosition->x << "," << newPosition->y << "\n";
				enemy.setPosition(sf::Vector2f(newPosition->x, newPosition->y));
				break;
			}
		}

		if (e.type == sf::Event::KeyPressed)
		{
			/* Here we include some code to control our own avatar or shut down
			the program. */

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				avatar.move(sf::Vector2f(-0.5f, 0.0f));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				avatar.move(sf::Vector2f(0.5f, 0.0f));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				avatar.move(sf::Vector2f(0.0f, -0.5f));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				avatar.move(sf::Vector2f(0.0f, 0.5f));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				if (peer != NULL)
				{
					enet_peer_disconnect_now(peer, 0);
				}

				window.close();
			}
		}

		/* SFML draw calls */

		window.clear(sf::Color::Green);
		window.draw(avatar);
		window.draw(enemy);
		window.display();

	}

	/* We delete newPosition, destroy the client instance, and deinitialise ENet. */

	delete newPosition;

	enet_host_destroy(client);
	atexit(enet_deinitialize);
}