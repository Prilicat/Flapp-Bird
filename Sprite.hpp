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