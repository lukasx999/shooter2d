#pragma once

#include "ControllableEntity.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "GameObject.h"
#include "Map.h"

class GamePlaying : public GameObject {
    const gfx::Window& m_window;
    const gfx::Font& m_font;
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
    explicit GamePlaying(const gfx::Window& window, const gfx::Font& font)
        : m_window(window)
        , m_font(font)
        , m_map("./assets/map.tmx")
        , m_player(m_window, { m_window.get_width() / 2.0f, m_window.get_height() / 2.0f })
        , m_enemy(m_window, { m_window.get_width() / 2.0f, m_window.get_height() / 2.0f }, m_player)
    {

        m_enemy.set_movement_speed(100);
    }

    void draw(gfx::Renderer& rd) const override {

        rd.set_camera(m_player.get_position());
        rd.with_camera([&] {
            for (auto& obj : m_objects)
            obj.get().draw(rd);
        });

        draw_ui(rd);
    }

    void update(double dt) override {


        for (auto& obj : m_objects)
            obj.get().update(dt);

        if (m_window.get_key_state(gfx::Key::J).pressed())
            m_player.attack();

        m_map.resolve_collisions(m_window, m_entities, dt);
        m_map.resolve_collisions_entities(m_enemy, m_player, dt);
    }

private:
    void draw_ui(gfx::Renderer& rd) const {
        auto text = std::format("Health: {}", m_player.get_health());
        rd.draw_text(0, 0, 50, text.c_str(), m_font, gfx::Color::white());
        rd.draw_text(0, 50, 50, std::format("{}", m_player.get_state()), m_font, gfx::Color::white());
        rd.draw_text(0, 100, 50, std::format("{}", m_player.get_direction()), m_font, gfx::Color::white());
    }

};

