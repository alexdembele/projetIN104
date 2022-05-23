#include "../inc/game.h"
#include "../inc/checkpoint.h"
#include "../inc/utils.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "pod.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <iostream>
#include <math.h>

Game::Game(std::vector<sf::Vector2f> checkpointsPositions, int nbCP) : finalCP_(checkpointsPositions[0])
{   
    nbPods_=0;
    nbCP_=nbCP;
    //la premier ligne construit le final checkpoint a partir de la position
    //la boucle suivante construit les autres checkpoints a partir de la position
    otherCPs_.reserve(nbCP-1);
    for (int cpID=1; cpID < nbCP; cpID++) {
        otherCPs_.emplace_back(checkpointsPositions[cpID],cpID);
    }

    //initialisation des time
    lastFrameTime=sf::seconds(0.f);
    physicsTime=sf::seconds(0.f);

    //initialisation du background
    backgroundTexture_.loadFromFile("../repository/Images/background.png");
    backgroundSprite_.setTexture(backgroundTexture_);
    scaleToMinSize(backgroundSprite_,16000,9000);

    //laser
    laser_.shape_=sf::RectangleShape(sf::Vector2f(500.f,70.f));
    setOriginToCenter(laser_.shape_);
    laser_.shape_.setFillColor(sf::Color::Green);
    laser_.shape_.setOutlineThickness(10);
    laser_.shape_.setOutlineColor(sf::Color::Black);
    laser_.angle_=0.f;
    laser_.pos_=sf::Vector2f(0.f,0.f);
    laser_.vel_=sf::Vector2f(0.f,0.f);

    //affichage texte
    font.loadFromFile("../repository/Fredoka-Bold.ttf");
    text.setFont(font);
    text.setCharacterSize(400);
    text.setFillColor(sf::Color::Black);


    //affichage bonus
    tex_champi.loadFromFile("../repository/Images/champignon.png");
    sp_champi.setTexture(tex_champi);
    setOriginToCenter(sp_champi);
    sp_champi.setPosition(sf::Vector2f(15500.f,500.f));
    scaleToMinSize(sp_champi,800,800);

    tex_bouclier.loadFromFile("../repository/Images/bouclier.png");
    sp_bouclier.setTexture(tex_bouclier);
    setOriginToCenter(sp_bouclier);
    sp_bouclier.setPosition(sf::Vector2f(14500.f,500.f));
    scaleToMinSize(sp_bouclier,800,800);


    tex_bouclier_used.loadFromFile("../repository/Images/bouclier_used.png");
    sp_bouclier_used.setTexture(tex_bouclier_used);
    setOriginToCenter(sp_bouclier_used);
    sp_bouclier_used.setPosition(sf::Vector2f(14500.f,500.f));
    scaleToMinSize(sp_bouclier_used,800,800);
}

void Game::addPod(int nbPods,std::vector<sf::Vector2f> positionPods)
{   
    //on reserve l'emplacement pour les pods, textures et sprite
    nbPods_+=nbPods;
    pods_.reserve(nbPods);
    podsTextures_.reserve(nbPods);
    podsSprites_.reserve(nbPods);
    for (int i=0; i<nbPods;++i) {
        podsTextures_.emplace_back();
        podsSprites_.emplace_back();
        
        //on cree le pod
        pods_.emplace_back(positionPods[i],0.f,sf::Vector2f(10.f,10.f));

        //on lui donne une texture + on l'applique au sprite + on le scale + on setorigin
        if (i!=0) {
            podsTextures_[i].loadFromFile("../repository/Images/SWMilleniumFalcon.png");
        } else {
            podsTextures_[i].loadFromFile("../repository/Images/NMSFighterG.png");
        }
        podsSprites_[i].setTexture(podsTextures_[i]);
        setOriginToCenter(podsSprites_[i]);
        podsSprites_[i].setPosition(positionPods[i]);
        scaleToMinSize(podsSprites_[i],800,800);
        //printf("%f;%f\n",positionPods[i].x,positionPods[i].y);


    }
    


}

