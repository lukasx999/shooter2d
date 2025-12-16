#pragma once

#include <gfx.h>
#include <print>

#include "GameObject.h"

enum class Direction { North, East, South, West };

class Entity : public GameObject {
protected:
    gfx::Vec m_position;
    static constexpr int m_max_health = 100;
    int m_health = m_max_health;

    enum class SpriteState {
        IdleNorth,
        IdleSidewards,
        IdleSouth,
        WalkingNorth,
        WalkingSidewards,
        WalkingSouth,
    };

    const gfx::Texture m_texture;
    Spritesheet<SpriteState> m_spritesheet{ m_texture, 0.1 };

    static constexpr float m_movement_speed = 500.0;
    static constexpr float m_sprite_width = 15;
    static constexpr float m_sprite_height = 22;
    static constexpr float m_texture_scale = 5.0f;
    Direction m_direction = Direction::South;
    bool m_is_idle = true;

public:
    Entity(gfx::Vec position, gfx::Texture texture)
        : m_position(position)
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
        m_is_idle = true;
    }

    void draw(gfx::Renderer& rd) const override {

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
