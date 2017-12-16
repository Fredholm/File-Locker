#include "functions.h"

/*******************
/*  DATABASE
*******************/

int RunDB()
{
    struct User**   m_Users = (struct User**)malloc(sizeof(struct User*) * MAX_USERS);
    int*            m_NumberOfUsers = (int*)malloc(sizeof(int));
    *m_NumberOfUsers = 0;

    AddUser(m_Users, m_NumberOfUsers, "1", "2");
    AddUser(m_Users, m_NumberOfUsers, "Name_2", "Pass_2");
    AddUser(m_Users, m_NumberOfUsers, "Name_3", "Pass_3");

    struct User* m_UserActive = NULL;
    int done = 1;
    while (done == 1)
    {
        // Logged in
        if (m_UserActive != NULL)
            iRunLocker(m_UserActive, &done);

        // NOT logged in
        else
            done = StartMenuPanel(m_UserActive, m_Users, m_NumberOfUsers);
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

int StartMenuPanel(struct User* userActive, struct User** users, int* count)
{
    int quit = 1;
    int selection = -1;

    printf("\n1. Login\n2. Create User.\n3. Quit\n(DEBUG)4. Print all users in database.\n Input: ");
    scanf("%d", &selection);

    switch (selection)
    {
    case 1: StartMenuLogin(userActive, users, count);   break;
    case 2: StartMenuCreate(users, count);              break;
    case 3: quit = 0;                                   break;
    case 4: PrintAllUsers(users, count);                break;
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

void StartMenuLogin(struct User* m_UserActive, struct User** users, int* count)
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
        m_UserActive = user;
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
            }
        }
        else
        {
            printf("No user with that username.\n");
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

/*******************
/*  LOCKER
*******************/

void iRunLocker(struct User* user, int* done)
{
    char            m_ContentFile[MAX_CHAR_FILE];
    char            m_Content[MAX_CHAR_CONTENT];
    struct User*    m_User = user;

    // Initialize char arrays
    iResetCurrent(m_Content, m_ContentFile);

    // Main loop for the locker
    int logout = 1;
    while (logout == 1)
    {
        char selection = '0';
        iPrintSaveFiles(m_User);
        printf("\n~Active File:\n Name: %s\n", m_ContentFile);
        printf("\n1. Create new file.\n2. Save active file.\n3. Modify old file.\n4. Print content from file.\n5. Change Password.\n6. Logout.\n7. Quit.\n Input: ");
        fseek(stdin, 0, SEEK_END);
        scanf("%c", &selection);
        system("cls");
        if (!isalpha(selection))
        {
            switch (selection - '0')
            {
            case 1: iLockerWrite(user, m_Content, m_ContentFile);   break;
            case 2: iSaveAFile(user, m_Content, m_ContentFile);     break;
            case 3: iModifyFile(user, m_Content, m_ContentFile);    break;
            case 4: iShowContent(user, m_Content, m_ContentFile);   break;
            case 5: iChangePassword(user);                          break;
            case 6: logout = 0;                                     break;
            case 7: *done = 0;                                      break;
            default:                                                break;
            }
        }
        else printf("Please insert a number, not a letter.\n");
    }

    user = NULL;
    return;
}

void iLockerWrite(struct User* user, char* content, char contentFile)
{
    struct SaveData* saveData = &user->s_SaveData;
    struct SaveFile** traveler = saveData->s_Saved;

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
    (*traveler) = (struct SaveFile*)malloc(sizeof(struct SaveFile));
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
    iEditFile(user, name, content, contentFile);
}

void iSaveAFile(struct User* user, char* content, char contentFile)
{
    // Check if a file is active first
    if (strcmp(content,     EMPTY_STRING) == 0 ||
        strcmp(contentFile, EMPTY_STRING) == 0)
    {
        printf("No active file.\n");
        return;
    }

    // Write the content down to the file
    FILE *ft;
    int ch;
    ft = fopen(contentFile, "w");
    char buffer[MAX_CHAR_CONTENT];

    // Encrypt the content
    char encrypted[MAX_CHAR_CONTENT];
    memset(encrypted, '\0', MAX_CHAR_CONTENT);
    for (size_t i = 0, iKey = 0; i < strlen(content); i++, iKey++)
    {
        // Resetting the key position
        if (iKey == strlen(user->s_Code)) iKey = 0;

        // Exceptions of encryptions, these characters stay the same
        if (content[i] < 32 || content[i] > 126) encrypted[i] = content[i];

        // Encrypt content character with user-code
        else encrypted[i] = ((content[i] - 32 + user->s_Code[iKey] - 32) % 95) + 32;
    }

    // Write the encrypted char array to the file
    fwrite(encrypted, 1, MAX_CHAR_CONTENT, ft);

    // Close the file
    fclose(ft);

    // Saving the savefile inside the user's information
    struct SaveFile** ptr = user->s_SaveData.s_Saved;
    int counter = user->s_SaveData.s_NumberOfSavedFiles;
    while (counter > 0)
    {
        counter--;
        ptr++;
    }

    // Creates a new save file in the first available position in the array
    *ptr = (struct SaveFile*)malloc(sizeof(struct SaveFile));
    memcpy((*ptr)->s_Path, contentFile, MAX_CHAR_FILE);
    printf("Save completed.\n");
}

void iModifyFile(struct User* user, char* content, char contentFile)
{
    // Get filename from user
    char name[MAX_CHAR_FILE];
    printf("Edit File\n File Name: ");
    scanf("%23s", &name);

    // Opens the file to edit
    iEditFile(user, name, content, contentFile);
}

void iShowContent(struct User* user, char* content, char contentFile)
{
    // Get filename from user
    char name[MAX_CHAR_FILE];
    printf("Edit File\n File Name: ");
    scanf("%23s", &name);

    // Simple print of the current file's content
    printf("Content in %s:\n %s", name, iGetFileContent(user, name, content, contentFile));
}

void iChangePassword(struct User* user)
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
        memcpy(user->s_Password, pass_1, MAX_CHAR_PASS);
        printf("Your password have been changed.\n\n");
    }
    else
    {
        // Wrong password
        printf("The two passwords didn't match.\n");
        printf("Your password have not been changed.\n\n");
    }
}

void iEditFile(struct User* user, char* filename, char* content, char* contentFile)
{
    int done = 1;

    // Copy content from file into our member
    memcpy(content, iGetFileContent(user, filename, content, contentFile), MAX_CHAR_CONTENT);

    // Also save the current content file path
    memcpy(contentFile, filename, MAX_CHAR_FILE);

    // File not found, reset everything, bad solution, fix this
    if (strcmp(content, INVALID_STRING) == 0)
    {
        system("cls");
        printf("\nFile does not exist\n");
        iResetCurrent(content, contentFile);
        return;
    }

    int pos = strlen(content);

    fseek(stdin, 0, SEEK_END);
    while (done == 1)
    {
        // Print current editing screen information
        printf("Editing %s\n****************\n", filename);
        printf("%s", content);

        // Simple single character read to get the typewriter effect, 
        //  it automaticly get's looped because stdin doesn't get flushed
        int nread = -1;
        if (((nread = getc(stdin)) != EOF))
        {
            // Normal Char
            content[pos] = nread;
            content[pos + 1] = '\0';
            pos++;
        }

        // Quiting steps
        if (content[pos - 2] == '-' &&
            content[pos - 1] == '1')
        {
            content[pos - 2] = '\0';
            content[pos - 1] = '\0';
            done = 0;
            system("cls");
            printf("Saving file and exiting.\n");
        }
        system("cls");
    }
}

char* iGetFileContent(struct User* user, char* filename, char* content, char* contentFile)
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
        for (size_t i = 0, iKey = 0; i < strlen(content); i++, iKey++)
        {
            // Resetting the key position
            if (iKey == strlen(user->s_Code)) iKey = 0;

            // Exceptions, characters that have not been encrypted
            if (buffer[i] < 32 || buffer[i] > 126) decrypted[i] = buffer[i];

            // Special case, modulus can't handle correcly
            else if ((buffer[i] + 32) - (user->s_Code[iKey] + 32) >= 0) decrypted[i] = (buffer[i] + 32) - (user->s_Code[iKey] + 32) + 32;

            // Normal decryption
            else decrypted[i] = ((buffer[i] + 32) - (user->s_Code[iKey] + 32) % 95) + 32;
        }

        // Copy the decrypted data to the buffer to use as return ptr 
        memcpy(buffer, decrypted, MAX_CHAR_CONTENT);
    }

    return buffer;
}

void iResetCurrent(char* content, char contentFile)
{
    memset(content,     '\0', MAX_CHAR_CONTENT);
    memset(contentFile, '\0', MAX_CHAR_FILE);
    memcpy(content,     EMPTY_STRING, EMPTY_STRING_SIZE);
    memcpy(contentFile, EMPTY_STRING, EMPTY_STRING_SIZE);
}

void iPrintSaveFiles(struct User* user)
{
    printf("~Your Savefiles: \n");
    struct SaveFile** ptr = user->s_SaveData.s_Saved;
    int counter = user->s_SaveData.s_NumberOfSavedFiles;
    if (counter == 0) printf(" No saved files.\n");
    while (counter > 0)
    {
        iPrintSaveFile(*ptr);
        counter--;
        ptr++;
    }
}

void iPrintSaveFile(struct SaveFile* sf)
{
    printf(" Path: %s\n", sf->s_Path);
}