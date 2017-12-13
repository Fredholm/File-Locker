#ifndef LOCKER_H
#define LOCKER_H

#include "User.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_CHAR_CONTENT 1000

class Locker
{
public:
    Locker();
    ~Locker();

    void Setup(User* user);
    void Run();

    bool GetQuit()      { return m_Quit;    }
    bool GetLogout()    { return m_Logout;  }

private:
    void LockerWrite();
    void SaveAFile();
    void ModifyFile();
    void ShowContent();
    void ChangePassword();
    void Logout();
    void Quit();

    // Internal uses
    void EditFile(char* filename);
    char* GetFileContent(char* filename);
    void ResetCurrent();
    void PrintSaveFiles();
    void PrintSaveFile(SaveFile* sf);

    char    m_ContentFile[MAX_CHAR_FILE];
    char    m_Content[MAX_CHAR_CONTENT];
    bool    m_Quit;
    bool    m_Logout;
    User*   m_User;
};

#endif // !LOCKER_H
