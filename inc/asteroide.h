#ifndef INC_ASTEROIDE_H
#define INC_ASTEROIDE_H

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>


class Asteroide {

    public :

    //private:
    sf::Texture tex_;
    sf::Sprite sp_;
    sf::Vector2f pos_;

};



#endif