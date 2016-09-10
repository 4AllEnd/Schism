#include <Wretched/Core/GameObject.h>
#include <Wretched/Core/Placement.h>
#include <Wretched/Graphics/Camera.h>
#include <SFML/Window/Keyboard.hpp>


#ifndef SCHISM_PLAYERSCRIPT_H
#define SCHISM_PLAYERSCRIPT_H

#endif //SCHISM_PLAYERSCRIPT_H
using namespace Wretched;
#include <Wretched/System/RuntimeObjectSystem/ObjectInterfacePerModule.h>
#include <Wretched/Core/GameObject.h>
#include "InterfaceIds.h"

struct PlayerScript : GameObject {
  Placement::Placement *playerplace;
  Camera *cam;
  PlayerScript(Entity *entity) : GameObject(entity) {}
  void init() {
    playerplace = entity->getcomponent<Placement::Placement>();
    cam = entity->getcomponent<Camera>();
  }
  void update(double deltatime) {
    if (playerplace != nullptr) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        playerplace->pitch((float) deltatime * W_degree * 100.f);
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
        playerplace->pitch((float) deltatime * W_degree * -100.f);

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        playerplace->roll((float) deltatime * W_degree * 200.f);
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        playerplace->roll((float) deltatime * W_degree * -200.f);

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        playerplace->yaw((float) deltatime * W_degree * 200.f);
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        playerplace->yaw((float) deltatime * W_degree * -200.f);
    }

    if (cam != nullptr) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
        cam->zoom -= (float) (deltatime * (cam->zoom + 1 / W_pi) / 4);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
        cam->zoom += (float) (deltatime * (121 / W_pi - cam->zoom) / 4);
      }
    }
};