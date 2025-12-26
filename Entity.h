#pragma once

#include <gfx.h>

#include "Sprite.h"
#include "GameObject.h"

enum class Direction { North, East, South, West };

class Entity : public GameObject {
protected:
    const gfx::Window& m_window;
    gfx::Vec m_position;
    const gfx::Texture m_texture;
    static constexpr int m_max_health = 100;
    static constexpr double m_animation_delay = 0.1;
    static constexpr float m_sprite_width = 15;
    static constexpr float m_sprite_height = 22;
    static constexpr float m_movement_speed = 500;
    static constexpr float m_texture_scale = 5;
    int m_health = m_max_health;
    Direction m_direction = Direction::South;
    bool m_is_idle = true;
    bool m_is_attacking = false;

    Sprite m_sprite_attack_south {
        m_texture, m_animation_delay,
        {
            { 5,   197, m_sprite_width, m_sprite_height },
            { 38,  197, m_sprite_width, m_sprite_height },
            { 72,  197, m_sprite_width, m_sprite_height },
            { 104, 197, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_idle_south {
        m_texture, m_animation_delay,
        {
            { 8,   4, m_sprite_width, m_sprite_height },
            { 40,  4, m_sprite_width, m_sprite_height },
            { 72,  4, m_sprite_width, m_sprite_height },
            { 104, 4, m_sprite_width, m_sprite_height },
            { 136, 4, m_sprite_width, m_sprite_height },
            { 168, 4, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_idle_sidewards {
        m_texture, m_animation_delay,
        {
            { 9,   36, m_sprite_width, m_sprite_height },
            { 41,  36, m_sprite_width, m_sprite_height },
            { 73,  36, m_sprite_width, m_sprite_height },
            { 105, 36, m_sprite_width, m_sprite_height },
            { 137, 36, m_sprite_width, m_sprite_height },
            { 169, 36, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_idle_north {
        m_texture, m_animation_delay,
        {
            { 8,   68, m_sprite_width, m_sprite_height },
            { 40,  68, m_sprite_width, m_sprite_height },
            { 72,  68, m_sprite_width, m_sprite_height },
            { 104, 68, m_sprite_width, m_sprite_height },
            { 136, 68, m_sprite_width, m_sprite_height },
            { 168, 68, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_walking_sidewards {
        m_texture, m_animation_delay,
        {
            { 9,   130, m_sprite_width, m_sprite_height },
            { 41,  130, m_sprite_width, m_sprite_height },
            { 73,  130, m_sprite_width, m_sprite_height },
            { 105, 130, m_sprite_width, m_sprite_height },
            { 137, 130, m_sprite_width, m_sprite_height },
            { 169, 130, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_walking_north {
        m_texture, m_animation_delay,
        {
            { 8,   162, m_sprite_width, m_sprite_height },
            { 40,  162, m_sprite_width, m_sprite_height },
            { 72,  162, m_sprite_width, m_sprite_height },
            { 104, 162, m_sprite_width, m_sprite_height },
            { 136, 162, m_sprite_width, m_sprite_height },
            { 168, 162, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_walking_south {
        m_texture, m_animation_delay,
        {
            { 8,   98, m_sprite_width, m_sprite_height },
            { 40,  98, m_sprite_width, m_sprite_height },
            { 72,  98, m_sprite_width, m_sprite_height },
            { 104, 98, m_sprite_width, m_sprite_height },
            { 136, 98, m_sprite_width, m_sprite_height },
            { 168, 98, m_sprite_width, m_sprite_height },
        }
    };

private:
    // const-agnostic code.
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
    Entity(const gfx::Window& window, gfx::Vec position, gfx::Texture texture)
        : m_window(window)
        , m_position(position)
        , m_texture(std::move(texture))
    { }

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
