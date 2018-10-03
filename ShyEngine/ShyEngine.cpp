// ShyEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;

GLuint positionBufferObject;

const float vertexPositions[] = {
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,
};

//void InitializeVertexBuffer() {
//	glGenBuffers(1, &positionBufferObject);
//	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
//}

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 800), "Don't Expect Much!", sf::Style::Default, sf::ContextSettings(32));
	
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	glewInit();
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer); //TUK GURMI
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	bool running = true;

	while (running) {

		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				running = false;
			}
			else if (ev.type == sf::Event::Resized) {
				glViewport(0, 0, ev.size.width, ev.size.height);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		window.display();
	}
	
	return 0;
}

