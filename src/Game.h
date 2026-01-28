#pragma once

#include <print>

#include <gfx/gfx.h>

#include "GameObject.h"
#include "states/GameStates.h"

class Game : public GameObject {
    gfx::Window& m_window;
    gfx::Font m_font;

    GameStates m_states;
    GameState* m_active_state = &m_states.m_state_playing;

public:
    explicit Game(gfx::Window& window)
        : m_window(window)
        , m_font(m_window.load_font("/usr/share/fonts/Adwaita/AdwaitaMono-Regular.ttf"))
        , m_states(m_window, m_font, m_states)
    { }

    void draw(gfx::Renderer& rd) const override {
        m_active_state->draw(rd);
    }

    void update(double dt) override {

        if (m_window.get_key_state(gfx::Key::Escape).pressed())
            m_window.close();

        GameState* new_state = m_active_state->update(dt);

        if (new_state != nullptr)
            m_active_state = new_state;
    }

};
