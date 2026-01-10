#pragma once

#include <gfx.h>

#include "Player.h"
#include "SpriteEntity.h"

class Enemy : public SpriteEntity {
    static constexpr double m_animation_delay = 0.1;
    static constexpr float m_texture_scale = 5;
    static constexpr int m_spritesheet_cell_size = 32;
    const gfx::Texture m_texture;
    gfx::Spritesheet m_spritesheet { m_texture, m_spritesheet_cell_size, m_spritesheet_cell_size };
    gfx::AnimatedTexture m_sprite_attacking_south     { m_window, m_spritesheet.get_row(6, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_attacking_north     { m_window, m_spritesheet.get_row(8, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_attacking_sidewards { m_window, m_spritesheet.get_row(7, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_south          { m_window, m_spritesheet.get_row(0, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_sidewards      { m_window, m_spritesheet.get_row(1, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_north          { m_window, m_spritesheet.get_row(2, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_sidewards   { m_window, m_spritesheet.get_row(4, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_north       { m_window, m_spritesheet.get_row(5, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_south       { m_window, m_spritesheet.get_row(3, 6), m_animation_delay };

    const Player& m_target;

public:
    Enemy(const gfx::Window& window, gfx::Vec position, const Player& target)
        : SpriteEntity(window, position, m_spritesheet_cell_size * m_texture_scale, m_spritesheet_cell_size * m_texture_scale)
        , m_texture(gfx::Texture("./assets/Cute_Fantasy_Free/Enemies/Skeleton.png"))
        , m_target(target)
    { }

    [[nodiscard]] const gfx::AnimatedTexture& get_current_sprite(Direction direction, State state) const override {

        switch (state) {
            using enum State;
            using enum Direction;

            case Idle: switch (direction) {
                case North: return m_sprite_idle_north;
                case SouthEast:
                case SouthWest:
                case NorthEast:
                case NorthWest:
                case East:
                case West:  return m_sprite_idle_sidewards;
                case South: return m_sprite_idle_south;
            }

            case Walking: switch (direction) {
                case North: return m_sprite_walking_north;
                case SouthEast:
                case SouthWest:
                case NorthEast:
                case NorthWest:
                case East:
                case West:  return m_sprite_walking_sidewards;
                case South: return m_sprite_walking_south;
            }

            case Attacking: switch (direction) {
                case North: return m_sprite_attacking_north;
                case SouthEast:
                case SouthWest:
                case NorthEast:
                case NorthWest:
                case East:
                case West:  return m_sprite_attacking_sidewards;
                case South: return m_sprite_attacking_south;
            }

        }

    }

    void update(double dt) override {
        SpriteEntity::update(dt);
        chase_target(dt);

    }

private:
    void chase_target(double dt) {

        auto diff = m_target.get_position() - get_position();
        gfx::Vec base(1.0, 0.0);
        auto angle = gfx::rad_to_deg(diff.angle(base));

        if (m_target.get_position().y > get_position().y)
            angle = 180 + (180 - angle);

        if (diff.length() >= 100)
            walk(angle_to_direction(angle), dt);

    }

    [[nodiscard]] constexpr Direction angle_to_direction(int angle) const {

        if (angle >= 45 && angle <= 135) return Direction::North;
        if (angle >= 135 && angle <= 225) return Direction::West;
        if (angle >= 225 && angle <= 315) return Direction::South;
        if (angle >= 315 || angle <= 45) return Direction::East;

        throw std::runtime_error("invalid angle");
    }

};
