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
    IA_=true;
    champignon_=-1;
};




Decision Pod::getDecision(Pod &pod, std::vector<CheckPoint> otherCPs_, FinalCheckPoint finalCP_) const
{   
    float power=30.f;

    if (pod.IA_==false) {
        //champignon
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) && pod.champignon_<0)
        {
            pod.champignon_+=1;
        }
        else if(pod.champignon_>=0 && pod.champignon_<=100)
        {
            power*=2;
            pod.champignon_+=1;
        }

        
        //clavier
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            // left key is pressed: move our character
            sf::Vector2f target=pod.pos_-1000.f*sf::Vector2f(0,1);
            return Decision(target, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            // left key is pressed: move our character
            sf::Vector2f target=pod.pos_-1000.f*sf::Vector2f(0,1);
            return Decision(target, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            // left key is pressed: move our character
            sf::Vector2f target=pod.pos_+1000.f*sf::Vector2f(-1,1);
            return Decision(target, power);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            // left key is pressed: move our character
            sf::Vector2f target=pod.pos_+1000.f*sf::Vector2f(-1,-1);
            return Decision(target, power);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            // left key is pressed: move our character
            sf::Vector2f target=pod.pos_+1000.f*sf::Vector2f(1,1);
            return Decision(target, power);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            // left key is pressed: move our character
            sf::Vector2f target=pod.pos_+1000.f*sf::Vector2f(1,-1);
            return Decision(target, power);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            // left key is pressed: move our character
            sf::Vector2f target=pod.pos_-1000.f*sf::Vector2f(0,1);
            return Decision(target, power);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            // left key is pressed: move our character
                
        
        
            sf::Vector2f target= pod.pos_+1000.f*sf::Vector2f(1,0);
            return Decision(target, power);
            
                
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            // left key is pressed: move our character
                
            
            sf::Vector2f target= pod.pos_+1000.f*sf::Vector2f(0,1);
            return Decision(target, power);
                
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            // left key is pressed: move our character
                
            
            
            
            sf::Vector2f target= pod.pos_-1000.f*sf::Vector2f(1,0);
            return Decision(target, power);
            


        }
        return Decision(pod.pos_+1000.f*pod.vel_/sqrt(pod.vel_.x*pod.vel_.x+pod.vel_.y*pod.vel_.y),0);
    } else {

        //IA NORMALE
        //on sauvegarde la position du next checkpoint pour ce pod
        int nextCP=pod.nextCP_; 
        if (nextCP!=-1) {

            //on va chercher la position du checkpoint pour ce pod
            sf::Vector2f position_nextCP=otherCPs_[nextCP].getPosition();

            //on retourne la decision
            return Decision(position_nextCP,power);
        } else {
            sf::Vector2f position_nextCP=finalCP_.getPosition();
            return Decision(position_nextCP,power);
        }
    }
    
    //default
    //return Decision(sf::Vector2f(1000.f,1000.f),power);
};