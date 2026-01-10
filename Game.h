#pragma once

#include <print>
#include <vector>

#include <gfx.h>

#include "ControllableEntity.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "GameObject.h"
#include "Map.h"

class GamePlaying : public GameObject {
    gfx::Renderer& m_rd;
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
    explicit GamePlaying(gfx::Renderer& rd, const gfx::Font& font)
        : m_rd(rd)
        , m_font(font)
        , m_map("./assets/map.tmx")
        , m_player(m_rd.get_window(), { m_rd.get_window().get_width() / 2.0f, m_rd.get_window().get_height() / 2.0f })
        , m_enemy(m_rd.get_window(), { m_rd.get_window().get_width() / 2.0f, m_rd.get_window().get_height() / 2.0f }, m_player)
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

        auto& window = m_rd.get_window();

        if (window.get_key_state(gfx::Key::J).pressed())
            m_player.attack();

        m_map.resolve_collisions(m_rd.get_window(), m_entities, dt);
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

class GameTitlescreen : public GameObject {
    gfx::Renderer& m_rd;
    const gfx::Font& m_font;

    gfx::ui::Button m_button { m_rd.get_window(), m_font, "start game", { 0, 0, 500, 100 }, 50 };
public:

    GameTitlescreen(gfx::Renderer& rd, const gfx::Font& font)
        : m_rd(rd)
        , m_font(font)
    {
        m_button.on_press([&] {
            // TODO: transition state
        });
    }

    void draw(gfx::Renderer& rd) const override {
        rd.draw_text_centered(rd.get_window().get_width()/2.0, 0, 50, "epic game", m_font, gfx::Color::white());
        m_button.draw(rd);
    }

    void update([[maybe_unused]] double dt) override {
        m_button.update();
    }

};

class Game : public GameObject {

    gfx::Renderer& m_rd;
    gfx::Font m_font;

    GamePlaying m_game_playing;
    GameTitlescreen m_game_titlescreen;

    enum class State {
        Titlescreen,
        Playing,
    } m_state = State::Playing;

public:
    explicit Game(gfx::Renderer& rd)
        : m_rd(rd)
        , m_font(m_rd.load_font("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf"))
        , m_game_playing(m_rd, m_font)
        , m_game_titlescreen(m_rd, m_font)
    {
    }

    void draw(gfx::Renderer& rd) const override {
        switch (m_state) {
            using enum State;

            case Titlescreen:
                m_game_titlescreen.draw(rd);
                break;

            case Playing:
                m_game_playing.draw(rd);
                break;
        }
    }

    void update(double dt) override {

        handle_inputs(dt);

        switch (m_state) {
            using enum State;

            case Titlescreen:
                m_game_titlescreen.update(dt);
                break;

            case Playing:
                m_game_playing.update(dt);
                break;
        }

    }

private:
    void handle_inputs(double dt) {
        auto& window = m_rd.get_window();

        // TODO: use the event system for this

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();
    }

};
