#ifndef INC_POD_H
#define INC_POD_H

#include <SFML/System.hpp>
#include <cmath>
#include "checkpoint.h"

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
    float angle_; //angle in radians
    int nextCP_, lapCount_;
    bool IA_;

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

    
    
    
    
    friend class Game; //allows game to modify pod's private attributes

};

#endif