#include "game.h"
#include "checkpoint.h"
#include "utils.h"
#include "pod.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>


//constructeur de Game
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
    laser_.pos_=sf::Vector2f(-1000.f,-1000.f);
    laser_.vel_=sf::Vector2f(0.f,0.f);

    //affichage texte du score
    font.loadFromFile("../repository/Fredoka-Bold.ttf");
    text.setFont(font);
    text.setCharacterSize(400);
    text.setFillColor(sf::Color::Black);

    //affichage IA
    font_IA.loadFromFile("../repository/Fredoka-Bold.ttf");
    text_IA.setFont(font_IA);
    text_IA.setCharacterSize(400);
    text_IA.setFillColor(sf::Color::Green);
    text_IA.setString("IA");
    text_IA.setPosition(sf::Vector2f(100.f,8500.f));


    //affichage bonus
    //champignon
    tex_champi.loadFromFile("../repository/Images/champignon.png");
    sp_champi.setTexture(tex_champi);
    setOriginToCenter(sp_champi);
    sp_champi.setPosition(sf::Vector2f(15500.f,500.f));
    scaleToMinSize(sp_champi,800,800);

    //tempete
    tex_tempete.loadFromFile("../repository/Images/tempete.png");
    sp_tempete.setTexture(tex_tempete);
    setOriginToCenter(sp_tempete);
    sp_tempete.setPosition(sf::Vector2f(8000.f,4500.f));
    scaleToMinSize(sp_tempete,16000,9000);

    //bouclier
    tex_bouclier.loadFromFile("../repository/Images/bouclier.png");
    sp_bouclier.setTexture(tex_bouclier);
    setOriginToCenter(sp_bouclier);
    sp_bouclier.setPosition(sf::Vector2f(14500.f,500.f));
    scaleToMinSize(sp_bouclier,800,800);

    //bouclier used
    tex_bouclier_used.loadFromFile("../repository/Images/bouclier_used.png");
    sp_bouclier_used.setTexture(tex_bouclier_used);
    setOriginToCenter(sp_bouclier_used);
    sp_bouclier_used.setPosition(sf::Vector2f(14500.f,500.f));
    scaleToMinSize(sp_bouclier_used,800,800);

    //etoile
    tex_etoile.loadFromFile("../repository/Images/etoile.png");
    sp_etoile.setTexture(tex_etoile);
    setOriginToCenter(sp_etoile);
    sp_etoile.setPosition(sf::Vector2f(13500.f,500.f));
    scaleToMinSize(sp_etoile,800,800);

    //bouclier sur le pod
    tex_bouclier_pod.loadFromFile("../repository/Images/bouclier_pod.png");
    sp_bouclier_pod.setTexture(tex_bouclier_pod);
    setOriginToCenter(sp_bouclier_pod);
    sp_bouclier_pod.setPosition(sf::Vector2f(0.f,0.f));
    scaleToMinSize(sp_bouclier_pod,2500,2500);

    //asteroide
    asteroide_.tex_.loadFromFile("../repository/Images/asteroide.png");
    asteroide_.sp_.setTexture(asteroide_.tex_);
    setOriginToCenter(asteroide_.sp_);
    asteroide_.sp_.setPosition(sf::Vector2f(-1000.f,-1000.f));
    scaleToMinSize(asteroide_.sp_,800,800);
    asteroide_.pos_=sf::Vector2f(-1000.f,-1000.f);

    //missile
    missile_.texture_.loadFromFile("../repository/Images/missile.png");
    missile_.sprite_.setTexture(missile_.texture_);
    setOriginToCenter(missile_.sprite_);
    missile_.sprite_.setPosition(sf::Vector2f(-1000.f,-1000.f));
    scaleToMinSize(missile_.sprite_,800,800);
    missile_.angle_=0.f;
    missile_.pos_=sf::Vector2f(-1000.f,-1000.f);
    missile_.vel_=sf::Vector2f(0.f,0.f);
    missile_.cible_=0;

    //bullet bill : on change la texture du pod
    tex_bullet_.loadFromFile("../repository/Images/NMSFighterY.png");

    //son
    //asteroide
    plopBuffer.loadFromFile("../repository/Sons/asteroide.wav");
    plopAudio.setBuffer(plopBuffer);

    //MissileHit
    missileHitBuffer.loadFromFile("../repository/Sons/missileHit.wav");
    missileHitAudio.setBuffer(missileHitBuffer);
    
    
}

//ajout de pods
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


    }
}





