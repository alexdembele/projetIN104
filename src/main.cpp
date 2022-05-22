#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>
#include "game.h"
#include <vector>
#include <string>

#include "utils.h"

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 32;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!", sf::Style::Default, settings);
    window.setView(sf::View(sf::Rect(0.f,0.f,16000.f,9000.f)));
    window.setVerticalSyncEnabled(false);

    //frame per second
    float fps = 60;
    //temps minimal entre deux frame
    sf::Time refreshTime = sf::microseconds(1e6/fps);
    window.setFramerateLimit(120);

    //horloge depuis le début du jeu
    sf::Clock globalClock;
    //horloge réinitialisée à chaque frame
    sf::Clock refreshClock;

    //temps de la physique du jeu
    sf::Time physicsTime = sf::Time::Zero;

    //Checkpoints
    unsigned int nbCP = 5;
    /*
    std::vector<sf::Vector2f> checkpointsPositions;
    sf::Vector2f vfinal(5000.f, 5000.f);
    checkpointsPositions.emplace_back(vfinal);
    for (unsigned int i=1; i<nbCP; i++) {
        sf::Vector2f v(1000.f*i, 1000.f*i);
        checkpointsPositions.emplace_back(v);
    }
    */
    std::vector<sf::Vector2f> checkpointsPositions = randomCP( nbCP);

    //Vous pouvez aussi initialiser myGame avec une liste de checkpoints prédéfinie
    Game myGame(checkpointsPositions);
    myGame.addPod();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        //si on a dépassé le refreshTime depuis la dernière frame
        if (refreshClock.getElapsedTime() >= refreshTime)
        {
            //réinitialise l'horloge qui mesure le temps entre les frames
            refreshClock.restart();

            //récupère le temps écoulé depuis le début du jeu
            sf::Time frameTime = globalClock.getElapsedTime();

            //si on a dépassé le temps de la physique
            if (frameTime > physicsTime)
            {
                //met à jour les sprite au temps de la physique
                myGame.updateGraphics(physicsTime);
                //fait avancer la physique d'un pas de temps
                myGame.updatePhysics();
                physicsTime += PHYSICS_TIME_STEP;

                //reprend le temps écoulé depuis le début pour rester précis
                frameTime = globalClock.getElapsedTime();
            }
            
            //met à jour les sprites au temps actuel
            myGame.updateGraphics(frameTime);
            window.draw(myGame);
            window.display();
        }
    }

    return 0;
}