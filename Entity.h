#pragma once

#include <gfx.h>

#include "GameObject.h"

enum class Direction { North, East, South, West };

class Entity : public GameObject {
protected:
    const gfx::Window& m_window;
    gfx::Vec m_position;
    const gfx::Texture m_texture;
    static constexpr int m_max_health = 100;
    static constexpr float m_movement_speed = 500;
    int m_health = m_max_health;

    static constexpr double m_animation_delay = 0.1;
    static constexpr float m_texture_scale = 5;
    gfx::Spritesheet m_spritesheet { m_texture, 32, 32 };
    gfx::AnimatedTexture m_sprite_attacking_south     { m_window, m_spritesheet.get_row(6, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_attacking_north     { m_window, m_spritesheet.get_row(8, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_attacking_sidewards { m_window, m_spritesheet.get_row(7, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_south          { m_window, m_spritesheet.get_row(0, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_sidewards      { m_window, m_spritesheet.get_row(1, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_north          { m_window, m_spritesheet.get_row(2, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_sidewards   { m_window, m_spritesheet.get_row(4, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_north       { m_window, m_spritesheet.get_row(5, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_south       { m_window, m_spritesheet.get_row(3, 6), m_animation_delay };

    Direction m_direction = Direction::South;

    enum class State { Idle, Walking, Attacking };
    State m_state = State::Idle;

public:
    Entity(const gfx::Window& window, gfx::Vec position, gfx::Texture texture);

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
    }

    [[nodiscard]] int get_health() const {
        return m_health;
    }

    [[nodiscard]] float get_movement_speed() const {
        return m_movement_speed;
    }

    void set_position(gfx::Vec position) {
        m_position = position;
    }

    [[nodiscard]] gfx::Rect get_hitbox() const;

    void update(double dt) override;
    void draw(gfx::Renderer& rd) const override;
    void walk(Direction dir, double dt);
    void attack();

};
