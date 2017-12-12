#ifndef DATABASE_H
#define DATABASE_H

#include "User.h"
#include "NonCopyable.h"
#include "Locker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Database : public NonCopyable
{
public:
    Database();
    ~Database();

    int Run();

    bool StartMenuPanel();
    void StartMenuLogin();
    void StartMenuCreate();

private:
    User* CheckCredentials(char* name, char* pass);
    void AddUser(char* name, char* pass);
    void DefaultUser(User* user);
    void PrintUserInformation(User* user);
    void PrintAllUsers();

    Locker* m_Locker;
    User**  m_Users;
    int     m_NumberOfUsers;
};

#endif // !DATABASE_H
