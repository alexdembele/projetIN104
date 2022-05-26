#ifndef INC_GameLOGIC_H
#define INC_GameLOGIC_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include "checkpoint.h"
#include "pod.h"
#include <math.h>

const unsigned int NUMBER_OF_LAPS = 3;
const float FRICTION_COEFF = 0.85;
const float ROTA_MAX = M_PI/10.;
const sf::Time PHYSICS_TIME_STEP = sf::milliseconds(100);
const std::string IMAGES_PATH = "../repository/Images/";
const std::string FONT_PATH = "../repository/";


struct GameInfos
{
    std::vector<Pod> pods;
    std::vector<sf::Vector2f> cpPositions;
};

struct Asteroide {
    sf::Texture tex_;
    sf::Sprite sp_;
    sf::Vector2f pos_;

};

struct missile {
    sf::Texture texture_;
    sf::Sprite sprite_;
    sf::Vector2f pos_;
    float angle_;
    sf::Vector2f vel_;
    int cible_;
};

struct laser {
    sf::RectangleShape shape_;
    sf::Vector2f pos_;
    float angle_;
    sf::Vector2f vel_;
};

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

    struct laser laser_;

    sf::Texture tex_bouclier;
    sf::Sprite sp_bouclier;

    sf::Texture tex_bouclier_used;
    sf::Sprite sp_bouclier_used;

    sf::Texture tex_bouclier_pod;
    sf::Sprite sp_bouclier_pod;

    sf::Texture tex_champi;
    sf::Sprite sp_champi;

    sf::Text text;
    sf::Font font;
    
    sf::Text text_IA;
    sf::Font font_IA;

    sf::Texture tex_tempete;
    sf::Sprite sp_tempete;

    void attaque_laser(Pod pod);
    void attaque_missile(std::vector<Pod> pods_, int nbPods_);
    bool isTouched(Pod pod);

    Asteroide asteroide_;

    missile missile_;

    sf::Sound laserAudio;
    sf::SoundBuffer laserBuffer;
    
    sf::Sound plopAudio;
    sf::SoundBuffer plopBuffer;

    
    sf::Sound etoileAudio;
    sf::SoundBuffer etoileBuffer;

    sf::Sound windAudio;
    sf::SoundBuffer windBuffer;

    sf::Sound missileHitAudio;
    sf::SoundBuffer missileHitBuffer;

};

#endif