#ifndef INC_CHECKPOINT_H
#define INC_CHECKPOINT_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include "utils.h"

class CheckPoint : public sf::Drawable
{
    public :
    CheckPoint(sf::Vector2f center, unsigned int id); //creates a Checkpoint from a position and its number
    sf::Vector2f getPosition(); //returns Checkpoint's position

    //private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::CircleShape circle_;
    sf::Font font_;
    sf::Text fillingText_;
};

class FinalCheckPoint : public sf::Drawable
{
    public :
    FinalCheckPoint(sf::Vector2f center);
    sf::Vector2f getPosition();

    private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::CircleShape circle_;
    sf::Texture fillingTexture_;
    sf::Sprite fillingSprite_;
};



#endif