#ifdef __cplusplus
extern "C" {
#endif

#include "Database.h"

int main()
{
    srand(time_t(NULL));
    Database DB;
    return DB.Run();
}

#ifdef __cplusplus
}
#endif