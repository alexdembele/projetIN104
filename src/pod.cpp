#include "pod.h"
#include "game.h"
#include "utils.h"
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>



Decision::Decision(sf::Vector2f target, float power) 
{   
    target_=target;
    power_=power;
};

Pod::Pod(sf::Vector2f pos, float angle, sf::Vector2f vel) 
{
    pos_=pos;
    vel_=vel;
    angle_=angle; //angle en degrés
    
    nextCP_=0;
    lapCount_=0;
    
    IA_=true;
    autopilot_=false;
    
    //Bonus
    champignon_=-1;
    
    being_touched_=0;
    timer_touched_=-1;
    
    bouclier_=0;
    timer_bouclier_=-1;
    
    attaque_=0;
    timer_attaque_=-1;

    asteroide_pose_=0;
    asteroide_timer_=-1;

    tempete_=0;
    tempete_timer_=-1;

    missile_=0;
    missile_timer_=-1;

    bullet_=0;
    bullet_timer_=-1;

    etoile_=0;
    etoile_timer_=-1;

    //chargement Son
    nyanCatBuffer.loadFromFile("../repository/Sons/nyanCat10s.wav");
    nyanCatAudio.setBuffer(nyanCatBuffer);

    etoileBuffer.loadFromFile("../repository/Sons/etoile.wav");
    etoileAudio.setBuffer(etoileBuffer);

    
    windBuffer.loadFromFile("../repository/Sons/wind-sound-effect.wav");
    windAudio.setBuffer(windBuffer);

    bulletBillBuffer.loadFromFile("../repository/Sons/bulletBill.wav");
    bulletBillAudio.setBuffer(bulletBillBuffer);

    bouclierBuffer.loadFromFile("../repository/Sons/bouclier.wav");
    bouclierAudio.setBuffer(bouclierBuffer);

 
    laserBuffer.loadFromFile("../repository/Sons/laser.wav");
    laserAudio.setBuffer(laserBuffer);  

    missileBuffer.loadFromFile("../repository/Sons/missile.wav");
    missileAudio.setBuffer(missileBuffer);

};




