#pragma once

#include <gfx/gfx.h>

#include "GameState.h"

class StatePaused : public GameState {
    const gfx::Window& m_window;
    const gfx::Font& m_font;

public:
    StatePaused(const gfx::Window& window, const gfx::Font& font, GameStates& states)
        : GameState(states)
        , m_window(window)
        , m_font(font)
    { }

    void draw(gfx::Renderer& rd) const override {
        // m_states.m_state_playing.draw(rd);

        rd.draw_rectangle(0, 0, m_window.get_width(), m_window.get_height(), gfx::Color::black().set_alpha(128));
        rd.draw_text(0, 0, 50, "paused.", m_font, gfx::Color::white());
    }

    GameState* update([[maybe_unused]] double dt) override;
};
