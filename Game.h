#pragma once

#include <print>

#include <gfx.h>

#include "GamePlaying.h"
#include "GameTitlescreen.h"

class Game : public GameObject {

    gfx::Window& m_window;
    gfx::Font m_font;

    GamePlaying m_game_playing;
    GameTitlescreen m_game_titlescreen;

    enum class State {
        Titlescreen,
        Playing,
    } m_state = State::Playing;

public:
    explicit Game(gfx::Window& window)
        : m_window(window)
        , m_font(m_window.load_font("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf"))
        , m_game_playing(m_window, m_font)
        , m_game_titlescreen(m_window, m_font)
    { }

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
    void handle_inputs([[maybe_unused]] double dt) {

        // TODO: use the event system for this

        if (m_window.get_key_state(gfx::Key::Escape).pressed())
            m_window.close();
    }

};
