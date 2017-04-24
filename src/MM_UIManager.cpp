#include "MM_UIManager.h"

float UI::aspectRatio{1.0f};

void UIManager::update(const Camera &camera)
{
  UI::aspectRatio = camera.aspectRatio;
  for (auto& ui : uiObjects)
    ui->update();
}

void UIManager::draw()
{
  for (auto &ui : uiObjects)
    ui->draw();
}
