#pragma once

#include "Entity.h"

class Player : public Entity {
public:
    explicit Player(gfx::Vec position)
    : Entity(position, gfx::Texture("./assets/Cute_Fantasy_Free/Player/Player.png"))
    {

        m_spritesheet.set_sprite(SpriteState::IdleSouth, {
            { 8, 4, m_sprite_width, m_sprite_height },
            { 40, 4, m_sprite_width, m_sprite_height },
            { 72, 4, m_sprite_width, m_sprite_height },
            { 104, 4, m_sprite_width, m_sprite_height },
            { 136, 4, m_sprite_width, m_sprite_height },
            { 168, 4, m_sprite_width, m_sprite_height },
        });

        m_spritesheet.set_sprite(SpriteState::IdleSidewards, {
            { 9, 36, m_sprite_width, m_sprite_height },
            { 41, 36, m_sprite_width, m_sprite_height },
            { 73, 36, m_sprite_width, m_sprite_height },
            { 105, 36, m_sprite_width, m_sprite_height },
            { 137, 36, m_sprite_width, m_sprite_height },
            { 169, 36, m_sprite_width, m_sprite_height },
        });

        m_spritesheet.set_sprite(SpriteState::IdleNorth, {
            { 8, 68, m_sprite_width, m_sprite_height },
            { 40, 68, m_sprite_width, m_sprite_height },
            { 72, 68, m_sprite_width, m_sprite_height },
            { 104, 68, m_sprite_width, m_sprite_height },
            { 136, 68, m_sprite_width, m_sprite_height },
            { 168, 68, m_sprite_width, m_sprite_height },
        });

        m_spritesheet.set_sprite(SpriteState::WalkingSidewards, {
            { 9, 130, m_sprite_width, m_sprite_height },
            { 41, 130, m_sprite_width, m_sprite_height },
            { 73, 130, m_sprite_width, m_sprite_height },
            { 105, 130, m_sprite_width, m_sprite_height },
            { 137, 130, m_sprite_width, m_sprite_height },
            { 169, 130, m_sprite_width, m_sprite_height },
        });

        m_spritesheet.set_sprite(SpriteState::WalkingNorth, {
            { 8, 162, m_sprite_width, m_sprite_height },
            { 40, 162, m_sprite_width, m_sprite_height },
            { 72, 162, m_sprite_width, m_sprite_height },
            { 104, 162, m_sprite_width, m_sprite_height },
            { 136, 162, m_sprite_width, m_sprite_height },
            { 168, 162, m_sprite_width, m_sprite_height },
        });

        m_spritesheet.set_sprite(SpriteState::WalkingSouth, {
            { 8, 98, m_sprite_width, m_sprite_height },
            { 40, 98, m_sprite_width, m_sprite_height },
            { 72, 98, m_sprite_width, m_sprite_height },
            { 104, 98, m_sprite_width, m_sprite_height },
            { 136, 98, m_sprite_width, m_sprite_height },
            { 168, 98, m_sprite_width, m_sprite_height },
        });
    }
};
