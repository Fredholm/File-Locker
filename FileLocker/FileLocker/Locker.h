#ifndef LOCKER_H
#define LOCKER_H

#include "User.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class Locker
{
public:
    Locker();
    ~Locker();

    void Setup(User* user);
    void Run();

private:
    User* m_User;
};

#endif // !LOCKER_H
