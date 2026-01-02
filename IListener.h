#pragma once

template <typename Payload>
class IListener {
public:
    virtual ~IListener() = default;
    virtual void on_notify(Payload payload) = 0;
};
