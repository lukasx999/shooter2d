#pragma once

#include <ranges>

#include "Entity.h"
#include "HealthPickup.h"
#include "Player.h"
#include "Enemy.h"
#include "GameObject.h"
#include "Map.h"
#include "misc.h"

class GamePlaying : public GameObject {
    const gfx::Window& m_window;
    const gfx::Font& m_font;
    Map m_map;
    Player m_player;
    Enemy m_enemy;
    HealthPickup m_health_pickup{10};

    std::vector<std::reference_wrapper<GameObject>> m_objects {
        m_map,
        m_player,
        m_enemy,
        m_health_pickup,
    };

    std::vector<std::reference_wrapper<Entity>> m_entities {
        m_player,
        m_enemy,
    };

public:
    GamePlaying(const gfx::Window& window, const gfx::Font& font)
        : m_window(window)
        , m_font(font)
        , m_map(ASSETS_PATH "/map.tmx")
        , m_player(m_window, { m_window.get_width() / 2.0f, m_window.get_height() / 2.0f })
        , m_enemy(m_window, { m_window.get_width() / 2.0f, m_window.get_height() / 2.0f }, m_player)
    {

        m_enemy.set_movement_speed(100);
        m_player.set_health(m_player.get_max_health() / 2);

        m_health_pickup.on_pickup([&] {
            auto it = std::ranges::find_if(m_objects, [&](auto& object) {
                return &object.get() == &m_health_pickup;
            });
            assert(it != m_objects.end());
            m_objects.erase(it);
        });
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

        for (auto& entity : m_entities)
            m_map.resolve_collisions(m_window, entity, dt);

        m_map.resolve_collisions_entities(m_enemy, m_player, dt);

        if (!m_health_pickup.is_consumed())
            m_health_pickup.check_collision_player(m_player);

    }

private:
    void draw_ui(gfx::Renderer& rd) const {
        auto text = std::format("Health: {}", m_player.get_health());
        rd.draw_text(0, 0, 50, text.c_str(), m_font, gfx::Color::white());
        rd.draw_text(0, 50, 50, std::format("{}", m_player.get_state()), m_font, gfx::Color::white());
        rd.draw_text(0, 100, 50, std::format("{}", m_player.get_direction()), m_font, gfx::Color::white());
    }

};

