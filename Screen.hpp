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

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "Entity.hpp"
#include "Sprite.hpp"

sf::Font Font;

class Screen {
public:
	bool Back{};

	Screen() {}

	virtual void Init() {};

	virtual void Update(sf::RenderWindow& Window) {}

	virtual void Render(sf::RenderWindow& Window) {}

	virtual bool GamePause() { return false; }
};

Screen* CurrentScreen{ nullptr };

namespace nik {
	void InitScreen() {
		Font.loadFromFile("res/font0.ttf");
	}

	void ChangeScreen(Screen* NewScreen) {
		CurrentScreen = NewScreen;
		CurrentScreen->Init();
	}
}

class GuiInGame :public Screen {
public:
	GuiInGame() {}

	EntPlayer Birds[2] { EntPlayer(), EntPlayer() };
	sf::Clock PointCounter[2];
	sf::Clock GameTimer;

	sf::SoundBuffer BufferMusic;
	sf::Sound Music;

	int CurrentJoyStickCount{0};

	struct Pipe {
		float X{800};
		float Height{};

		Pipe() {
			Height = 100+std::rand() % 200;
		}
	};

	std::vector<Pipe*> Pipes;

	sf::Clock PipeSpawn;

	void Init() override {
		BufferMusic.loadFromFile("res/music.mp3");
		Music = sf::Sound(BufferMusic);
		Music.setLoop(true);
		Music.play();

		int JoyCount{};
		for (int i = 0; i < 1; i++) {
			if (sf::Joystick::isConnected(i))
				JoyCount++;
		}
		CurrentJoyStickCount = JoyCount;

		for (int i = 0; i < CurrentJoyStickCount; i++) {
			Birds[i].Died = true;
			Birds[i].Position.x = 200 - (i * 70);
		}
	}

	float Speed = 3;

	void Update(sf::RenderWindow& Window) override {

		for (int i = 0; i < CurrentJoyStickCount; i++) {
			Birds[i].MotionY += 0.37f;
			if (sf::Joystick::isConnected(i)) {
				if (sf::Joystick::isButtonPressed(i, 0) && Birds[i].MotionY > 0) {
					if (!Birds[i].Died) {
						Birds[i].MotionY = -7.0f;

						Birds[i].Buffer.loadFromFile("res/fow.mp3");
						Birds[i].Sound = sf::Sound(Birds[i].Buffer);
						Birds[i].Sound.play();
					}
					else if (Birds[i].Position.y > Window.getView().getSize().y-60) {
						Birds[i].Died = false;
						Birds[i].MotionY = 0;
						Birds[i].Position.y = (Window.getView().getSize().y / 2.0F);
						Birds[i].Position.x = 200 - (i*70);
					}
				}
				Birds[i].Position += {0, Birds[i].MotionY};
			}

			if (Birds[i].Point > Birds[i].PointMax)
				Birds[i].PointMax = Birds[i].Point;

			if (Birds[i].Died)
				Birds[i].Position.x -= Speed;
		}

		if (GameTimer.getElapsedTime().asSeconds() >= 3 && PipeSpawn.getElapsedTime().asSeconds() >= 1.5F) {
			Pipes.push_back(new Pipe());
			PipeSpawn.restart();
		}

		if (Sun > 254) {
			Night = true;
		}
		else if (Sun < 30)
			Night = false;

		Sun += Night ? -0.16f : 0.16f;

		OffsetX -= Speed;
		for (Pipe* p : Pipes) {
			if (p == nullptr)
				continue;
			p->X -= Speed;
		}
	}

	float OffsetX{0};

	float Sun{ 255 };
	bool Night{ true };

	sf::RectangleShape Base;
	sf::RectangleShape Base1;
	sf::RectangleShape Sky;
	sf::Text Points;

