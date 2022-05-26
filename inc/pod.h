#ifndef INC_POD_H
#define INC_POD_H

#include <SFML/System.hpp>
#include <cmath>
#include <SFML/Audio.hpp>
#include "checkpoint.h"
#include "utils.h"

class Game;

struct Decision
{
    Decision(sf::Vector2f target, float power);

    sf::Vector2f target_;
    float power_;
    bool IA_;
};

class Pod
{
    public :
    Pod(sf::Vector2f pos, float angle, sf::Vector2f vel = {0, 0});

    Decision getDecision(Pod &pod, std::vector<CheckPoint> otherCPs_,FinalCheckPoint finalCP_) const; 

    void addLap();
    int getLap();
    void changeMode();

    private :
    sf::Vector2f pos_, vel_; //position and velocity vectors
    float angle_; //angle en degres
    int nextCP_, lapCount_;
    
    bool IA_;
    bool autopilot_;
    //bonus
    int champignon_;

    //attaque
    //laser
    int attaque_;
    int timer_attaque_;
    //stun
    int being_touched_;
    int timer_touched_;
    //bouclier
    int bouclier_;
    int timer_bouclier_;
    //asteroide
    int asteroide_pose_;
    int asteroide_timer_;
    //tempete
    int tempete_;
    int tempete_timer_;
    //missile
    int missile_;
    int missile_timer_;
    //bullet
    int bullet_;
    int bullet_timer_;
    //etoile
    int etoile_;
    int etoile_timer_;

    sf::Sound nyanCatAudio;
    sf::SoundBuffer nyanCatBuffer;

    sf::Sound etoileAudio;
    sf::SoundBuffer etoileBuffer;

    sf::Sound bulletBillAudio;
    sf::SoundBuffer bulletBillBuffer;

    sf::Sound windAudio;
    sf::SoundBuffer windBuffer;

    sf::Sound laserAudio;
    sf::SoundBuffer laserBuffer;

    sf::Sound bouclierAudio;
    sf::SoundBuffer bouclierBuffer;

    sf::Sound missileAudio;
    sf::SoundBuffer missileBuffer;


    
    
    
    
     
       
    friend class Game; //allows game to modify pod's private attributes

};

#endif