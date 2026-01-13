#pragma once

#include <functional>

#include "../GameObject.h"

enum class State {
    Titlescreen,
    Playing,
    Paused,
};

class GameState : public GameObject {
protected:
    using StateChangeFn = std::function<void(State)>;
    StateChangeFn m_on_state_change = [](State) { };

public:
    void on_state_change(StateChangeFn on_state_change) {
        m_on_state_change = on_state_change;
    }

};
