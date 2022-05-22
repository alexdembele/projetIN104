#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include "../inc/checkpoint.h"
#include "../inc/utils.h"
//#include "game.h"

CheckPoint::CheckPoint(sf::Vector2f center, unsigned int id) : circle_(600)
{
    setOriginToCenter(circle_);
    circle_.setPosition(center);
    circle_.setFillColor(sf::Color(0,0,0,63));
    circle_.setOutlineColor(sf::Color(0,0,0));
    circle_.setOutlineThickness(-50);

    font_.loadFromFile("../repository/Fredoka-Bold.ttf");
    fillingText_ = sf::Text(std::to_string(id), font_, 500);
    setOriginToCenter(fillingText_);
    fillingText_.setOutlineThickness(100);
    fillingText_.setPosition(center);
};

void CheckPoint::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(circle_, states);
    target.draw(fillingText_);
};

sf::Vector2f CheckPoint::getPosition()
{
    return circle_.getPosition();
}


FinalCheckPoint::FinalCheckPoint(sf::Vector2f center) : circle_(600)
{
    setOriginToCenter(circle_);
    circle_.setPosition(center);
    circle_.setFillColor(sf::Color(0,0,0,63));
    circle_.setOutlineColor(sf::Color(0,0,0));
    circle_.setOutlineThickness(-50);

    fillingTexture_.loadFromFile("../repository/Images/checkeredFlag.png");
    fillingSprite_.setTexture(fillingTexture_);
    setOriginToCenter(fillingSprite_);
    fillingSprite_.setPosition(center);
    fillingSprite_.scale(sf::Vector2f(2.f, 2.f));

};

void FinalCheckPoint::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(circle_, states);
    target.draw(fillingSprite_, states);
};

sf::Vector2f FinalCheckPoint::getPosition()
{
    return circle_.getPosition();
}

