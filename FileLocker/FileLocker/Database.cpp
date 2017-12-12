#include "Database.h"

Database::Database()
{
    m_Users         = NULL;
    m_NumberOfUsers = 0;

    m_Users = (User**)malloc(sizeof(User*));

    AddUser("Name_1", "Pass_1");
    AddUser("Name_2", "Pass_2");
    AddUser("Name_3", "Pass_3");
}

Database::~Database()
{
    int counter = m_NumberOfUsers;
    while (counter > 0)
    {
        free(*m_Users);
        counter--;
        m_Users++;
    }
    free(m_Users);
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

    *m_Users = (User*)malloc(sizeof(User));
    DefaultUser(*m_Users);
    m_NumberOfUsers++;

    memcpy((*m_Users)->s_Username, name, 24);
    memcpy((*m_Users)->s_Password, pass, 24);

    PrintUserInformation(*m_Users);

    // Reset the pointer to the user array
    m_Users = original;
}

void Database::DefaultUser(User* user)
{
    // Default char arrays
    memset(user->s_Code,        'X',    MAX_CHAR_CODE);  
    memset(user->s_Password,    'X',    MAX_CHAR_PASS);  
    memset(user->s_Username,    'X',    MAX_CHAR_NAME);  
    user->s_Code    [MAX_CHAR_CODE - 1] = '\0';
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

bool Database::Run()
{
    printf("\n%d Number of users in the databse.\n", m_NumberOfUsers);

    bool done = false;
    while (!done)
    {
        ControlPanel();
    }

    return done;
}

void Database::ControlPanel()
{
    int selection = -1;

    printf("1. Print all users in database.\n2. Create User.\n3. Login\n. Input: ");
    scanf("%d", &selection);

    switch (selection)
    {
    case 1: break;
    case 2: ControlAddUser(); break;
    case 3: break;
    default: break;
    }
}

void Database::ControlAddUser()
{
    char name[24];
    char pass[24];

    printf("Username: ");
    scanf("%23s", name);
    printf("Password: ");
    scanf("%23s", pass);

    AddUser(name, pass);
}