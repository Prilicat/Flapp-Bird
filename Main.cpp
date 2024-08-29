#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#include "Sprite.hpp"
#include "Screen.hpp"

using namespace std;

static inline void InitGame() {
	/*nik::RegSprite("grass", sf::IntRect{0, 0, 8, 8}, "res/objects.png");*/
	nik::RegSprite("bird0", nik::NULL_INTRECT, "res/bird0.png");
	nik::RegSprite("bird1", nik::NULL_INTRECT, "res/bird1.png");
	nik::RegSprite("bird2", nik::NULL_INTRECT, "res/bird2.png");
	nik::RegSprite("pipe", nik::NULL_INTRECT, "res/pipe.png");
	nik::RegSprite("base", nik::NULL_INTRECT, "res/base.png");
	nik::InitScreen();

	nik::ChangeScreen(new GuiMain());
}

static void Update(sf::RenderWindow& Render) {
	CurrentScreen->Update(Render);
}

static void Render(sf::RenderWindow& Render) {
	CurrentScreen->Render(Render);
}

int main(int argc, char* argv[]) {
	sf::RenderWindow WindowRender{ sf::VideoMode{ 800, 600 }, "Flappo Birdo!", sf::Style::Default };

	WindowRender.setFramerateLimit(60);
	
	InitGame();
	
	while (WindowRender.isOpen()) {
		sf::Event E;
		sf::Clock DeltaTime;
		while (WindowRender.pollEvent(E)) {
			if (E.type == sf::Event::Closed)
				WindowRender.close();
		}
		WindowRender.clear({100, 100, 250});
		Update(WindowRender);
		Render(WindowRender);
		WindowRender.display();
	}
	delete CurrentScreen;
	return 0;
}