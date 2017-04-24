#pragma once

#include "MM_GameObject.h"
#include <vector>
#include <memory>

#include "MM_Typedefs.h"

 class GameObjectManager
 {
   public:
     std::vector<std::unique_ptr<GameObject> > gameObjects;

     void update()
     {
       for (auto& gameobject : gameObjects)
         gameobject->update();
     }
     void draw()
     {
       for (auto& gameobject: gameObjects)
         gameobject->draw();
     }
 };