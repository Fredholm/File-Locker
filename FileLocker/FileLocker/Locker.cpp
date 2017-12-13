#include "Locker.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define EMPTY_STRING_SIZE   5
#define EMPTY_STRING        "Empty"
#define INVALID_STRING      "Invalid"

Locker::Locker() { }

Locker::~Locker() { }

void Locker::Setup(User* user)
{
    m_User      = user;
    m_Logout    = false;
    m_Quit      = false;
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
        PrintSaveFiles();
        printf("\n~Active File:\n Name: %s\n", m_ContentFile);
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
    SaveData* saveData  = &m_User->s_SaveData;
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
    // Check if a file is active first
    if (strcmp(m_Content,       EMPTY_STRING) == 0 ||
        strcmp(m_ContentFile,   EMPTY_STRING) == 0)
    {
        printf("No active file.\n");
        return;
    }

    // Write the content down to the file
    FILE *ft;
    int ch;
    ft = fopen(m_ContentFile, "w");
    char buffer[MAX_CHAR_CONTENT];
    
    // Encrypt the content
    char encrypted[MAX_CHAR_CONTENT];
    memset(encrypted, '\0', MAX_CHAR_CONTENT);
    for (size_t i = 0, iKey = 0; i < strlen(m_Content); i++, iKey++)
    {
        if (iKey == strlen(m_User->s_Code)) iKey = 0;
        printf("Encrypting: %d, with %d, resulting in: %d\n", m_Content[i], m_User->s_Code[iKey], ((m_Content[i] - 32 + m_User->s_Code[iKey] - 32) % 95) + 32);
        if (m_Content[i] >= 32 || m_Content[i] < 126) encrypted[i] = m_Content[i];
        else encrypted[i] = ((m_Content[i] - 32 + m_User->s_Code[iKey] - 32) % 95) + 32;
    }
    fwrite(encrypted, 1, MAX_CHAR_CONTENT, ft);
    fclose(ft);

    // Saving the savefile inside the user's information
    SaveFile** ptr = m_User->s_SaveData.s_Saved;
    int counter = m_User->s_SaveData.s_NumberOfSavedFiles;
    while (counter > 0)
    {
        counter--;
        ptr++;
    }
    *ptr = (SaveFile*)malloc(sizeof(SaveFile));
    memcpy((*ptr)->s_Path, m_ContentFile, MAX_CHAR_FILE);

    printf("Save completed.\n");
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
    char pass_1[MAX_CHAR_FILE];
    char pass_2[MAX_CHAR_FILE];

    // Get password input from user
    printf("Change password\n Password: ");
    scanf("%23s", &pass_1);
    printf("Again please.\n Password: ");
    scanf("%23s", &pass_2);

    // Check if passwords are correct
    if (strcmp(pass_1, pass_2) == 0)
    {
        // Changes the password
        memcpy(m_User->s_Password, pass_1, MAX_CHAR_PASS);
        printf("Your password have been changed.\n\n");
    }
    else
    {
        // Wrong password
        printf("The two passwords didn't match.\n");
        printf("Your password have not been changed.\n\n");
    }
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
    if (strcmp(m_Content, INVALID_STRING) == 0)
    {
        system("cls"); 
        printf("\nFile does not exist\n");
        ResetCurrent();
        return; 
    }

    int pos = strlen(m_Content);

    fseek(stdin, 0, SEEK_END);
    while (!done)
    {
        // Print current editing screen information
        printf("Editing %s\n****************\n", filename);
        printf("%s", m_Content);

        // Simple single character read to get the typewriter effect, 
        //  it automaticly get's looped because stdin doesn't get flushed
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
            m_Content[pos - 2] = '\0';
            m_Content[pos - 1] = '\0';
            done = true;
            system("cls");
            printf("Saving file and exiting.\n");
        }       
        system("cls");
    }
}

char* Locker::GetFileContent(char* filename)
{
    char buffer[MAX_CHAR_CONTENT] = INVALID_STRING;

    // Creates the actual file
    FILE *ft;
    int ch;
    ft = fopen(filename, "r");
    if (ft)
    {
        fread(buffer, 1, MAX_CHAR_CONTENT, ft);
        fclose(ft);

        // Skip this if not filled
        if (strcmp(buffer, "") == 0) return buffer;

        // Decrypt the content
        char decrypted[MAX_CHAR_CONTENT];
        memset(decrypted, '\0', MAX_CHAR_CONTENT);
        for (size_t i = 0, iKey = 0; i < strlen(m_Content); i++, iKey++)
        {
            if (iKey == strlen(m_User->s_Code)) iKey = 0;          
            printf("Decrypting: %d, with %d, resulting in: %d (%c)\n", buffer[i], m_User->s_Code[iKey], ((buffer[i] + 32 - m_User->s_Code[iKey] + 32) % 95) + 32 + 31, ((buffer[i] + 32 - m_User->s_Code[iKey] + 32) % 95) + 32 + 31);
            if (buffer[i] >= 32 || buffer[i] < 126) decrypted[i] = buffer[i];
            else decrypted[i] = ((buffer[i] + 32 - m_User->s_Code[iKey] + 32) % 95) + 32 + 31;
        }
        memcpy(buffer, decrypted, MAX_CHAR_CONTENT);
    }

    return buffer;
}

void Locker::ResetCurrent()
{
    memset(m_Content,       '\0',           MAX_CHAR_CONTENT);  
    memset(m_ContentFile,   '\0',           MAX_CHAR_FILE); 
    memcpy(m_Content,       EMPTY_STRING,   EMPTY_STRING_SIZE);
    memcpy(m_ContentFile,   EMPTY_STRING,   EMPTY_STRING_SIZE);
}

void Locker::PrintSaveFiles()
{
    printf("~Your Savefiles: \n");
    SaveFile** ptr = m_User->s_SaveData.s_Saved;
    int counter = m_User->s_SaveData.s_NumberOfSavedFiles;
    if (counter == 0) printf(" No saved files.\n");
    while (counter > 0)
    {
        PrintSaveFile(*ptr);
        counter--;
        ptr++;
    }
}

void Locker::PrintSaveFile(SaveFile* sf)
{
    printf(" Path: %s\n", sf->s_Path);
}