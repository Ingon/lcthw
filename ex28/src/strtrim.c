#include "strtrim.h"
#include <ctype.h>

void trim_end(char *data, int len)
{
    for(int i = len - 1; len >= 0; i--) {
        if(isspace(data[i]) || data[i] == '\0') {
            data[i] = '\0';
        } else {
            break;
        }
    }
}