	void Render(sf::RenderWindow& Window) override {
		Sky.setSize({ Window.getView().getSize().x, Window.getView().getSize().y });
		Sky.setFillColor({ static_cast<sf::Uint8> (Sun > 150 ? 150 : Sun),static_cast<sf::Uint8> (Sun > 150 ? 150 : Sun), static_cast<sf::Uint8> (Sun) });

		Window.draw(Sky);

		Base.setPosition({ OffsetX,  Window.getView().getSize().y - 40 });
		Base.setSize({ 800, 200 });
		Base.setTexture(&nik::RetTexture("base"));

		Base1.setPosition({ OffsetX + 800,  Window.getView().getSize().y - 40 });
		Base1.setSize({ 800, 200 });
		Base1.setTexture(&nik::RetTexture("base"));

		if (OffsetX <= -800)
			OffsetX = 0;

		for (int i = 0; i < CurrentJoyStickCount; i++) {
			if (!Birds[i].Died)
				continue;
			sf::RectangleShape GhostFlappy;
			GhostFlappy.setSize({ 60.0F, 50.0F });
			GhostFlappy.setPosition({ 200.0F - (i * 70.0F), Window.getView().getSize().y / 2.0F });
			GhostFlappy.setRotation(0);
			GhostFlappy.setOrigin({ 25.0F, 25.0F });
			GhostFlappy.setTexture(&nik::RetTexture("bird1"));

			if (i != 1) {
				GhostFlappy.setFillColor({ 255, 0, 0, 120 });
			}
			else
				GhostFlappy.setFillColor({ 0, 0, 255, 120 });

			Window.draw(GhostFlappy);
		}

		int Counter = 0;
		for (Pipe* p : Pipes) {
			if (p == nullptr)
				continue;
			sf::RectangleShape Pipe;
			Pipe.setSize({ 100.0F, 400.0F });
			Pipe.setPosition({ p->X, p->Height });
			Pipe.setTexture(&nik::RetTexture("pipe"));
			Pipe.setScale(1, -1);

			sf::RectangleShape Pipe1;
			Pipe1.setSize({ 100.0F, 400.0F });
			Pipe1.setPosition({ p->X, p->Height + 150 });
			Pipe1.setTexture(&nik::RetTexture("pipe"));

			// Collision, Point Detection
			for (int i = 0; i < CurrentJoyStickCount; i++) {
				if (Birds[i].Position.x >= p->X && Birds[i].Position.x <= p->X + 100) {
					if (PointCounter[i].getElapsedTime().asSeconds() > 1.1f) {
						PointCounter[i].restart();
						Birds[i].Point++;
					}

					if (Birds[i].Position.y >= p->Height + 120 ||
						Birds[i].Position.y <= p->Height + 30) {
						if (!Birds[i].Died) {
							if (((int)(std::rand() % 10)) == 1) {
								Birds[i].Buffer.loadFromFile("res/yell.mp3");
								Birds[i].Sound = sf::Sound(Birds[i].Buffer);
								Birds[i].Sound.play();
							}
							else {
								Birds[i].Buffer.loadFromFile("res/bonk.mp3");
								Birds[i].Sound = sf::Sound(Birds[i].Buffer);
								Birds[i].Sound.play();
							}

							Birds[i].Died = true;
						}

						Birds[i].Point = 0;
					}
				}
			}

			Window.draw(Pipe);
			Window.draw(Pipe1);

			if (p->X < 200) { // If pipe a is off the screen, remove it
				delete Pipes[Counter];
				Pipes[Counter] = nullptr;
				continue;
			}
			Counter++;
		}

		Window.draw(Base);
		Window.draw(Base1);


		float PointOffsetY{};
		Points.setFont(Font);
		Points.setPosition(20, 20);
		Points.setCharacterSize(30);

		for (int i = 0; i < CurrentJoyStickCount; i++) {
			if (Birds[i].Position.y > Window.getView().getSize().y - 50) {
				Birds[i].Position.y = Window.getView().getSize().y - 50;
				Birds[i].MotionY = 90;
			}


			sf::RectangleShape Flappy;
			Flappy.setSize({ 60.0F, 50.0F });
			Flappy.setPosition(Birds[i].Position);
			Flappy.setRotation(Birds[i].MotionY * 5.0F);
			Flappy.setOrigin({ 25.0F, 25.0F });
			Flappy.setTexture(Birds[i].MotionY < 1 ? &nik::RetTexture("bird2") : (Birds[i].MotionY > -0.0F && Birds[i].MotionY < 2.0f) ? &nik::RetTexture("bird1") : &nik::RetTexture("bird0"));

			if (i != 1) {
				Flappy.setFillColor({255, 0, 0});
			} else
				Flappy.setFillColor({ 0, 0, 255 });

			sf::Text Point;
			Point.setFont(Font);
			Point.setPosition(Flappy.getPosition() - sf::Vector2f{-20, 90});
			Point.setOrigin(30, 30);
			Point.setCharacterSize(60);
			Point.setString(std::to_string(Birds[i].Point));

			if (Birds[i].Point > 14) {
				Point.setFillColor({255, 0, 255});
			} else 
			if (Birds[i].Point > 8) {
				Point.setFillColor({ 255, 0, 0 });
			}
			else {
				Point.setFillColor({ 0, 255, 0 });
			}

			Points.move(0, PointOffsetY);
			Points.setString("Record Player " + std::to_string(i+1) + " : " + std::to_string(Birds[0].PointMax));
			Window.draw(Points);
			PointOffsetY += 20;

			Window.draw(Flappy);
			Window.draw(Point);

		}
	}
};

