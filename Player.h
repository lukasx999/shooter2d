#pragma once

#include "Entity.h"

class Player : public Entity {
public:
    Player(const gfx::Window& window, gfx::Vec position)
    : Entity(window, position, gfx::Texture("./assets/Cute_Fantasy_Free/Player/Player.png"))
    {

    }

};
