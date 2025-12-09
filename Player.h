#pragma once

#include <gfx.h>
#include <print>

enum class Direction { North, East, South, West };

class Player {
    gfx::Vec m_position;
    static constexpr float m_movement_speed = 500.0;
    const gfx::Texture m_texture;

    enum class SpriteState {
        IdleNorth,
        IdleSidewards,
        IdleSouth,
        WalkingNorth,
        WalkingSidewards,
        WalkingSouth,
    };

    Spritesheet<SpriteState> m_spritesheet{ m_texture, 0.1 };

    static constexpr float m_sprite_width = 15;
    static constexpr float m_sprite_height = 22;
    static constexpr float m_texture_scale = 5.0f;
    Direction m_direction = Direction::South;
    bool m_is_idle = true;

public:
    explicit Player(gfx::Vec position)
        : m_position(position)
        , m_texture("./assets/Cute_Fantasy_Free/Player/Player.png")
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

    [[nodiscard]] bool is_idle() const {
        return m_is_idle;
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

    void update() {
        m_is_idle = true;
    }

    void draw(gfx::Renderer& rd) {

        switch (m_direction) {
            using enum Direction;

            case North: {
                auto state = m_is_idle ? SpriteState::IdleNorth : SpriteState::WalkingNorth;
                m_spritesheet.draw(rd, state, get_hitbox());
            } break;

            case East: {
                auto state = m_is_idle ? SpriteState::IdleSidewards : SpriteState::WalkingSidewards;
                m_spritesheet.draw(rd, state, get_hitbox());
            } break;

            case West: {
                auto state = m_is_idle ? SpriteState::IdleSidewards : SpriteState::WalkingSidewards;
                m_spritesheet.draw_mirrored(rd, state, get_hitbox());
            } break;

            case South: {
                auto state = m_is_idle ? SpriteState::IdleSouth : SpriteState::WalkingSouth;
                m_spritesheet.draw(rd, state, get_hitbox());
            } break;

        }

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
