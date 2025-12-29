#pragma once

#include <gfx.h>
#include <print>

#include "GameObject.h"

enum class Direction { North, East, South, West };

[[nodiscard]] inline constexpr const char* stringify_direction(Direction direction) {
    switch (direction) {
        using enum Direction;
        case North: return "North";
        case East:  return "East";
        case South: return "South";
        case West:  return "West";
    }
}

class Entity : public GameObject {
protected:
    // TODO: this should be public
    enum class State { Idle, Walking, Attacking };

    static constexpr int m_max_health = 100;
    static constexpr float m_movement_speed = 500;
    const int m_width;
    const int m_height;
    const gfx::Window& m_window;

    gfx::Vec m_position;
    Direction m_direction = Direction::South;
    State m_state = State::Idle;
    int m_health = m_max_health;

    bool m_is_idle = false;
    bool m_idle_lock = false;
    bool m_is_holding_walk_button = false;

public:
    Entity(const gfx::Window& window, gfx::Vec position, int width, int height);

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
    }

    [[nodiscard]] int get_health() const {
        return m_health;
    }

    [[nodiscard]] float get_movement_speed() const {
        return m_movement_speed;
    }

    [[nodiscard]] Direction get_direction() const {
        return m_direction;
    }

    [[nodiscard]] State get_state() const {
        return m_state;
    }

    void set_position(gfx::Vec position) {
        m_position = position;
    }

    [[nodiscard]] static constexpr const char* stringify_state(State state) {
        switch (state) {
            using enum State;
            case Idle:      return "Idle";
            case Walking:   return "Walking";
            case Attacking: return "Attacking";
        }
    }

    [[nodiscard]] gfx::Rect get_hitbox() const;
    void update(double dt) override;
    void draw([[maybe_unused]] gfx::Renderer& rd) const override { }
    void walk(Direction dir, double dt);
    void attack();

protected:
    virtual void on_direction_change([[maybe_unused]] Direction new_direction) { }
    virtual void on_state_change([[maybe_unused]] State new_state) { }
    [[nodiscard]] virtual bool is_attack_done() const = 0;

};
