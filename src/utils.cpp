#include "utils.h"
#include <cmath>
#include <stdlib.h>
#include <time.h>

#define MAX_X 16000
#define MAX_Y 9000
void setOriginToCenter(sf::Sprite &object)
{
    auto rect = object.getGlobalBounds();
    object.setOrigin(rect.width/2., rect.height/2.);
}

void setOriginToCenter(sf::Shape &object)
{
    auto rect = object.getGlobalBounds();
    object.setOrigin(rect.width/2., rect.height/2.);
}

void setOriginToCenter(sf::Text &object)
{
    auto rect = object.getGlobalBounds();
    object.setOrigin(rect.width/2., rect.height);
}



void scaleToMinSize(sf::Sprite &sp, double size_x, double size_y)
{
    double scalingFactor = std::max(size_x/sp.getTextureRect().width, size_y/sp.getTextureRect().height);
    sp.scale(scalingFactor, scalingFactor);
}

void scaleToMaxSize(sf::Sprite &sp, double size_x, double size_y)
{
    double scalingFactor = std::min(size_x/sp.getTextureRect().width, size_y/sp.getTextureRect().height);
    sp.scale(scalingFactor, scalingFactor);
}

void scaleByRadius(sf::Sprite &sp, int radius)
{
    double scalingFactor = 2*radius/std::sqrt(sp.getTextureRect().width*sp.getTextureRect().width + sp.getTextureRect().height*sp.getTextureRect().height);
    sp.scale(scalingFactor, scalingFactor);
}

void setTextCenterPosition(sf::Text &txt, sf::Vector2f center)
{
    sf::Vector2f offset;
    offset.x = txt.getPosition().x - txt.getGlobalBounds().left - txt.getGlobalBounds().width/2.;
    offset.y = txt.getPosition().y - txt.getGlobalBounds().top - txt.getGlobalBounds().height/2.;

    txt.setPosition(center + offset);
}

bool check(float x, float y, float *listex, float *listey, int size) {
    int i = 0;
    float norm = sqrt((x-listex[i])*(x-listex[i])+(y-listey[i])*(y-listey[i]));
    while (norm > 300 && i<size-1) {
        i=i+1;
        norm = sqrt((x-listex[i])*(x-listex[i])+(y-listey[i])*(y-listey[i]));
    }
    if (i==size-1 && norm >300) {
        return true;
    }
    return false;
}

std::vector<sf::Vector2f> randomCP(int nbCP) {
    std::vector<sf::Vector2f> checkpointsPositions;
    srand(time(NULL));
    float listex[nbCP];
    float listey[nbCP];
    float x = rand() % MAX_X;
    float y = rand() % MAX_Y;
    listex[0]=x;
    listey[0]=y;
    sf::Vector2f v(x,y);
    checkpointsPositions.emplace_back(v);
    for (int i=1; i<nbCP; ++i) {
        x = rand() % MAX_X;
        y = rand() % MAX_Y;
        while (check(x,y,listex,listey,i)==false){
            x = rand() % MAX_X;
            y = rand() % MAX_Y;
        }
        listex[i]=x;
        listey[i]=y;
        sf::Vector2f v(x,y);
        checkpointsPositions.emplace_back(v);

    }
    return checkpointsPositions;
}