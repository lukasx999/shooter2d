#include "GameStates.h"
#include "StateTitlescreen.h"

GameState* StateTitlescreen::update([[maybe_unused]] double dt) {
    m_button.update();

    if (m_button.is_pressed())
        return &m_states.m_state_playing;

    return nullptr;

}