void Game::updatePhysics()
{  
    int nbPod=pods_.size();
    for(int i=0;i<nbPod;i++)
    {   
        //printf("decalage:%f\n",decalageAngle);
        Decision decision=pods_[i].getDecision(pods_[i],otherCPs_,finalCP_);
        sf::Vector2f pod_target = decision.target_;
        sf::Vector2f pod_pos = pods_[i].pos_;
        sf::Vector2f vecteur_vers_target = pod_target-pod_pos;
        //sf::Vector2f vecteur_vers_direction = pods_[i].vel_;
        float decalageAngle = angle(sf::Vector2f(0.00000001f,0.f),vecteur_vers_target)-pods_[i].angle_;

        if (decalageAngle>=180) {
            decalageAngle-=360;
        } else if (decalageAngle<=-180) {
            decalageAngle+=360;
        }

        //printf("%f   %f\n",pod_pos.x,pod_target.x);

        //si le decalageAngle est superieur a pi/10 on fait un decalage de pi/10
        if(abs(decalageAngle)>18.f)
        {   
            
            //calcul du vecteur vers le target intermediaire
            //vecteur vers la target intermediaire dans le repere du pod
            float decalage_intermediaire=0.f;
            //on check si le decalage est vers le haut ou le bas
            if (decalageAngle>0) {
                decalage_intermediaire=(-decalageAngle+18)*(M_PI/180.f);
            } else if (decalageAngle<=0) {
                decalage_intermediaire=(-decalageAngle-18)*(M_PI/180.f);
            }
            
            float Nx=cos(decalage_intermediaire)*vecteur_vers_target.x-sin(decalage_intermediaire)*vecteur_vers_target.y; 
            float Ny=sin(decalage_intermediaire)*vecteur_vers_target.x+cos(decalageAngle)*vecteur_vers_target.y;
            
            sf::Vector2f target_intermediaire;
            target_intermediaire=sf::Vector2f (Nx,Ny);
            
            float norme_vintermediaire= sqrt(target_intermediaire.x*target_intermediaire.x+target_intermediaire.y*target_intermediaire.y);
            
            //calcul vitesse
            pods_[i].vel_=0.85f*(pods_[i].vel_+decision.power_*(target_intermediaire/norme_vintermediaire));
            
            //calcul position
            pods_[i].pos_=pods_[i].pos_+pods_[i].vel_;

            //calcul angle
            if (decalageAngle>0) {
                pods_[i].angle_=pods_[i].angle_+18.f;
            } else if (decalageAngle<=0) {
                pods_[i].angle_=pods_[i].angle_-18.f;
            }

            //test si sur checkpoint et si lapcount
            //printf("%d\n",pods_[i].nextCP_);
            if (norme_vintermediaire < 300.f && pods_[i].IA_==true) {
                if (pods_[i].nextCP_<nbCP_-2) {
                    pods_[i].nextCP_=pods_[i].nextCP_+1;
                } else if (pods_[i].nextCP_==nbCP_-2) {
                    pods_[i].nextCP_=-1;
                }
                if (pods_[i].nextCP_==0) {
                pods_[i].lapCount_=pods_[i].lapCount_+1;
                }
            }
            //printf("%f   %f\n",Nx,Ny);
            //printf("%f   %f    %f\n",decalage_intermediaire, decalageAngle,pods_[i].angle_);
            //printf("%f;%f    %f;%f\n",pod_pos.x,pod_pos.y,target_intermediaire.x+pod_pos.x,target_intermediaire.y+pod_pos.y);

        } else {
          
            float norme = sqrt(vecteur_vers_target.x*vecteur_vers_target.x+vecteur_vers_target.y*vecteur_vers_target.y);
            
            //calcul vitesse
            pods_[i].vel_=0.85f*(pods_[i].vel_+decision.power_*(vecteur_vers_target/norme));
            
            //calcul position
            pods_[i].pos_=pods_[i].pos_+pods_[i].vel_;

            //calcul angle
            pods_[i].angle_=pods_[i].angle_+decalageAngle;

            
            //test si sur checkpoint et si lapcount
            if (norme < 300  && pods_[i].IA_==true) {
                if (pods_[i].nextCP_<nbCP_-2) {
                    pods_[i].nextCP_=pods_[i].nextCP_+1;
                } else if (pods_[i].nextCP_==nbCP_-2) {
                    pods_[i].nextCP_=-1;
                }
                if (pods_[i].nextCP_==0) {
                    pods_[i].lapCount_=pods_[i].lapCount_+1;
                }
            }
            
        }
        if (pods_[i].IA_==false) {   
            float dist;
            if (pods_[i].nextCP_>=0) {
                sf::Vector2f podCheckpoint= pods_[i].pos_ - otherCPs_[pods_[i].nextCP_].getPosition();
                dist=sqrt(podCheckpoint.x*podCheckpoint.x+podCheckpoint.x*podCheckpoint.x);
            } else {
                sf::Vector2f podCheckpoint= pods_[i].pos_ - finalCP_.getPosition();
                dist=sqrt(podCheckpoint.x*podCheckpoint.x+podCheckpoint.x*podCheckpoint.x);
            }

            if (dist < 300) {
                if (pods_[i].nextCP_<nbCP_-2) {
                    otherCPs_[pods_[i].nextCP_].fillingText_.setFillColor(sf::Color::Green);
                    pods_[i].nextCP_=pods_[i].nextCP_+1;
                } else if (pods_[i].nextCP_==nbCP_-2) {
                    otherCPs_[pods_[i].nextCP_].fillingText_.setFillColor(sf::Color::Green);
                    pods_[i].nextCP_=-1;
                }
                if (pods_[i].nextCP_==0) {
                    for (int j=0;j<nbCP_-1;++j) {
                        otherCPs_[j].fillingText_.setFillColor(sf::Color::White);
                    }
                    pods_[i].lapCount_=pods_[i].lapCount_+1;
                }
            }
            //printf("%d\n",pods_[i].nextCP_);
        }


        //printf("%f;%f    %f;%f\n",pod_pos.x,pod_pos.y,pod_target.x+pod_pos.x,pod_target.y+pod_pos.y);
        //printf("physics:%f\n",pods_[i].angle_);
    }
    if (pods_[0].timer_attaque_==0) {
        attaque(pods_[0]);
    }
    if (pods_[0].timer_attaque_>0 && pods_[0].timer_attaque_<=101) {
        laser_.pos_=laser_.pos_+laser_.vel_;
        laser_.shape_.setPosition(laser_.pos_);
        printf("%f  %f\n",laser_.pos_.x,laser_.pos_.y);
    }
    printf("%d  %d\n", pods_[0].attaque_,pods_[0].timer_attaque_);
    //printf("%f;%f\n",pods_[0].pos_.x,pods_[0].pos_.y);
    

    physicsTime += PHYSICS_TIME_STEP;
}