class GuiMain:public Screen {
public:
	GuiMain() {}

	sf::Clock SelectedDelay;
	int Selected{};
	bool Select{};
	const char* MenuList[4] = { "Play Now!", "Exit" };

	void Init() override {}

	sf::SoundBuffer Buffer;
	sf::Sound Sound;
	void Update(sf::RenderWindow& Window) override {

		if (sf::Joystick::isConnected(0)) {
			if (SelectedDelay.getElapsedTime().asSeconds() > 0.3f) {
				if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) > 90) {
					Selected++;
					SelectedDelay.restart();
				}
				if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) < -90) {
					Selected--;
					SelectedDelay.restart();
				}
				if (SelectedDelay.getElapsedTime().asSeconds() <= 0.05) {
					Buffer.loadFromFile("res/menu-selection.mp3");
					Sound = sf::Sound(Buffer);
					Sound.play();
				}
			}
			Select = sf::Joystick::isButtonPressed(0, 0);
		}

		if (Selected > 1)
			Selected = 0;
		else if (Selected < 0)
			Selected = 1;
	}

	void Render(sf::RenderWindow& Window) override {
		float Offset = 80.0f;

		sf::Text Title;
		Title.setFont(Font);
		Title.setPosition({ 10, 20 });
		Title.setFillColor(sf::Color::White);
		Title.setString("Flappo Birdo");
		Title.setCharacterSize(40);

		for (int Selection = 0; Selection < 2; Selection++) {
			sf::RectangleShape MenuRect;
			MenuRect.setPosition({ 10, Offset });
			MenuRect.setSize({ 200.0f, 60.0f });
			MenuRect.setFillColor({ 155, 155, 155 });
			MenuRect.setOutlineColor({ 105, 105, 105 });


			sf::Text Label;
			Label.setFont(Font);
			Label.setPosition({ 14, 20 + Offset });
			Label.setFillColor(sf::Color::White);
			Label.setString(MenuList[Selection]);
			Label.setCharacterSize(15);

			if (Selection == Selected) {
				Label.move({ 5, 5 });
				MenuRect.setFillColor({ 100, 255, 100 });
				MenuRect.setScale({ 1.2f, 1.2f });
				Label.setCharacterSize(20);


				if (Select) {
					switch (Selection) {
					case 0:
						nik::ChangeScreen(new GuiInGame);
						break;
					case 1:
						Window.close();
						break;
					}

					Buffer.loadFromFile("res/menu-button.mp3");
					Sound = sf::Sound(Buffer);
					Sound.play();
				}
			}

			Window.draw(MenuRect);
			Window.draw(Label);
			Offset += Selection == Selected ? 80 : 70;
		}
		Window.draw(Title);
	}

	bool GamePause() override {
		return true;
	}
};
