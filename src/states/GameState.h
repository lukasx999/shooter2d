#pragma once

#include <gfx.h>

struct GameStates;

class GameState {
protected:
    GameStates& m_states;

public:
    explicit GameState(GameStates& states) : m_states(states) { }

    virtual ~GameState() = default;
    virtual void draw(gfx::Renderer& rd) const = 0;
    [[nodiscard("state change should be handled")]] virtual GameState* update(double dt) = 0;

};
