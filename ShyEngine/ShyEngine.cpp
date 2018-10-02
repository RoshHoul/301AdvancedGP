// ShyEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 800), "Don't Expect Much!", sf::Style::Default, sf::ContextSettings(32));
	sf::CircleShape circle(100.0f);
	
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer); //TUK GURMI
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	
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

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		window.display();
	}
	
	return 0;
}

