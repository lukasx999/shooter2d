#pragma once

template <typename Payload>
struct IListener {
    virtual ~IListener() = default;
    virtual void on_notify(Payload payload) = 0;
};
