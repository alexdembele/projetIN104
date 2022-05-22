#include "../inc/game.h"
#include "../inc/checkpoint.h"
#include "../inc/utils.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <iostream>

Game::Game(std::vector<sf::Vector2f> checkpointsPositions) : finalCP_(checkpointsPositions[0])
{   
    //la premier ligne construit le final checkpoint a partir de la position
    //la boucle suivante construit les autres checkpoints a partir de la position
    unsigned int size=checkpointsPositions.size();
    otherCPs_.reserve(size-1);
    for (unsigned int cpID=1; cpID < size; cpID++) {
        otherCPs_.emplace_back(checkpointsPositions[cpID],cpID);
    }

    //initialisation des time
    lastFrameTime=sf::seconds(0.f);
    physicsTime=sf::seconds(0.f);

    //initialisation du background
    backgroundTexture_.loadFromFile("../repository/Images/background.png");
    backgroundSprite_.setTexture(backgroundTexture_);
    scaleToMinSize(backgroundSprite_,16000,9000);
    
}

void Game::addPod()
{   
    //on reserve l'emplacement pour les pods, textures et sprite
    pods_.reserve(1);
    podsTextures_.reserve(1);
    podsTextures_.emplace_back();
    podsSprites_.reserve(1);
    podsSprites_.emplace_back();
    
    //on cree le pod
    sf::Vector2f pos(8000.f, 8000.f);
    sf::Vector2f vel(100.f, 100.f);
    pods_.emplace_back(pos,0.f,vel);

    //on lui donne une texture + on l'applique au sprite + on le scale + on setorigin
    podsTextures_[0].loadFromFile("../repository/Images/SWMilleniumFalcon.png");
    podsSprites_[0].setTexture(podsTextures_[0]);
    setOriginToCenter(podsSprites_[0]);
    podsSprites_[0].setPosition(8000.f, 4500.f);
    scaleToMinSize( podsSprites_[0],800,800);
}

