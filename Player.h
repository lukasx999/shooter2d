#pragma once

#include <gfx.h>

enum class Direction { North, East, South, West };

class Player {
    gfx::Vec m_position;
    static constexpr float m_movement_speed = 500.0;
    const gfx::Texture m_texture;

    enum class SpriteState {
        Idle, Walking,
    };

    Spritesheet<SpriteState> m_spritesheet{ m_texture };

    static constexpr float m_sprite_width = 15;
    static constexpr float m_sprite_height = 22;
    static constexpr float m_texture_scale = 5.0f;
    Direction m_direction = Direction::North;

public:
    explicit Player(gfx::Vec position)
        : m_position(position)
        , m_texture("./assets/Cute_Fantasy_Free/Player/Player.png")
    {
        m_spritesheet.set_sprite(SpriteState::Idle, { 8, 4, m_sprite_width, m_sprite_height });
        m_spritesheet.set_sprite(SpriteState::Walking, { 8, 35, m_sprite_width, m_sprite_height });
    }

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
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

    void draw(gfx::Renderer& rd) const {
        m_spritesheet.draw(rd, SpriteState::Idle, get_hitbox());
        rd.draw_rectangle(get_hitbox(), gfx::Color::red().set_alpha(0x7f));
    }

    void move(Direction dir, double dt) {

        switch (dir) {
            using enum Direction;

            case North:
                m_position.y -= m_movement_speed * dt;
                m_direction = North;
                break;

            case East:
                m_position.x += m_movement_speed * dt;
                m_direction = East;
                break;

            case South:
                m_position.y += m_movement_speed * dt;
                m_direction = South;
                break;

            case West:
                m_position.x -= m_movement_speed * dt;
                m_direction = West;
                break;
        }
    }

};
