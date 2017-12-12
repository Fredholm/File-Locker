#ifndef DATABASE_H
#define DATABASE_H

#include "User.h"
#include "NonCopyable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Database : public NonCopyable
{
public:
    Database();
    ~Database();

    bool Run();
    void ControlPanel();
    void ControlAddUser();

private:
    void AddUser(char* name, char* pass);
    void DefaultUser(User* user);
    void PrintUserInformation(User* user);

    User**  m_Users;
    int     m_NumberOfUsers;
};

#endif // !DATABASE_H
