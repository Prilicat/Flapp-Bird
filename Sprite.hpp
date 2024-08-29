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

#ifndef SPRITEHPP
#define SPRITEHPP

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <map>

namespace nik {
	static const sf::IntRect NULL_INTRECT{0, 0, 0, 0};

	class NikSprite {
	public:
		sf::Texture Texture;
		sf::Sprite Sprite;

		NikSprite() {}
	};

	// Lista de sprite carregados
	std::map < std::string, NikSprite* > SpriteMap;

	/* Registra sprite com o apelido */
	void RegSprite(const std::string& Alias="", const sf::IntRect& Rect=NULL_INTRECT, const std::string& Path="") {
		SpriteMap[Alias] = new NikSprite;
		
		if (Rect == NULL_INTRECT ? SpriteMap[Alias]->Texture.loadFromFile(Path) 
			: SpriteMap[Alias]->Texture.loadFromFile(Path, Rect)) {
			std::cout << "Texture loaded... ";
		}

		SpriteMap[Alias]->Sprite.setTexture(SpriteMap[Alias]->Texture);
		std::cout << "Sprite loaded... " << '\n';
	}

	/* Retorna o sprite correspondente ao apelido do mesmo */
	inline sf::Sprite& RetSprite(const std::string& Alias) {
		return SpriteMap[Alias]->Sprite;
	}

	/* Retorna a textura correspondente ao apelido da mesma */
	inline sf::Texture& RetTexture(const std::string& Alias) {
		return SpriteMap[Alias]->Texture;
	}
}

#endif SPRITEHPP
