#pragma once

#include <print>
#include <vector>

#include <gfx.h>

#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "GameObject.h"
#include "Map.h"

class Game : public GameObject {
    gfx::Renderer& m_rd;
    gfx::Font m_font;
    Map m_map;
    Player m_player;
    Enemy m_enemy;

    std::vector<std::reference_wrapper<GameObject>> m_objects {
        m_map,
        m_player,
        m_enemy,
    };

    std::vector<std::reference_wrapper<Entity>> m_entities {
        m_player,
        m_enemy,
    };

public:
    explicit Game(gfx::Renderer& rd)
        : m_rd(rd)
        , m_font(m_rd.load_font("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf"))
        , m_map("./assets/map.tmx")
        , m_player(m_rd.get_window(), { m_rd.get_window().get_width() / 2.0f, m_rd.get_window().get_height() / 2.0f })
        , m_enemy(m_rd.get_window(), { m_rd.get_window().get_width() / 2.0f, m_rd.get_window().get_height() / 2.0f }, m_player)
    { }

    void draw(gfx::Renderer& rd) const override {

        rd.set_camera(m_player.get_position());
        rd.with_camera([&] {
            for (auto& obj : m_objects)
                obj.get().draw(rd);
        });

        auto text = std::format("Health: {}", m_player.get_health());
        rd.draw_text(0, 0, 50, text.c_str(), m_font, gfx::Color::white());
        rd.draw_text(0, 50, 50, std::format("{}", m_player.get_state()), m_font, gfx::Color::white());
        rd.draw_text(0, 100, 50, std::format("{}", m_player.get_direction()), m_font, gfx::Color::white());

    }

    void update(double dt) override {

        for (auto& obj : m_objects)
            obj.get().update(dt);

        handle_inputs(dt);

        m_map.resolve_collisions(m_rd.get_window(), m_entities, dt);
        m_map.resolve_collisions_entities(m_enemy, m_player, dt);
    }

private:
    void handle_inputs(double dt) {
        auto& window = m_rd.get_window();

        using enum Direction;

        // TODO: use the event system for this

        bool up = window.get_key_state(gfx::Key::W).pressed();
        bool right = window.get_key_state(gfx::Key::D).pressed();
        bool down = window.get_key_state(gfx::Key::S).pressed();
        bool left = window.get_key_state(gfx::Key::A).pressed();

        auto direction = input_to_direction(up, right, left, down);

        if (direction)
            m_player.walk(*direction, dt);


        if (window.get_key_state(gfx::Key::J).pressed())
            m_player.attack();

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();
    }

    [[nodiscard]] static constexpr auto
    input_to_direction(bool up, bool right, bool left, bool down) -> std::optional<Direction> {
        using enum Direction;

        // TODO: how to handle multiple keys pressed at the same time

        if (up && right) return NorthEast;
        if (up && left) return NorthWest;
        if (down && right) return SouthEast;
        if (down && left) return SouthWest;
        if (up) return North;
        if (down) return South;
        if (right) return East;
        if (left) return West;

        return {};
    }

};
