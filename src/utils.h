#ifndef INC_UTILS_H
#define INC_UTILS_H

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

void setOriginToCenter(sf::Sprite &object);
void setOriginToCenter(sf::Shape &object);
void setOriginToCenter(sf::Text &object);

void scaleToMinSize(sf::Sprite &sp, double size_x, double size_y);
void scaleToMaxSize(sf::Sprite &sp, double size_x, double size_y);
void scaleByRadius(sf::Sprite &sp, int radius);

void setTextCenterPosition(sf::Text &txt, sf::Vector2f center);

bool check(float x, float y, float *listex, float *listey, int size);
std::vector<sf::Vector2f> randomCP(int nbCP);

#endif