//on update la physique avec un pas de temps plus long
void Game::updatePhysics()
{   
    //pour l'affichage du score
    std::string score=""; 
    
    int nbPod=pods_.size();
    for(int i=0;i<nbPod;i++)
    {   
        //on calcule le decalage d'angle entre le vecteur vitesse du pod et le vecteur "vers la target"
        Decision decision=pods_[i].getDecision(pods_[i],otherCPs_,finalCP_);
        sf::Vector2f pod_target = decision.target_;
        sf::Vector2f pod_pos = pods_[i].pos_;
        sf::Vector2f vecteur_vers_target = pod_target-pod_pos;
        float decalageAngle = angle(sf::Vector2f(0.00000001f,0.f),vecteur_vers_target)-pods_[i].angle_;

        //on a ajouté ces lignes car le domaine de definition des angles est [-180,+180]
        if (decalageAngle>=180) {
            decalageAngle-=360;
        }
        if (decalageAngle<=-180) {
            decalageAngle+=360;
        }

        //si le decalageAngle est superieur a pi/10 on fait un decalage plafonné à pi/10
        if(abs(decalageAngle)>18.f)
        {   
            float decalage_intermediaire=0.f;
            //on check si le decalage est vers le haut ou le bas
            if (decalageAngle>0) {
                //on doit faire une rotation de decalageAngle+(pi/10) à notre vecteur "vers la target"
                decalage_intermediaire=(-decalageAngle+18)*(M_PI/180.f);
            } else if (decalageAngle<=0) {
                //on doit faire une rotation de decalageAngle-(pi/10) à notre vecteur "vers la target"
                decalage_intermediaire=(-decalageAngle-18)*(M_PI/180.f);
            }
            
            //on applique la matrice de rotation d'angle "decalage_intermediaire" à notre vecteur "vers la target"
            float Nx=cos(decalage_intermediaire)*vecteur_vers_target.x-sin(decalage_intermediaire)*vecteur_vers_target.y; 
            float Ny=sin(decalage_intermediaire)*vecteur_vers_target.x+cos(decalage_intermediaire)*vecteur_vers_target.y;
            
            //vecteur finalement obtenu vers la target virtuelle
            sf::Vector2f target_intermediaire;
            target_intermediaire=sf::Vector2f (Nx,Ny);
            
            //calcul de la norme
            float norme_vintermediaire=norme(target_intermediaire);
            
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

            //test pour savoir si on est sur checkpoint et si on incremente lapcount
            if (norme_vintermediaire < 600.f && pods_[i].IA_==true) {
                if (pods_[i].nextCP_<nbCP_-2) {
                    pods_[i].nextCP_=pods_[i].nextCP_+1;
                } else if (pods_[i].nextCP_==nbCP_-2) {
                    pods_[i].nextCP_=-1;
                }
                if (pods_[i].nextCP_==0) {
                pods_[i].lapCount_=pods_[i].lapCount_+1;
                }
            }
        
        //si le decalage d'angle est inferieur a pi/10, on fait une rotation de decalageAngle
        } else {
          
            //calcul de la norme
            float norm = norme(vecteur_vers_target);
            
            //calcul vitesse
            pods_[i].vel_=0.85f*(pods_[i].vel_+decision.power_*(vecteur_vers_target/norm));
            
            //calcul position
            pods_[i].pos_=pods_[i].pos_+pods_[i].vel_;

            //calcul angle
            pods_[i].angle_=pods_[i].angle_+decalageAngle;

            //test pour savoir si on est sur checkpoint et si on incremente lapcount
            if (norm < 600.f  && pods_[i].IA_==true) {
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

        //si on est en commande manuelle, lorsqu'on passe sur les checkpoints, ils vont se colorer en vert
        //puis redevenir normaux lorsqu'on a finit un tour
        if (pods_[i].IA_==false) { 
            float dist;
            if (pods_[i].nextCP_>=0) {
                sf::Vector2f podCheckpoint= pods_[i].pos_ - otherCPs_[pods_[i].nextCP_].getPosition();
                dist=norme(podCheckpoint);
            } else {
                sf::Vector2f podCheckpoint= pods_[i].pos_ - finalCP_.getPosition();
                dist=norme(podCheckpoint);
            }

            if (dist < 600.f) {
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
        }

        //Si le pod est touché par un laser, un missile ou un asteroide on le ralenti pendant une durée
        //pendant cette durée, il ne peut pas être touché a nouveau
        //puis pendant une autre durée, il reste immunisé par les attaques
        //et une joue un son
        if (pods_[i].bouclier_==0) {
            if (isTouched(pods_[i]) && pods_[i].being_touched_==0) {
                missileHitAudio.play();
                pods_[i].timer_touched_+=1;
                pods_[i].being_touched_=1;
                pods_[i].vel_=sf::Vector2f(0.0001f,0.0001f);
            } else if (pods_[i].timer_touched_>=0 && pods_[i].timer_touched_<=50 && pods_[i].being_touched_==1) {
                pods_[i].timer_touched_+=1;
                pods_[i].vel_=sf::Vector2f(0.0001f,0.0001f);
            } else if (pods_[i].timer_touched_==51 && pods_[i].being_touched_==1) {
                pods_[i].timer_touched_=0;
                pods_[i].being_touched_=-1;
            } else if (pods_[i].timer_touched_>=0 && pods_[i].timer_touched_<=50 && pods_[i].being_touched_==-1) {
                pods_[i].timer_touched_+=1;
            } else if (pods_[i].timer_touched_==51 && pods_[i].being_touched_==-1) {
                pods_[i].timer_touched_=-1;
                pods_[i].being_touched_=0;
            }
        }

        //affichage du score
        if (i==0) {
            score += "JOUEUR : " +(std::to_string(pods_[i].getLap()))+"\n";
        } else {
            score += "IA " + (std::to_string(i)) + " : " + (std::to_string(pods_[i].getLap()))+"\n";
        }
    }

    //gestion des attaques lancées par le pod 0 (pod humain controlé par le joueur)
    //bullet bill : on change la texture du pod
    if (pods_[0].bullet_==1 && pods_[0].bullet_timer_==0) {
        podsSprites_[0].setTexture(tex_bullet_);
    } else if (pods_[0].bullet_timer_==101) {
        podsSprites_[0].setTexture(podsTextures_[0]);
    }

    //laser
    //on lance le laser a l'instant 0
    if (pods_[0].timer_attaque_==0 && pods_[0].attaque_==1) {
        attaque_laser(pods_[0]);
    //puis il se deplace de maniere rectiligne
    } else if (pods_[0].timer_attaque_>0 && pods_[0].timer_attaque_<=100 && pods_[0].attaque_==1 ) {
        laser_.pos_=laser_.pos_+laser_.vel_;
        laser_.shape_.setPosition(laser_.pos_);
    } else if (pods_[0].timer_attaque_==0 && pods_[0].attaque_==-1) {
        laser_.pos_=sf::Vector2f(-10000.f,-10000.f);
        laser_.shape_.setPosition(laser_.pos_);
    }

    //pose de l'asteroide par le joueur
    if (pods_[0].asteroide_pose_==1 && pods_[0].asteroide_timer_==0) {
        asteroide_.pos_=pods_[0].pos_-900.f*pods_[0].vel_/norme(pods_[0].vel_);
        asteroide_.sp_.setPosition(asteroide_.pos_);
        plopAudio.play();
    } else if (pods_[0].asteroide_pose_==-1 && pods_[0].asteroide_timer_==0) {
        asteroide_.pos_=sf::Vector2f(-10000.f,-10000.f);
        asteroide_.sp_.setPosition(asteroide_.pos_);
    }

    //bouclier
    if (pods_[0].bouclier_==1) {
        sp_bouclier_pod.setPosition(pods_[0].pos_);
    }

    //missile
    //pose du missile 
    if (pods_[0].missile_timer_==0 && pods_[0].missile_==1) {
        attaque_missile(pods_,nbPods_);
    //puis il se dirige vers sa cible avec la meme phyique qu'un pod sans la limitation pi/10
    } else if (pods_[0].missile_timer_>0 && pods_[0].missile_timer_<=50 && pods_[0].missile_==1) {
        
        sf::Vector2f missile_target = pods_[missile_.cible_].pos_;
        sf::Vector2f missile_pos = missile_.pos_;
        sf::Vector2f vecteur_vers_target_missile = missile_target-missile_pos;
        float decalageAngle_missile = angle(sf::Vector2f(0.00000001f,0.f),vecteur_vers_target_missile)-missile_.angle_;

        if (decalageAngle_missile>=180) {
            decalageAngle_missile-=360;
        }
        if (decalageAngle_missile<=-180) {
            decalageAngle_missile+=360;
        }
        float norm_missile = norme(vecteur_vers_target_missile);
        
        //calcul vitesse
        missile_.vel_=0.85f*(missile_.vel_+120.f*(vecteur_vers_target_missile/norm_missile));
        
        //calcul position
        missile_.pos_=missile_.pos_+missile_.vel_;

        //calcul angle
        missile_.angle_=missile_.angle_+decalageAngle_missile;

        missile_.sprite_.setPosition(missile_.pos_);
        missile_.sprite_.setRotation(missile_.angle_);
        if (norm_missile<300) {
            pods_[0].missile_timer_=51;
        }
    }
    if (pods_[0].missile_timer_>50 && pods_[0].missile_==1) {
        missile_.pos_=sf::Vector2f(-10000.f,-10000.f);
        missile_.sprite_.setPosition(missile_.pos_);
    }

    

    //affichage score                
    text.setString(score);

    //on incremente le temps
    physicsTime += PHYSICS_TIME_STEP;
}





//update de l'affichage sur un pas de temps plus court grace a l'interpolation
void Game::updateGraphics(sf::Time frameTime)
{
   
    if (frameTime==physicsTime) 
    {
        //pour se recaler toujours sur la physique
        int nbPod=pods_.size();
        for(int i=0;i<nbPod;i++)
        {
            podsSprites_[i].setPosition(pods_[i].pos_);
            podsSprites_[i].setRotation(pods_[i].angle_);
        }
    }

    else
    {
        // calcul des positions intermediaires des sprites
        int nbPod=pods_.size();
        for(int i=0;i<nbPod;i++)
        {  
            podsSprites_[i].move(((physicsTime-frameTime)/PHYSICS_TIME_STEP)*(pods_[i].pos_-podsSprites_[i].getPosition())); //bouge sprite avec fraction temporelle
        }
    }
}    



//
void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //affichage du background
    target.draw(backgroundSprite_, states);

    //affichage des checkpoints
    target.draw(finalCP_, states);

    for (const auto &cp : otherCPs_)
    {
        target.draw(cp, states);
    }

    //affichage des Pods
    for (const auto &podSprite : podsSprites_)
    {
        target.draw(podSprite, states);
    }

    //affichage du score
    target.draw(text);

    //affichage de l'etat des bonus
    if (pods_[0].champignon_>=0 && pods_[0].champignon_<=100 ) {
        target.draw(sp_champi);        
    }

    if (pods_[0].etoile_==1) {
        target.draw(sp_etoile);
    }

    if (pods_[0].bouclier_==1 && pods_[0].bullet_!=1 && pods_[0].etoile_!=1) {
        target.draw(sp_bouclier);
        //affichage du bouclier sur le pod
        target.draw(sp_bouclier_pod);
    }

    if (pods_[0].bouclier_==-1) {
        target.draw(sp_bouclier_used);
    }
    

    //affichage des sprites des attaques
    if (pods_[0].attaque_==1) {
        target.draw(laser_.shape_);
    }

    if (pods_[0].asteroide_pose_==1) {
        target.draw(asteroide_.sp_);
    }

    if (pods_[0].tempete_==1) {
        
        target.draw(sp_tempete);
    }

    if (pods_[0].missile_==1) {
        target.draw(missile_.sprite_);
    }


    //affichage du mode autopilot ou non
    if (pods_[0].autopilot_ && pods_[0].bullet_!=1) {
        target.draw(text_IA);
    }
    
}



//fonction qui initialise le laser a l'instant t=0 du tir
void Game::attaque_laser(Pod pod) {
    // lance un laser et gère le sprite associé
    laser_.vel_=300.f*pod.vel_/norme(pod.vel_);
    laser_.pos_= pod.pos_ + 3.f*laser_.vel_;
    laser_.angle_=pod.angle_;
    laser_.shape_.setRotation(laser_.angle_);
    laser_.shape_.setPosition(laser_.pos_);
}


//fonction qui initialise le missile a l'instant t=0 du tir
void Game::attaque_missile(std::vector<Pod> pods_,int nbPods_) {
    //recherche de la cible la plus proche
    float dist=norme(pods_[0].pos_-pods_[1].pos_);
    missile_.cible_=1;
    for (int i=2;i<nbPods_;++i) {
        float dist1=norme(pods_[0].pos_-pods_[1].pos_);
        if (dist1<dist) {
            dist=dist1;
            missile_.cible_=i;
        }
    }
    //initialisation
    missile_.vel_=300.f*pods_[0].vel_/norme(pods_[0].vel_);
    missile_.pos_= pods_[0].pos_ + 3.f*missile_.vel_;
    missile_.angle_=pods_[0].angle_;
    missile_.sprite_.setRotation(missile_.angle_);
    missile_.sprite_.setPosition(missile_.pos_);
}


//fonction qui determine si un Pod est touche par un missile, un laser ou un asteroide
bool Game::isTouched(Pod pod) {
    //regarde si un pod est touche par un bonus
    sf::Vector2f vect_asteroid=asteroide_.pos_-pod.pos_;
    sf::Vector2f vect_laser = laser_.pos_-pod.pos_;
    sf::Vector2f vect_missile = missile_.pos_-pod.pos_;
    float dist_laser=norme(vect_laser);
    float dist_asteroid=norme(vect_asteroid);
    float dist_missile=norme(vect_missile);
    if (dist_laser<300 || dist_asteroid<600 || dist_missile<300) {
        return true;
    }
    return false;
}

