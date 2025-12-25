#pragma once

#include <gfx.h>
#include <print>

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

    Sprite m_sprite_idle_south {
        m_texture, m_animation_delay,
        {
            { 8, 4, m_sprite_width, m_sprite_height },
            { 40, 4, m_sprite_width, m_sprite_height },
            { 72, 4, m_sprite_width, m_sprite_height },
            { 104, 4, m_sprite_width, m_sprite_height },
            { 136, 4, m_sprite_width, m_sprite_height },
            { 168, 4, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_idle_sidewards {
        m_texture, m_animation_delay,
        {
            { 9, 36, m_sprite_width, m_sprite_height },
            { 41, 36, m_sprite_width, m_sprite_height },
            { 73, 36, m_sprite_width, m_sprite_height },
            { 105, 36, m_sprite_width, m_sprite_height },
            { 137, 36, m_sprite_width, m_sprite_height },
            { 169, 36, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_idle_north {
        m_texture, m_animation_delay,
        {
            { 8, 68, m_sprite_width, m_sprite_height },
            { 40, 68, m_sprite_width, m_sprite_height },
            { 72, 68, m_sprite_width, m_sprite_height },
            { 104, 68, m_sprite_width, m_sprite_height },
            { 136, 68, m_sprite_width, m_sprite_height },
            { 168, 68, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_walking_sidewards {
        m_texture, m_animation_delay,
        {
            { 9, 130, m_sprite_width, m_sprite_height },
            { 41, 130, m_sprite_width, m_sprite_height },
            { 73, 130, m_sprite_width, m_sprite_height },
            { 105, 130, m_sprite_width, m_sprite_height },
            { 137, 130, m_sprite_width, m_sprite_height },
            { 169, 130, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_walking_north {
        m_texture, m_animation_delay,
        {
            { 8, 162, m_sprite_width, m_sprite_height },
            { 40, 162, m_sprite_width, m_sprite_height },
            { 72, 162, m_sprite_width, m_sprite_height },
            { 104, 162, m_sprite_width, m_sprite_height },
            { 136, 162, m_sprite_width, m_sprite_height },
            { 168, 162, m_sprite_width, m_sprite_height },
        }
    };

    Sprite m_sprite_walking_south {
        m_texture, m_animation_delay,
        {
            { 8, 98, m_sprite_width, m_sprite_height },
            { 40, 98, m_sprite_width, m_sprite_height },
            { 72, 98, m_sprite_width, m_sprite_height },
            { 104, 98, m_sprite_width, m_sprite_height },
            { 136, 98, m_sprite_width, m_sprite_height },
            { 168, 98, m_sprite_width, m_sprite_height },
        }
    };


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

    [[nodiscard]] gfx::Rect get_hitbox() const {
        float width = m_sprite_width * m_texture_scale;
        float height = m_sprite_height * m_texture_scale;

        return {
            m_position.x - width / 2.0f,
            m_position.y - height / 2.0f,
            width,
            height,
        };
    }

    [[nodiscard]] float get_movement_speed() const {
        return m_movement_speed;
    }

    void set_position(gfx::Vec position) {
        m_position = position;
    }

    void update([[maybe_unused]] double dt) override {

        auto& current_sprite = [&] -> Sprite& {
            switch (m_direction) {
                using enum Direction;
                case North: return m_is_idle ? m_sprite_idle_north     : m_sprite_walking_north;
                case East:  return m_is_idle ? m_sprite_idle_sidewards : m_sprite_walking_sidewards;
                case West:  return m_is_idle ? m_sprite_idle_sidewards : m_sprite_walking_sidewards;
                case South: return m_is_idle ? m_sprite_idle_south     : m_sprite_walking_south;
            }
        }();

        current_sprite.update(m_window);

        m_is_idle = true;
    }

    void draw(gfx::Renderer& rd) const override {

        const auto& current_sprite = [&] -> const Sprite& {
            switch (m_direction) {
                using enum Direction;
                case North: return m_is_idle ? m_sprite_idle_north     : m_sprite_walking_north;
                case East:  return m_is_idle ? m_sprite_idle_sidewards : m_sprite_walking_sidewards;
                case West:  return m_is_idle ? m_sprite_idle_sidewards : m_sprite_walking_sidewards;
                case South: return m_is_idle ? m_sprite_idle_south     : m_sprite_walking_south;
            }
        }();

        if (m_direction == Direction::West)
            current_sprite.draw_mirrored(rd, get_hitbox());
        else
            current_sprite.draw(rd, get_hitbox());

        // rd.draw_rectangle(get_hitbox(), gfx::Color::red().set_alpha(0x7f));
    }

    void move(Direction dir, double dt) {
        m_direction = dir;
        m_is_idle = false;

        switch (dir) {
            using enum Direction;

            case North:
                m_position.y -= m_movement_speed * dt;
                break;

            case East:
                m_position.x += m_movement_speed * dt;
                break;

            case South:
                m_position.y += m_movement_speed * dt;
                break;

            case West:
                m_position.x -= m_movement_speed * dt;
                break;
        }
    }

};
