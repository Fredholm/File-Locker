#include "Locker.h"

Locker::Locker()
{
    m_User = NULL;
}

Locker::~Locker() { }

void Locker::Setup(User* user)
{
    m_User = user;
}

void Locker::Run()
{
    // Double check to see if the locker is correct
    if (m_User == NULL)
    {
        printf("Locker have not been setup correcly.\n");
        return;
    }

    // Main loop for the locker
    bool logout = false;
    while (!logout)
    {
        printf("I'm logged in.\n");
    }

    return;
}
