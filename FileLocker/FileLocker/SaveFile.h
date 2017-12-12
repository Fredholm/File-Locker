#ifndef SAVEFILE_H
#define SAVEFILE_H

#define MAX_SAVE_FILES  248
#define MAX_CHAR_FILE   24

struct SaveFile
{
    char s_Path[MAX_CHAR_FILE];             // The Path to the saved file
};

struct SaveData
{
    SaveFile**  s_Saved;                    // The data for each savefile
    int         s_NumberOfSavedFiles;       // The number of saved data files
};

#endif // !SAVEFILE_H