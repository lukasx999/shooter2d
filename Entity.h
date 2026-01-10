#pragma once

#include <gfx.h>

#include "GameObject.h"
#include "misc.h"

enum class Direction {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
};

class Entity : public GameObject {
protected:
    static constexpr int m_max_health = 100;
    float m_movement_speed = 500;
    const int m_width;
    const int m_height;
    const gfx::Window& m_window;

    gfx::Vec m_position;
    Direction m_direction = Direction::South;
    int m_health = m_max_health;

    bool m_is_idle = false;
    bool m_idle_lock = false;

    bool m_is_first_iteration = true;

public:
    enum class State { Idle, Walking, Attacking };

protected:
    State m_state = State::Idle;

public:
    Entity(const gfx::Window& window, gfx::Vec position, int width, int height);

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
    }

    [[nodiscard]] int get_health() const {
        return m_health;
    }

    void set_movement_speed(float movement_speed) {
        m_movement_speed = movement_speed;
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

template <>
struct std::formatter<Direction> : std::formatter<std::string> {
    auto format(const Direction& direction, std::format_context& ctx) const {

        auto fmt = [&] {
            switch (direction) {
                using enum Direction;
                case North:     return STRINGIFY(North);
                case East:      return STRINGIFY(East);
                case South:     return STRINGIFY(South);
                case West:      return STRINGIFY(West);
                case NorthEast: return STRINGIFY(NorthEast);
                case SouthEast: return STRINGIFY(SouthEast);
                case SouthWest: return STRINGIFY(SouthWest);
                case NorthWest: return STRINGIFY(NorthWest);
            }
        }();

        return std::formatter<std::string>::format(fmt, ctx);
    }
};

template <>
struct std::formatter<Entity::State> : std::formatter<std::string> {
    auto format(const Entity::State& state, std::format_context& ctx) const {

        auto fmt = [&] {
            switch (state) {
                using enum Entity::State;
                case Idle:      return STRINGIFY(Idle);
                case Walking:   return STRINGIFY(Walking);
                case Attacking: return STRINGIFY(Attacking);
            }
        }();

        return std::formatter<std::string>::format(fmt, ctx);
    }
};
