#pragma once

#include <vector>

#include "IListener.h"

template <typename Payload>
class Emitter {
    using Listener = IListener<Payload>;

protected:
    std::vector<std::reference_wrapper<Listener>> m_listeners;

    void notify(Payload payload) {
        for (auto& listener : m_listeners)
        listener.get().on_notify(payload);
    }

public:
    void add_listener(Listener& listener) {
        m_listeners.push_back(listener);
    }

    void remove_listener(Listener& listener) {
        auto iter = std::ranges::find_if(m_listeners, [&](auto l) {
            return &l.get() == &listener;
        });

        assert(iter != m_listeners.end());
        m_listeners.erase(iter);
    }

};
