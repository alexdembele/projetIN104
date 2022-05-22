#include "../inc/pod.h"
#include "../inc/game.h"
#include <SFML/System/Vector2.hpp>

Decision::Decision(sf::Vector2f target, float power) 
{
    target_=target;
    power_=power;
};

Pod::Pod(sf::Vector2f pos, float angle, sf::Vector2f vel) 
{
    pos_=pos;
    vel_=vel;
    angle_=angle; //angle in radians
    nextCP_=0;
    lapCount_=0;
};

Decision Pod::getDecision(Pod pod, std::vector<CheckPoint> otherCPs_, FinalCheckPoint finalCP_) const
{   
    //on sauvegarde la position du next checkpoint pour ce pod
    int nextCP=pod.nextCP_;
    if (nextCP!=-1) {

        //on va chercher la position du checkpoint pour ce pod
        sf::Vector2f position_nextCP=otherCPs_[nextCP].getPosition();

        //on retourne la decision
        return Decision(position_nextCP,20.f);
    } else if (nextCP==-1) {
        sf::Vector2f position_nextCP=finalCP_.getPosition();
        return Decision(position_nextCP,20.f);
    }

    /*
    //clavier
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        // left key is pressed: move our character
            
        printf("gauche");
        if (abs(pod.angle_+ 90)>18)
        {
            sf::Vector2f target= pod.pos_+1000.f*sf::Vector2f(cos(M_PI/10),sin(M_PI/10));
            return Decision({16000, 4500}, 10);
        }
            
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        // left key is pressed: move our character
            
        printf("haut"); 
            
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        // left key is pressed: move our character
            
        printf("bas"); 
            
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        // left key is pressed: move our character
            
        printf("doite"); 
            
    }

    //IA

    //defaut
    return Decision({16000, 4500}, 10);
    */

    
};