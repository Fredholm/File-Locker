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

struct SaveFile
{
    char                s_Path[MAX_CHAR_FILE];          // The Path to the saved file
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

/********************/
/*  Database        */
/********************/
/*                  *
    All the functions here are only used when not logged in
*/

// The main loop of the program, Loops either the Start menu or the Logged in screen
// \return Returns 0 or 1 if an error accursed
int RunDB();

// Presents the different options for the player
// \return Returns 1 if the player doesn't want to quit instantly, otherwise zero
int StartMenuPanel(struct User** userActive, struct User** users, int* count);

// Asks the user for username and password, trying to log in
// \param userActive This is the pointer to the logged in user, this function will change this ptr if correct creditials
// \see CheckCreditials()
void StartMenuLogin(struct User** userActive, struct User** users, int* count);

// Asks the user to username and password & makes a new account of this
void StartMenuCreate(struct User** users, int* count);

/***********************/
/*  Internal DB Funcs  */
/***********************/

// Checks with the database to see if correct user information
// \param name The username of the user
// \param pass The password of the user
// \return Returns the pointer to the corresponding user in the database, if nothing was found, returns NULL
struct User* CheckCredentials(struct User** users, int* count, char* name, char* pass);

// Simple helper function to allocate new memory for a new user
// \see StartMenuCreate()
void AddUser(struct User** users, int* count, char* name, char* pass);

// Simply empty all information about this user, reset
void DefaultUser(struct User* user);

// Prints information about one user
// \param user Pointer to the specific user
void PrintUserInformation(struct User* user);

// Loops every user in the database and prints their information
void PrintAllUsers(struct User** users, int* count);

/********************/
/*  Locker          */
/********************/
/*                  *
    All the functions here are only used when logged in
*/

// The main loop for the logged in state of the program
// \param user A ptr to the adress of the user in question, all other corresponding functions
//              called from this function will only use the single adress, this functions needs to be able to set the ptr to NULL
void iRunLocker(struct User** user, int* done);

// Asks the user what name of the file he wants to create
// Then creates the actual file, will also start to modify the file instantly after creation
// \see iModifyFile();
void iLockerWrite(struct User* user, char* content, char* contentFile);

// Will save an encrypted file on the drive with the data saved currently inside content and contentFile
// \note Encrypting using the Vigenère cipher
void iSaveAFile(struct User* user, char* content, char* contentFile);

// Asks for a textfile name, decrypts content and opens up a editor shell
// \see iEditFile() & iGetFileContent()
void iModifyFile(struct User* user, char* content, char* contentFile);

// Asks for a textfile name, sand shows the decrypted content for the user
// \see iGetFileContent()
void iShowContent(struct User* user, char* content, char* contentFile);

// Asks for a new password from the user, twice
void iChangePassword(struct User* user);

/**********************/
/*  Internal L Funcs  */
/**********************/

// Opens up a editor loop to edit the file, with a typewriter effect
// \note The editor loop can only be exited if the user types -1 & then presses Enter
void iEditFile(struct User* user, char* filename, char* content, char* contentFile);

// Replaces the currently active content and contentfile if successful content getting
// \param filename The path to the file
// \param content Pointer to the data that should be changed
// \param contentFile Pointer to the path data that should be changed
void iGetFileContent(struct User* user, char* filename, char* content, char* contentFile);

// Reset function, clears all data from both char arrays and filled with \0 chars (empty)
void iResetCurrent(char* content, char* contentFile);

// Prints all savefiles for a user
// \param user The user in question
// \see iPrintSaveFile()
void iPrintSaveFiles(struct User* user);

// Prints a single savefile
// \see iPrintSaveFiles()
void iPrintSaveFile(struct SaveFile* sf);

#endif // !FUNCTIONS_H
