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
    static constexpr double m_animation_delay = 0.1;
    static constexpr float m_movement_speed = 500;
    static constexpr float m_texture_scale = 5;
    int m_health = m_max_health;
    Direction m_direction = Direction::South;
    bool m_is_idle = true;
    bool m_is_attacking = false;
    // TODO: more generic interface for this
    gfx::Spritesheet m_spritesheet { m_texture, 32, 32 };
    gfx::AnimatedTexture m_sprite_attack_south      { m_window, m_spritesheet.get_row(6, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_attack_north      { m_window, m_spritesheet.get_row(8, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_attack_sidewards  { m_window, m_spritesheet.get_row(7, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_south        { m_window, m_spritesheet.get_row(0, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_sidewards    { m_window, m_spritesheet.get_row(1, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_north        { m_window, m_spritesheet.get_row(2, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_sidewards { m_window, m_spritesheet.get_row(4, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_north     { m_window, m_spritesheet.get_row(5, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_south     { m_window, m_spritesheet.get_row(3, 6), m_animation_delay };

private:
    // const-agnostic code
    template <typename This> requires std::is_pointer_v<This>
    [[nodiscard]] static auto& get_current_sprite(This self) {

        bool is_idle = self->m_is_idle;

        switch (self->m_direction) {
            using enum Direction;
            case North: return is_idle ? self->m_sprite_idle_north     : self->m_sprite_walking_north;
            case East:  return is_idle ? self->m_sprite_idle_sidewards : self->m_sprite_walking_sidewards;
            case West:  return is_idle ? self->m_sprite_idle_sidewards : self->m_sprite_walking_sidewards;
            case South: return is_idle ? self->m_sprite_idle_south     : self->m_sprite_walking_south;
        }
    }

public:
    Entity(const gfx::Window& window, gfx::Vec position, gfx::Texture texture);

    [[nodiscard]] bool is_idle() const {
        return m_is_idle;
    }

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
    }

    [[nodiscard]] int get_health() const {
        return m_health;
    }

    [[nodiscard]] gfx::Rect get_hitbox() const;

    [[nodiscard]] float get_movement_speed() const {
        return m_movement_speed;
    }

    void set_position(gfx::Vec position) {
        m_position = position;
    }

    void update(double dt) override;

    void draw(gfx::Renderer& rd) const override;

    void attack() {
        m_is_attacking = true;
    }

    void move(Direction dir, double dt);

};
