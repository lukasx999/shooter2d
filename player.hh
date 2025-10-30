#pragma once

#include <gfx.hh>

enum class Direction { North, East, South, West };

class Player {
    gfx::Vec m_position;
    static constexpr float m_size = 25.0;
    static constexpr float m_movement_speed = 500.0;

public:
    explicit Player(gfx::Vec position)
    : m_position(position)
    { }

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
    }

    [[nodiscard]] gfx::Rect get_hitbox() const {
        return {
            m_position.x - m_size,
            m_position.y - m_size,
            m_size * 2,
            m_size * 2,
        };
    }

    [[nodiscard]] float get_size() const {
        return m_size;
    }

    [[nodiscard]] float get_movement_speed() const {
        return m_movement_speed;
    }

    void set_position(gfx::Vec position) {
        m_position = position;
    }

    void draw(gfx::Renderer& rd) const {
        rd.draw_circle(m_position, m_size, gfx::Color::blue());
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
