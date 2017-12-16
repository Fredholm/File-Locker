#include "functions.h"

int RunDB()
{
//    Locker* m_Locker;
    struct User**   m_Users = (struct User**)malloc(sizeof(struct User*) * MAX_USERS);
    int*            m_NumberOfUsers = (int*)malloc(sizeof(int));
    *m_NumberOfUsers = 0;

    AddUser(m_Users, m_NumberOfUsers, "1", "2");
    AddUser(m_Users, m_NumberOfUsers, "Name_2", "Pass_2");
    AddUser(m_Users, m_NumberOfUsers, "Name_3", "Pass_3");

    int done = 1;
    while (done == 1)
    {
        // Logged in
        //if (m_Locker != NULL)
        //{
        //    m_Locker->Run();

        //    // Quick Exit
        //    if (m_Locker->GetQuit())
        //    {
        //        done = 0;
        //    }

        //    // Logout
        //    else if (m_Locker->GetLogout())
        //    {
        //        free(m_Locker);
        //        m_Locker = NULL;
        //    }
        //}

        //// NOT logged in
        //else
            done = StartMenuPanel(m_Users, m_NumberOfUsers);
    }

    int* counter = m_NumberOfUsers;
    struct User** original = m_Users;
    while (counter > 0)
    {
        struct User* user = *m_Users;

        // Remove each user's savefiles
        int saveFileCounter = user->s_SaveData.s_NumberOfSavedFiles;
        struct SaveFile** savefiles = user->s_SaveData.s_Saved;
        while (saveFileCounter > 0)
        {
            free(savefiles);
            saveFileCounter--;
            savefiles++;
        }

        // Remove user
        free(user);
        counter--;
        m_Users++;
    }

    //if (m_Locker != NULL)
    //    free(m_Locker);

    free(m_NumberOfUsers);

    printf("\nBye!\n");

    return 0;
}

int StartMenuPanel(struct User** users, int* count)
{
    int quit = 1;
    int selection = -1;

    printf("\n1. Login\n2. Create User.\n3. Quit\n(DEBUG)4. Print all users in database.\n Input: ");
    scanf("%d", &selection);

    switch (selection)
    {
    case 1: StartMenuLogin(users, count);   break;
    case 2: StartMenuCreate(users, count);  break;
    case 3: quit = 0;                       break;
    case 4: PrintAllUsers(users, count);    break;
    default: break;
    }

    return quit;
}

void StartMenuCreate(struct User** users, int* count)
{
    char name[MAX_CHAR_NAME];
    char pass[MAX_CHAR_PASS];

    printf("\n\n\nCreate an account!\n");
    printf("*******\n");
    printf("Username: ");
    scanf("%23s", name);
    printf("*******\n");
    printf("Password: ");
    scanf("%23s", pass);

    AddUser(users, count, name, pass);
}

void StartMenuLogin(struct User** users, int* count)
{
    char name[MAX_CHAR_NAME];
    char pass[MAX_CHAR_PASS];

    printf("\n\n\nLogin!\n");
    printf("*******\n");
    printf("Username: ");
    scanf("%23s", name);
    printf("*******\n");
    printf("Password: ");
    scanf("%23s", pass);

    struct User* user = CheckCredentials(users, count, name, pass);
    if (user != NULL)
    {
        printf("Sucess!");
        // Setup the locker
        //size_t size = sizeof(struct Locker);
        //printf("Size: %d\n", size);
        //m_Locker = (struct Locker*)malloc(sizeof(struct Locker));
        //m_Locker->Setup(user);
    }
}

struct User* CheckCredentials(struct User** users, int* count, char* name, char* pass)
{
    struct User* success = NULL;
    struct User** original = users;
    int counter = *count;
    while (counter > 0 && !success)
    {
        if (strcmp(name, (*users)->s_Username) == 0)
        {
            if (strcmp(pass, (*users)->s_Password) == 0)
            {
                success = *users;
            }
            else
            {
                printf("Wrong password.\n");
                break;
            }
        }
        else
        {
            printf("No user with that username.\n");
            break;
        }

        counter--;
        users++;
    }
    users = original;

    return success;
}

void AddUser(struct User** users, int* count, char* name, char* pass)
{
    // Saving the original pointer to the user array
    struct User** original = users;

    // Traversing through array
    int counter = *count;
    while (counter > 0)
    {
        counter--;
        users++;
    }

    // Creating a defaulted user object
    *users = (struct User*)malloc(sizeof(struct User));
    DefaultUser(*users);

    // Setting inserted name and password
    memcpy((*users)->s_Username, name, MAX_CHAR_NAME);
    memcpy((*users)->s_Password, pass, MAX_CHAR_PASS);

    // Reset the pointer to the user array
    users = original;

    // Adding one user to counter
    *count += 1;
}

void DefaultUser(struct User* user)
{
    char randomCode[MAX_CHAR_CODE];
    for (size_t i = 0; i < MAX_CHAR_CODE - 1; i++)
        randomCode[i] = rand() % 26 + 'A';
    randomCode[MAX_CHAR_CODE - 1] = '\0';
    memcpy(user->s_Code, randomCode, MAX_CHAR_CODE);

#if _DEBUG
#if _PRINT_INFO
    printf(" ~Randomized Code: %s\n", randomCode);
#endif
#endif

    // Default char arrays
    memset(user->s_Password, 'X', MAX_CHAR_PASS);
    memset(user->s_Username, 'X', MAX_CHAR_NAME);
    user->s_Password[MAX_CHAR_PASS - 1] = '\0';
    user->s_Username[MAX_CHAR_NAME - 1] = '\0';

    // Default save data
    user->s_SaveData.s_NumberOfSavedFiles = 0;
    user->s_SaveData.s_Saved = (struct SaveFile**)malloc(sizeof(struct SaveFile*) * MAX_SAVE_FILES);
}

void PrintUserInformation(struct User* user)
{
    printf("\n\n\nPrinting user information\n");
    printf(" Name: \t\t%s\n", user->s_Username);
    printf(" Password: \t%s\n", user->s_Password);
    printf(" Code: \t\t%s\n", user->s_Code);
    printf(" Saved Files: \t%d\n", user->s_SaveData.s_NumberOfSavedFiles);
}

void PrintAllUsers(struct User** users, int* count)
{
    struct User** original = users;
    int counter = *count;
    while (counter > 0)
    {
        PrintUserInformation(*users);
        counter--;
        users++;
    }
    users = original;
}