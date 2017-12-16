#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/*******************
/*  C INCLUDES
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************
/*  DEFINES
*******************/

// Defines for max capacities
#define MAX_SAVE_FILES      248
#define MAX_CHAR_CONTENT    1000
#define MAX_CHAR_FILE       24
#define MAX_CHAR_NAME       24
#define MAX_CHAR_PASS       24
#define MAX_CHAR_CODE       24

// DB Defines
#define _PRINT_INFO         false   // Prints extra information for debugging uses
#define MAX_USERS           512

// Locker Defines
#define EMPTY_STRING_SIZE   5
#define EMPTY_STRING        "Empty"
#define INVALID_STRING      "Invalid"

/*******************
/*  STRUCTS
*******************/

// Forward Deceleration
struct SaveFile
{
    char                s_Path[MAX_CHAR_FILE];                 // The Path to the saved file
};

struct SaveData
{
    struct SaveFile**   s_Saved;                        // The data for each savefile
    int                 s_NumberOfSavedFiles;           // The number of saved data files
};

struct User
{
    struct SaveData     s_SaveData;                     // Saved data for this user 
    char                s_Username[MAX_CHAR_NAME];      // Username for this user
    char                s_Password[MAX_CHAR_PASS];      // password needed to login
    char                s_Code[MAX_CHAR_CODE];          // Encryption key 
};

/*******************
/*  DATABASE
*******************/
int RunDB();

int StartMenuPanel(struct User* userActive, struct User** users, int* count);
void StartMenuLogin(struct User* userActive, struct User** users, int* count);
void StartMenuCreate(struct User** users, int* count);

// Internal Database functions
struct User* CheckCredentials(struct User** users, int* count, char* name, char* pass);
void AddUser(struct User** users, int* count, char* name, char* pass);
void DefaultUser(struct User* user);
void PrintUserInformation(struct User* user);
void PrintAllUsers(struct User** users, int* count);

/*******************
/*  LOCKER
*******************/
void iRunLocker(struct User* user, int* done);
     
void iLockerWrite(struct User* user, char* content, char contentFile);
void iSaveAFile(struct User* user, char* content, char contentFile);
void iModifyFile(struct User* user, char* content, char contentFile);
void iShowContent(struct User* user, char* content, char contentFile);
void iChangePassword(struct User* user);

// Internal Locker Functions
void iEditFile(struct User* user, char* filename, char* content, char* contentFile);
char* iGetFileContent(struct User* user, char* filename, char* content, char* contentFile);
void iResetCurrent(char* content, char contentFile);
void iPrintSaveFiles(struct User* user);
void iPrintSaveFile(struct SaveFile* sf);

#endif // !FUNCTIONS_H
