#include "Database.h"

Database::Database()
{
    m_Users         = NULL;
    m_Locker        = NULL;
    m_NumberOfUsers = 0;

    m_Users = (User**)malloc(sizeof(User*));

    AddUser("Name_1", "Pass_1");
    AddUser("Name_2", "Pass_2");
    AddUser("Name_3", "Pass_3");
}

Database::~Database()
{
    int counter = m_NumberOfUsers;
    User** original = m_Users;
    while (counter > 0)
    {
        free(*m_Users);
        counter--;
        m_Users++;
    }
    free(original);

    if (m_Locker != NULL)
        free(m_Locker);
}

bool Database::Run()
{
    printf("\n%d Number of users in the databse.\n", m_NumberOfUsers);

    bool done = false;
    while (!done)
    {
        // Logged in
        if (m_Locker != NULL)
            m_Locker->Run();

        // NOT logged in
        else
            StartMenuPanel();
    }

    return done;
}

void Database::StartMenuPanel()
{
    int selection = -1;

    printf("\n1. Login\n2. Create User.\n(DEBUG)3. Print all users in database.\n Input: ");
    scanf("%d", &selection);
    system("cls");

    switch (selection)
    {
    case 1: StartMenuLogin(); break;
    case 2: StartMenuCreate(); break;
    case 3: PrintAllUsers(); break;
    default: break;
    }
}

void Database::StartMenuCreate()
{
    char name[24];
    char pass[24];

    printf("Create an account!\n");
    printf("*******\n");
    printf("Username: ");
    scanf("%23s", name);
    printf("*******\n");
    printf("Password: ");
    scanf("%23s", pass);

    AddUser(name, pass);
}

void Database::StartMenuLogin()
{
    char name[24];
    char pass[24];

    printf("Login!\n");
    printf("*******\n");
    printf("Username: ");
    scanf("%23s", name);
    printf("*******\n");
    printf("Password: ");
    scanf("%23s", pass);

    if (User* user = CheckCredentials(name, pass))
    {
        // Setup the locker
        size_t size = sizeof(Locker);
        printf("Size: %d\n", size);
        m_Locker = (Locker*)malloc(sizeof(Locker));
        m_Locker->Setup(user);
    }
}

User* Database::CheckCredentials(char* name, char* pass)
{
    User* success = NULL;
    User** original = m_Users;
    int counter = m_NumberOfUsers;
    while (counter > 0 && !success)
    {
        if (strcmp(name, (*m_Users)->s_Username) == 0)
        { 
            if (strcmp(pass, (*m_Users)->s_Password) == 0)
            {
                success = *m_Users;
            }
            else
            {
                printf("Wrong password.\n");
            }
        }
        counter--;
        m_Users++;
    }
    m_Users = original;

    return success;
}

void Database::AddUser(char* name, char* pass)
{
    // Saving the original pointer to the user array
    User** original = m_Users;

    // Traversing through array
    int counter = m_NumberOfUsers;
    while (counter > 0)
    {
        counter--;
        m_Users++;
    }

    // Creating a defaulted user object
    *m_Users = (User*)malloc(sizeof(User));
    DefaultUser(*m_Users);
    m_NumberOfUsers++;

    // Setting inserted name and password
    memcpy((*m_Users)->s_Username, name, 24);
    memcpy((*m_Users)->s_Password, pass, 24);

    // Reset the pointer to the user array
    m_Users = original;
}

void Database::DefaultUser(User* user)
{
    // Default char arrays
    memset(user->s_Code, 'X', MAX_CHAR_CODE);
    memset(user->s_Password, 'X', MAX_CHAR_PASS);
    memset(user->s_Username, 'X', MAX_CHAR_NAME);
    user->s_Code[MAX_CHAR_CODE - 1] = '\0';
    user->s_Password[MAX_CHAR_PASS - 1] = '\0';
    user->s_Username[MAX_CHAR_NAME - 1] = '\0';

    // Default save data
    user->s_SaveData.s_NumberOfSavedFiles = 0;
    user->s_SaveData.s_Saved = NULL;
}

void Database::PrintUserInformation(User * user)
{
    printf("\nPrinting user information\n");
    printf(" Name: \t\t%s\n", user->s_Username);
    printf(" Password: \t%s\n", user->s_Password);
    printf(" Code: \t\t%s\n", user->s_Code);
    printf(" Saved Files: \t%d\n", user->s_SaveData.s_NumberOfSavedFiles);
}

void Database::PrintAllUsers()
{
    User** original = m_Users;
    int counter = m_NumberOfUsers;
    while (counter > 0)
    {
        PrintUserInformation(*m_Users);
        counter--;
        m_Users++;
    }
    m_Users = original;
}