#include "functions.h"

#include <stdlib.h>  
#include <crtdbg.h>

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//  srand((unsigned int)time(NULL));
    return RunDB();
}