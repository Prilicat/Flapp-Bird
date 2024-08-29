/*
Copyright (c) 2024 Nicolas G.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
