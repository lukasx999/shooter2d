#include "StatePaused.h"
#include "StatePlaying.h"
#include "StateTitlescreen.h"

struct GameStates {
    StateTitlescreen m_state_titlescreen;
    StatePlaying m_state_playing;
    StatePaused m_state_paused;

    GameStates(const gfx::Window& window, const gfx::Font& font, GameStates& states)
        : m_state_titlescreen(window, font, states)
        , m_state_playing(window, font, states)
        , m_state_paused(window, font, states)
    { }

};