void Game::updatePhysics()
{  
    int nbPod=pods_.size();
   for(int i=0;i<nbPod;i++)
   {
       //test angle limite
       sf::Vector2f target;
       float decalageAngle =-180/M_PI*angle(pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_-pods_[i].pos_,sf::Vector2f (0.001,0))-pods_[i].angle_;
       printf("decalage:%f\n",decalageAngle);
       if(abs(decalageAngle>18))
       {
           float Nx= cos(decalageAngle/180*M_PI)*(pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_-pods_[i].pos_).x +sin(decalageAngle/180*M_PI)*(pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_-pods_[i].pos_).y ; 
           float Ny=-sin(decalageAngle/180*M_PI)*(pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_-pods_[i].pos_).x +cos(decalageAngle/180*M_PI)*(pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_-pods_[i].pos_).y;
           target=pods_[i].pos_ +sf::Vector2f (Nx,Ny);
          
          
           sf::Vector2f nor =target-pods_[i].pos_;
           float norme= sqrt(nor.x*nor.x+nor.y*nor.y);
           //vitesse  .
           pods_[i].vel_=0.85f*(pods_[i].vel_+pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).power_*(target-pods_[i].pos_)/norme);
           pods_[i].pos_=pods_[i].pos_+pods_[i].vel_;
           pods_[i].angle_=-180/M_PI*angle(target-pods_[i].pos_,sf::Vector2f (0.001,0));
       }
     
       else
       {
          
           //
           sf::Vector2f nor =pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_-pods_[i].pos_;
           float norme= sqrt(nor.x*nor.x+nor.y*nor.y);
           //vitesse  .
           pods_[i].vel_=0.85f*(pods_[i].vel_+pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).power_*(pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_-pods_[i].pos_)/norme);
           //position
           pods_[i].pos_=pods_[i].pos_+pods_[i].vel_;
 
           //angle...angle max dans get decision
           pods_[i].angle_=-180/M_PI*angle(pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_-pods_[i].pos_,sf::Vector2f (0.001,0));
           /*if (pods_[i].getDecision(pods_[i]).target_.x-pods_[i].pos_.x >0)
           {
               pods_[i].angle_= (180/M_PI*atan((pods_[i].getDecision(pods_[i]).target_.y-pods_[i].pos_.y)/(pods_[i].getDecision(pods_[i]).target_.x-pods_[i].pos_.x)));
           }
           else
           {
             pods_[i].angle_= (180-180/M_PI*atan((pods_[i].getDecision(pods_[i]).target_.y-pods_[i].pos_.y)/(pods_[i].getDecision(pods_[i]).target_.x-pods_[i].pos_.x)));
            }*/
          
       }
       printf("physics:%f\n",pods_[i].angle_);
 
      
   }
   physicsTime += PHYSICS_TIME_STEP;

   /*  
   int nbPod=pods_.size();
   for(int i=0;i<nbPod;i++)
   {

       
 
       //on calcule les nouveaux vecteurs position et vitesse
       //pour cela on calcule le vecteur entre le pod et sa target
       sf::Vector2f nor =pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_-pods_[i].pos_;
       float norme= sqrt(nor.x*nor.x+nor.y*nor.y);

       //vitesse
       pods_[i].vel_=0.85f*(pods_[i].vel_+pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).power_*(pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_-pods_[i].pos_)/norme);
       //position
       pods_[i].pos_=pods_[i].pos_+pods_[i].vel_;
 
       //angle...angle max dans get decision
    
       if (pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_.x-pods_[i].pos_.x >0)
       {
           pods_[i].angle_= (180/M_PI*atan((pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_.y-pods_[i].pos_.y)/(pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_.x-pods_[i].pos_.x)));
       }
       else
       {
          pods_[i].angle_= (180-180/M_PI*atan((pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_.y-pods_[i].pos_.y)/(pods_[i].getDecision(pods_[i],otherCPs_,finalCP_).target_.x-pods_[i].pos_.x)));
       }
       //printf("%f\n",pods_[i].angle_);
       printf("%d\n",pods_[i].lapCount_);
       if (norme < 300) {
           if (pods_[i].nextCP_<3) {
               pods_[i].nextCP_=pods_[i].nextCP_+1;
           } else if (pods_[i].nextCP_==3) {
               pods_[i].nextCP_=-1;
           }
           if (pods_[i].nextCP_==0) {
           pods_[i].lapCount_=pods_[i].lapCount_+1;
       }
       }

       
       

 
      
 
      
   }
   physicsTime += PHYSICS_TIME_STEP;

   */
    /*
    int nbPod=pods_.size();
    for(int i=0;i<nbPod;i++)
    {
        sf::Vector2f nor =pods_[i].getDecision(pods_[i]).target_-pods_[i].pos_;
        float norme= sqrt(nor.x*nor.x+nor.y*nor.y);
        //vitesse
        pods_[i].vel_=0.85f*(pods_[i].vel_+pods_[i].getDecision(pods_[i]).power_*(pods_[i].getDecision(pods_[i]).target_-pods_[i].pos_)/norme);
        //position
        pods_[i].pos_=pods_[i].pos_+pods_[i].vel_;


        float ax = pods_[i].getDecision(pods_[i]).target_.x;
        float ay = pods_[i].getDecision(pods_[i]).target_.y;
        float norma = sqrt(ax*ax + ay*ay);

        float tx = cos((2*M_PI*pods_[i].angle_)/360);
        float ty = sin((2*M_PI*pods_[i].angle_)/360);
        float normt = sqrt(tx*tx + ty*ty);

        float ps = ax*tx + ay*ty;
        
        float angle = acos(ps/(norma*normt));
        
        if 
        if () 


        //angle......angle max dans get decision
        pods_[i].angle_=180/M_PI*atan((pods_[i].getDecision(pods_[i]).target_.y-pods_[i].pos_.y)/(pods_[i].getDecision(pods_[i]).target_.x-pods_[i].pos_.x));


        

        
    }
    physicsTime += PHYSICS_TIME_STEP;
    */
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
            //podsSprites_[i].rotate(((physicsTime-frameTime)/PHYSICS_TIME_STEP)*(pods_[i].angle_-podsSprites_[i].getRotation())); //tourne sprite avec fraction temporelle
            //probleme modulo
            
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
}