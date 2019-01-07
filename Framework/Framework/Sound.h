#pragma once

#include <SFML\Audio.hpp>  

class Sound {
private:
	sf::SoundBuffer pBuffer;
	sf::Sound pSound;
public:

	Sound() {}
	Sound(std::string filename) {
		pBuffer.loadFromFile(filename);
		pSound.setBuffer(pBuffer);
	}

	void setLoop(bool state) {
		pSound.setLoop(state);
	}

	void setVolume(float volume) {
		pSound.setVolume(volume);
	}

	void play() {
		pSound.play();
	}
};

