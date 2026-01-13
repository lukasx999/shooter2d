#include "GameStates.h"
#include "StatePaused.h"

GameState* StatePaused::update([[maybe_unused]] double dt) {
    if (m_window.get_key_state(gfx::Key::P).pressed())
        return &m_states.m_state_playing;

    return nullptr;
}
