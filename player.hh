#pragma once

#include <gfx.hh>

enum class Direction { North, East, South, West };

class Player {
    gfx::Vec m_position;
    static constexpr float m_movement_speed = 500.0;
    const gfx::Texture m_texture;

    static constexpr gfx::Rect m_texture_src {
        8, 4, 15, 22
    };

    static constexpr float m_texture_scale = 5.0f;

public:
    explicit Player(gfx::Vec position)
        : m_position(position)
        , m_texture("./assets/Cute_Fantasy_Free/Player/Player.png")
    { }

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
    }

    // TODO: seperate the render hitbox from the collision hitbox
    [[nodiscard]] gfx::Rect get_hitbox() const {
        float width = m_texture_src.width * m_texture_scale;
        float height = m_texture_src.height * m_texture_scale;

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

    void draw(gfx::Renderer& rd) const {
        rd.draw_texture_sub(get_hitbox(), m_texture_src, m_texture);
    }

    void move(Direction dir, double dt) {

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