Decision Pod::getDecision(Pod &pod, std::vector<CheckPoint> otherCPs_, FinalCheckPoint finalCP_) const
{   
    float power=50.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        pod.autopilot_=!pod.autopilot_;
    }

    if (pod.IA_==false && pod.autopilot_==false) {
        //asteroide
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && pod.asteroide_timer_<0 && pod.asteroide_pose_==0) {
            pod.asteroide_timer_+=1;
            pod.asteroide_pose_=1;
        } else if(pod.asteroide_timer_>=0 && pod.asteroide_timer_<=300 && pod.asteroide_pose_==1) {
            pod.asteroide_timer_+=1;
        } else if (pod.asteroide_timer_==301 && pod.asteroide_pose_==1) {
            pod.asteroide_pose_=-1;
            pod.asteroide_timer_=0;
        } else if (pod.asteroide_timer_>=0 && pod.asteroide_timer_<=100 && pod.asteroide_pose_==-1) {
            pod.asteroide_timer_+=1;
        } else if (pod.asteroide_timer_==101 && pod.asteroide_pose_==-1) {
            pod.asteroide_timer_=-1;
            pod.asteroide_pose_=0;
        }

        //missile
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::M) && pod.missile_timer_<0 && pod.missile_==0) {
            pod.missile_timer_+=1;
            pod.missile_=1;
            pod.missileAudio.play();
        } else if(pod.missile_timer_>=0 && pod.missile_timer_<=50 && pod.missile_==1) {
            pod.missile_timer_+=1;
        } else if (pod.missile_timer_==51 && pod.missile_==1) {
            pod.missile_=-1;
            pod.missile_timer_=0;
        } else if (pod.missile_timer_>=0 && pod.missile_timer_<=100 && pod.missile_==-1) {
            pod.missile_timer_+=1;
        } else if (pod.missile_timer_==101 && pod.missile_==-1) {
            pod.missile_timer_=-1;
            pod.missile_=0;
        }

        //champignon
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::C) && pod.champignon_<0 && pod.etoile_!=1)
        {
            pod.champignon_+=1;
            pod.nyanCatAudio.play();
        }
        else if(pod.champignon_>=0 && pod.champignon_<=100)
        {
            power*=2;
            pod.champignon_+=1;
        }

        //tempete
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::T) && pod.tempete_timer_<0 && pod.tempete_==0) {
            pod.tempete_timer_+=1;
            pod.tempete_=1;
        } else if(pod.tempete_timer_>=0 && pod.tempete_timer_<=100 && pod.tempete_==1) {
            pod.tempete_timer_+=1;
        } else if (pod.tempete_timer_==101 && pod.tempete_==1) {
            pod.tempete_=-1;
            pod.tempete_timer_=0;
        } else if (pod.tempete_timer_>=0 && pod.tempete_timer_<=100 && pod.tempete_==-1) {
            pod.tempete_timer_+=1;
        } else if (pod.tempete_timer_==101 && pod.tempete_==-1) {
            pod.tempete_timer_=-1;
            pod.tempete_=0;
        }
        
        //bouclier
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::B) && pod.timer_bouclier_<0 && pod.bouclier_==0 && pod.etoile_!=1) {
            pod.timer_bouclier_+=1;
            pod.bouclier_=1;
            pod.bouclierAudio.play();
        } else if(pod.timer_bouclier_>=0 && pod.timer_bouclier_<=100 && pod.bouclier_==1) {
            pod.timer_bouclier_+=1;
        } else if (pod.timer_bouclier_==101 && pod.bouclier_==1) {
            pod.bouclier_=-1;
            pod.timer_bouclier_=0;
        } else if (pod.timer_bouclier_>=0 && pod.timer_bouclier_<=100 && pod.bouclier_==-1) {
            pod.timer_bouclier_+=1;
        } else if (pod.timer_bouclier_==101 && pod.bouclier_==-1) {
            pod.timer_bouclier_=-1;
            pod.bouclier_=0;
        }

        //laser
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::L) && pod.timer_attaque_<0 && pod.attaque_==0) {
            pod.timer_attaque_+=1;
            pod.attaque_=1;
            pod.laserAudio.play();
        } else if(pod.timer_attaque_>=0 && pod.timer_attaque_<=35 && pod.attaque_==1) {
            pod.timer_attaque_+=1;
        } else if (pod.timer_attaque_==36 && pod.attaque_==1) {
            pod.attaque_=-1;
            pod.timer_attaque_=0;
        } else if (pod.timer_attaque_>=0 && pod.timer_attaque_<=2 && pod.attaque_==-1) {
            pod.timer_attaque_+=1;
        } else if (pod.timer_attaque_==3 && pod.attaque_==-1) {
            pod.timer_attaque_=-1;
            pod.attaque_=0;
        }
        
        //etoile
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) && pod.etoile_timer_<0 && pod.etoile_==0 && pod.champignon_!=1 && pod.bouclier_!=1) {
            pod.etoile_timer_+=1;
            pod.etoile_=1;
            power*=2;
            pod.bouclier_=1;
            pod.etoileAudio.play();
        } else if(pod.etoile_timer_>=0 && pod.etoile_timer_<=100 && pod.etoile_==1) {
            pod.etoile_timer_+=1;
            power*=2;
            pod.bullet_timer_=1;
        } else if (pod.etoile_timer_==101 && pod.etoile_==1) {
            pod.etoile_=-1;
            pod.etoile_timer_=0;
            pod.bouclier_=0;
        } else if (pod.etoile_timer_>=0 && pod.etoile_timer_<=100 && pod.etoile_==-1) {
            pod.etoile_timer_+=1;
        } else if (pod.etoile_timer_==101 && pod.etoile_==-1) {
            pod.etoile_timer_=-1;
            pod.etoile_=0;
        }

        //bullet

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F) && pod.bullet_timer_<0 && pod.bullet_==0 && pod.bouclier_!=1 && pod.champignon_!=1 && pod.etoile_!=1) {
            pod.bullet_timer_+=1;
            pod.bullet_=1;
            power*=2;
            pod.bouclier_=1;
            pod.autopilot_=true;
            pod.bulletBillAudio.play();
        }

        
        //clavier
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            sf::Vector2f target=pod.pos_-1000.f*sf::Vector2f(0,1);
            return Decision(target, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            sf::Vector2f target=pod.pos_-1000.f*sf::Vector2f(0,1);
            return Decision(target, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            sf::Vector2f target=pod.pos_+1000.f*sf::Vector2f(-1,1);
            return Decision(target, power);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            sf::Vector2f target=pod.pos_+1000.f*sf::Vector2f(-1,-1);
            return Decision(target, power);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            sf::Vector2f target=pod.pos_+1000.f*sf::Vector2f(1,1);
            return Decision(target, power);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)&&sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            sf::Vector2f target=pod.pos_+1000.f*sf::Vector2f(1,-1);
            return Decision(target, power);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            sf::Vector2f target=pod.pos_-1000.f*sf::Vector2f(0,1);
            return Decision(target, power);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            sf::Vector2f target= pod.pos_+1000.f*sf::Vector2f(1,0);
            return Decision(target, power);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            sf::Vector2f target= pod.pos_+1000.f*sf::Vector2f(0,1);
            return Decision(target, power);    
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            sf::Vector2f target= pod.pos_-1000.f*sf::Vector2f(1,0);
            return Decision(target, power);
        }
        //action par defaut
        return Decision(pod.pos_+1000.f*pod.vel_/norme(pod.vel_),0);
    }

    //bullet
    if (pod.bullet_timer_>=0 && pod.bullet_timer_<=100 && pod.bullet_==1) {
        pod.bullet_timer_+=1;
        power*=2;
        pod.bouclier_=1;
        pod.autopilot_=true;
    } else if (pod.bullet_timer_==101 && pod.bullet_==1) {
        pod.bullet_=-1;
        pod.bullet_timer_=0;
        pod.bouclier_=0;
        pod.autopilot_=false;
    } else if (pod.bullet_timer_>=0 && pod.bullet_timer_<=100 && pod.bullet_==-1) {
        pod.bullet_timer_+=1;
    } else if (pod.bullet_timer_==101 && pod.bullet_==-1) {
        pod.bullet_timer_=-1;
        pod.bullet_=0;
    }

    if (pod.IA_==true || pod.autopilot_==true || pod.bullet_==1) {

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
    
    return Decision(sf::Vector2f(1000.f,1000.f),power);
};

void Pod::changeMode(){
    IA_=!(IA_);
};

void Pod::addLap(){
    lapCount_++;
};

int Pod::getLap(){
    return lapCount_;
};