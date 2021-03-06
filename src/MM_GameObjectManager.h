#pragma once

#include "MM_GameObject.h"
#include <vector>
#include <memory>

#include "MM_Typedefs.h"

 class GameObjectManager
 {
   public:
     vector<unique_ptr<GameObject> > gameObjects;

     void update()
     {
        for (auto& gameobject : gameObjects)
          gameobject->update();
     }
     void draw(const Camera &camera)
     {
        const mat4 viewProjection = camera.getViewProjection();
        for (auto& gameobject: gameObjects)
          gameobject->draw(viewProjection);
     }
 };
