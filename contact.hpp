#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <Box2D/Box2D.h>
#include "object.hpp"

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {
        b2Body *bodyA = contact->GetFixtureA()->GetBody();
        b2Body *bodyB = contact->GetFixtureB()->GetBody();

        PhysicsObject *objectA = (PhysicsObject*)bodyA->GetUserData();
        PhysicsObject *objectB = (PhysicsObject*)bodyB->GetUserData();

        if(objectA)
        {
            objectA->onBeginContact(bodyB);
        }

        if(objectB)
        {
            objectB->onBeginContact(bodyA);
        }
    }

    void EndContact(b2Contact* contact)
    {
        b2Body *bodyA = contact->GetFixtureA()->GetBody();
        b2Body *bodyB = contact->GetFixtureB()->GetBody();

        PhysicsObject *objectA = (PhysicsObject*)bodyA->GetUserData();
        PhysicsObject *objectB = (PhysicsObject*)bodyB->GetUserData();

        if(objectA)
        {
            objectA->onEndContact(bodyB);
        }

        if(objectB)
        {
            objectB->onEndContact(bodyA);
        }
    }
};

#endif