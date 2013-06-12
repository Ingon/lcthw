#include <stdio.h>
#include <string.h>
#include "dbg.h"
#include <sys/time.h>

int normal_copy(char *from, char *to, int count)
{
    int i = 0;

    for(i = 0; i < count; i++) {
        to[i] = from[i];
    }

    return i;
}

int duffs_device(char *from, char *to, int count)
{
    {
        int n = (count + 7) / 8;

        switch(count % 8) {
            case 0: do { *to++ = *from++;
                        case 7: *to++ = *from++;
                        case 6: *to++ = *from++;
                        case 5: *to++ = *from++;
                        case 4: *to++ = *from++;
                        case 3: *to++ = *from++;
                        case 2: *to++ = *from++;
                        case 1: *to++ = *from++;
                    } while(--n > 0);
        }
    }

    return count;
}

int zeds_device(char *from, char *to, int count)
{
    {
        int n = (count + 7) / 8;

        switch(count % 8) {
            case 0:
            again: *to++ = *from++;

            case 7: *to++ = *from++;
            case 6: *to++ = *from++;
            case 5: *to++ = *from++;
            case 4: *to++ = *from++;
            case 3: *to++ = *from++;
            case 2: *to++ = *from++;
            case 1: *to++ = *from++;
                if(--n > 0) goto again;
        }
    }

    return count;
}

int valid_copy(char *data, int count, char expects)
{
    int i = 0;
    for(i = 0; i < count; i++) {
        if(data[i] != expects) {
            log_err("[%d] %c != %c", i, data[i], expects);
            return 0;
        }
    }

    return 1;
}

#define SIZE 1000000
int main(int argc, char *argv[])
{
    struct timeval tval;
    gettimeofday(&tval, NULL);
    printf("xxx: %ld %d\n", tval.tv_sec, tval.tv_usec);

    char from[SIZE] = {'a'};
    char to[SIZE] = {'c'};
    int rc = 0;
    suseconds_t now = 0;

    memset(from, 'x', SIZE);

    memset(to, 'y', SIZE);
    check(valid_copy(to, SIZE, 'y'), "Not initialized right");

    gettimeofday(&tval, NULL);
    printf("1: %ld %d\n", tval.tv_sec, tval.tv_usec);
    now = tval.tv_usec;

    rc = normal_copy(from, to, SIZE);
    check(rc == SIZE, "Normal copy failed: %d", rc);
    check(valid_copy(to, SIZE, 'x'), "Normal copy failed.");

    gettimeofday(&tval, NULL);
    printf("2: %ld %d\n", tval.tv_sec, tval.tv_usec);
    printf("DIFF: %d\n", (tval.tv_usec - now));

    // reset
    memset(to, 'y', SIZE);

    gettimeofday(&tval, NULL);
    printf("3: %ld %d\n", tval.tv_sec, tval.tv_usec);
    now = tval.tv_usec;

    rc = duffs_device(from, to, SIZE);
    check(rc == SIZE, "Duff's device failed: %d", rc);
    check(valid_copy(to, SIZE, 'x'), "Duff's device failed copy.");    

    gettimeofday(&tval, NULL);
    printf("4: %ld %d\n", tval.tv_sec, tval.tv_usec);
    printf("DIFF: %d\n", (tval.tv_usec - now));

    // reset
    memset(to, 'y', SIZE);

    gettimeofday(&tval, NULL);
    printf("5: %ld %d\n", tval.tv_sec, tval.tv_usec);
    now = tval.tv_usec;

    rc = zeds_device(from, to, SIZE);
    check(rc == SIZE, "Zed's device failed: %d", rc);
    check(valid_copy(to, SIZE, 'x'), "Zed's device failed copy.");

    gettimeofday(&tval, NULL);
    printf("6: %ld %d\n", tval.tv_sec, tval.tv_usec);
    printf("DIFF: %d\n", (tval.tv_usec - now));

    // reset
    memset(to, 'y', SIZE);

    gettimeofday(&tval, NULL);
    printf("7: %ld %d\n", tval.tv_sec, tval.tv_usec);
    now = tval.tv_usec;

    memcpy(to, from, SIZE);
    check(rc == SIZE, "Zed's device failed: %d", rc);
    check(valid_copy(to, SIZE, 'x'), "Zed's device failed copy.");

    gettimeofday(&tval, NULL);
    printf("8: %ld %d\n", tval.tv_sec, tval.tv_usec);
    printf("DIFF: %d\n", (tval.tv_usec - now));

    // reset
    memset(to, 'y', SIZE);

    gettimeofday(&tval, NULL);
    printf("9: %ld %d\n", tval.tv_sec, tval.tv_usec);
    now = tval.tv_usec;

    memmove(to, from, SIZE);
    check(rc == SIZE, "Zed's device failed: %d", rc);
    check(valid_copy(to, SIZE, 'x'), "Zed's device failed copy.");

    gettimeofday(&tval, NULL);
    printf("0: %ld %d\n", tval.tv_sec, tval.tv_usec);
    printf("DIFF: %d\n", (tval.tv_usec - now));

    // reset
    memset(to, 'y', SIZE);

    gettimeofday(&tval, NULL);
    printf("9: %ld %d\n", tval.tv_sec, tval.tv_usec);
    now = tval.tv_usec;

    memset(to, 'x', SIZE);
    check(rc == SIZE, "Zed's device failed: %d", rc);
    check(valid_copy(to, SIZE, 'x'), "Zed's device failed copy.");

    gettimeofday(&tval, NULL);
    printf("0: %ld %d\n", tval.tv_sec, tval.tv_usec);
    printf("DIFF: %d\n", (tval.tv_usec - now));

    return 0;

error:
    return 1;
}