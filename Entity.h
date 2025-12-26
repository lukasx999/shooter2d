#pragma once

#include <gfx.h>
#include <print>

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

    Direction m_direction = Direction::South;

    enum class State { Idle, Walking, Attacking };
    State m_state = State::Idle;

    const int m_width;
    const int m_height;

public:
    Entity(const gfx::Window& window, gfx::Vec position, gfx::Texture texture, int width, int height);

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
    void draw([[maybe_unused]] gfx::Renderer& rd) const override { }
    void walk(Direction dir, double dt);
    void attack();

    virtual void on_direction_change([[maybe_unused]] Direction new_direction) { }
    virtual void on_state_change([[maybe_unused]] State new_state) { }

};
