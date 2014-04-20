#ifndef ROCKETEVENTLISTENER_H
#define ROCKETEVENTLISTENER_H

#include <Rocket/Core.h>

class RocketEventListener : public Rocket::Core::EventListener
{
public:
    RocketEventListener(int r);
    int r;

protected:
    virtual void ProcessEvent(Rocket::Core::Event& event);
    virtual void OnAttach(Rocket::Core::Element* ROCKET_UNUSED(element));
    virtual void OnDetach(Rocket::Core::Element* ROCKET_UNUSED(element));

};

#endif // ROCKETEVENTLISTENER_H
