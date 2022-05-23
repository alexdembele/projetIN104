#ifndef INC_GameLOGIC_H
#define INC_GameLOGIC_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "checkpoint.h"
#include "pod.h"
#include <math.h>

const unsigned int NUMBER_OF_LAPS = 3;
const float FRICTION_COEFF = 0.85;
const float ROTA_MAX = M_PI/10.;
const sf::Time PHYSICS_TIME_STEP = sf::milliseconds(100);
const std::string IMAGES_PATH = "../repository/Images/";
const std::string FONT_PATH = "../repository/";

class Game : public sf::Drawable
{
    public :
    Game(); //create a Game with 2 to 8 checkpoints at random positions (they shouldn't overlap)
    Game(std::vector<sf::Vector2f> checkpointsPositions,int nbCP); //create a Game with predefined checkpoints positions

    void addPod(int nbPods,std::vector<sf::Vector2f> positionPods); //can add various arguments here to chose the apparence or the decision making type of the pod
    void updatePhysics();
    void updateGraphics(sf::Time frameTime); //change pods sprites positions and rotations depending on the frame time
    
    // private :
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Texture backgroundTexture_;
    sf::Sprite backgroundSprite_;

    std::vector<Pod> pods_;
    std::vector<sf::Texture> podsTextures_;
    std::vector<sf::Sprite> podsSprites_;

    FinalCheckPoint finalCP_;
    std::vector<CheckPoint> otherCPs_;

    sf::Time lastFrameTime;
    sf::Time physicsTime;

    int nbCP_;
    int nbPods_;

    sf::RectangleShape laser_;

    sf::Texture tex_bouclier;
    sf::Sprite sp_bouclier;

    sf::Texture tex_bouclier_used;
    sf::Sprite sp_bouclier_used;

    sf::Texture tex_champi;
    sf::Sprite sp_champi;

    sf::Text text;
    sf::Font font;

    void attaque(Pod pod);


};

#endif