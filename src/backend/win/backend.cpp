#include <debugapi.h>
#include "../os.h"

void *prime_message(char *message, const int message_length)
{
    OutputDebugStringA(message);
}