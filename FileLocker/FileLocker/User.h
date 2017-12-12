#ifndef USER_H
#define USER_H

#include "SaveFile.h"

#define MAX_CHAR_NAME   24
#define MAX_CHAR_PASS   24
#define MAX_CHAR_CODE   24

struct User
{
    char        s_Username   [MAX_CHAR_NAME];       // Username for this user
    char        s_Password   [MAX_CHAR_PASS];       // password needed to login
    char        s_Code       [MAX_CHAR_CODE];       // Encryption key 
    SaveData    s_SaveData;                         // Saved data for this user 
};

#endif // !USER_H