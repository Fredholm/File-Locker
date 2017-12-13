#include "Locker.h"

// File Manipulation
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

Locker::Locker() { }

Locker::~Locker() { }

void Locker::Setup(User* user)
{
    m_User = user;
    m_Logout = false;
    m_Quit = false;
    ResetCurrent();
}

void Locker::Run()
{
    // Double check to see if the locker is correct
    if (m_User == NULL)
    {
        printf("Locker have not been setup correcly.\n");
        return;
    }

    // Main loop for the locker
    while (!m_Logout)
    {
        char selection = '0';
        printf("\n~Active File.\n Name: %s\n", m_ContentFile);
        printf("\n1. Create new file.\n2. Save active file.\n3. Modify old file.\n4. Print content from file.\n5. Change Password.\n6. Logout.\n7. Quit.\n Input: ");
        fseek(stdin, 0, SEEK_END);
        scanf("%c", &selection);
        system("cls");
        if (!isalpha(selection))
        {
            switch (selection - '0')
            {
            case 1: LockerWrite();      break;
            case 2: SaveAFile();        break;
            case 3: ModifyFile();       break;
            case 4: ShowContent();      break;
            case 5: ChangePassword();   break;
            case 6: Logout();           break;
            case 7: Quit();             break;
            default:                    break;
            }
        }
        else printf("Please insert a number, not a letter.\n");
    }

    return;
}

void Locker::LockerWrite()
{
    SaveData* saveData = &m_User->s_SaveData;
    SaveFile** traveler = saveData->s_Saved;

    // Traversing through array
    int counter = saveData->s_NumberOfSavedFiles;
    while (counter > 0)
    {
        counter--;
        traveler++;
    }

    // Get filename from user
    char name[MAX_CHAR_FILE];
    printf("Create File\n File Name: ");
    scanf("%23s", &name);

    // Saves the path for this file
    (*traveler) = (SaveFile*)malloc(sizeof(SaveFile));
    memcpy((*traveler)->s_Path, &name, MAX_CHAR_FILE);
    saveData->s_NumberOfSavedFiles++;

    // Creates the actual file
    FILE *ft;
    int ch;
    ft = fopen(name, "w+");
    char buffer[MAX_CHAR_CONTENT];
    memset(buffer, '\0', MAX_CHAR_CONTENT - 1);
    fwrite(buffer, 1, MAX_CHAR_CONTENT, ft);
    fclose(ft);

    // Opens the file to edit
    EditFile(name);
}

void Locker::SaveAFile()
{

}

void Locker::ModifyFile()
{
    // Get filename from user
    char name[MAX_CHAR_FILE];
    printf("Edit File\n File Name: ");
    scanf("%23s", &name);

    // Opens the file to edit
    EditFile(name);
}

void Locker::ShowContent()
{
    printf("Content in active file:\n %s", m_Content);
}

void Locker::ChangePassword()
{
}

void Locker::Logout()
{
    m_User      = NULL;
    m_Logout    = true;
    m_Quit      = false;
}

void Locker::Quit()
{
    m_User      = NULL;
    m_Logout    = true;
    m_Quit      = true;
}

void Locker::EditFile(char* filename)
{
    bool done = false;
    memcpy(m_Content, GetFileContent(filename), MAX_CHAR_CONTENT);
    memcpy(m_ContentFile, filename, MAX_CHAR_FILE);
    
    // File not found, reset everything, bad solution, fix this
    if (strcmp(m_Content, "Invalid") == 0) 
    {
        system("cls"); 
        printf("\nFile does not exist\n");
        ResetCurrent();
        return; 
    }

    int pos = strlen(m_Content);

    while (!done)
    {
        // Print current editing screen information
        system("cls");
        printf("Editing %s\n****************\n", filename);
        printf("%s", m_Content);

        // Simple single character read
        int nread = -1;
        if (((nread = getc(stdin)) != EOF))
        {
            // Normal Char
            m_Content[pos] = nread;
            m_Content[pos + 1] = '\0';
            pos++;
        }

        // Quiting steps
        if (m_Content[pos - 2] == '-' &&
            m_Content[pos - 1] == '1')
        {
            done = true;
            system("cls");
            printf("Saving file and exiting.\n");
        }       
    }
}

char* Locker::GetFileContent(char* filename)
{
    char buffer[MAX_CHAR_CONTENT] = "Invalid";

    // Creates the actual file
    FILE *ft;
    int ch;
    ft = fopen(filename, "r");
    if (ft)
    {
        fread(buffer, 1, MAX_CHAR_CONTENT, ft);
        fclose(ft);
    }

    return buffer;
}

void Locker::ResetCurrent()
{
    memset(m_Content,       '\0', MAX_CHAR_CONTENT);  
    memset(m_ContentFile,   '\0', MAX_CHAR_FILE); 
    memcpy(m_Content,       "Empty", 5);
    memcpy(m_ContentFile,   "Empty", 5);
}