void Game::updateGraphics(sf::Time frameTime)
{
    if (frameTime==physicsTime) 
    {
        int nbPod=pods_.size();
        for(int i=0;i<nbPod;i++)
        {
            podsSprites_[i].setPosition(pods_[i].pos_);
            podsSprites_[i].setRotation(pods_[i].angle_);
        }
    }

    else
    {
        int nbPod=pods_.size();
        for(int i=0;i<nbPod;i++)
        {
            
            podsSprites_[i].move(((physicsTime-frameTime)/PHYSICS_TIME_STEP)*(pods_[i].pos_-podsSprites_[i].getPosition())); //bouge sprite avec fraction temporelle
            //int angle_pod=int(pods_[i].angle_);
            //int angle_int=angle_pod%360;
            //float angle = angle_int;
            //podsSprites_[i].rotate(((physicsTime-frameTime)/PHYSICS_TIME_STEP)*(angle-podsSprites_[i].getRotation())); //tourne sprite avec fraction temporelle
            //probleme modulo
            //printf("%f;%f\n",angle,podsSprites_[2].getRotation());
        }
    }
}    


void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(backgroundSprite_, states);
    target.draw(finalCP_, states);

    for (const auto &cp : otherCPs_)
    {
        target.draw(cp, states);
    }

    for (const auto &podSprite : podsSprites_)
    {
        target.draw(podSprite, states);
    }

    //bonus actif
    if (pods_[0].champignon_>=0 && pods_[0].champignon_<=100 ) {
        target.draw(sp_champi);
    }
    if (pods_[0].bouclier_==1) {
        target.draw(sp_bouclier);
    }
    if (pods_[0].bouclier_==-1) {
        target.draw(sp_bouclier_used);
    }

    if (pods_[0].attaque_==1) {
        target.draw(laser_.shape_);
    }
}


void Game::attaque(Pod pod) {
        laser_.vel_=300.f*pod.vel_/float (sqrt(pod.vel_.x*pod.vel_.x+pod.vel_.y*pod.vel_.y));
        laser_.pos_= pod.pos_ + 3.f*laser_.vel_;
        laser_.angle_=pod.angle_;
        laser_.shape_.setRotation(laser_.angle_);
        laser_.shape_.setPosition(laser_.pos_);
    }