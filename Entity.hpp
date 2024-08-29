#pragma once

#include <SFML/System/Vector2.hpp>

class Entity {
public:
	sf::Vector2f Position; // Entity current position
	float MotionY{}; // Fall acceleration
	int Point { 0 }, PointMax{0};
	bool Died; // if entity is dead
	
	// Entity current sound
	sf::SoundBuffer Buffer;
	sf::Sound Sound;


	Entity() {
	
	}
};

class EntPlayer:public Entity {
public:
	EntPlayer(const sf::Vector2f& Position) {
		this->Position = Position;
	}

	EntPlayer() {

	}
};