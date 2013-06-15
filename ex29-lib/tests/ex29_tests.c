#include <stdio.h>
#include "dbg.h"
#include <dlfcn.h>

#define LIB_FILE "build/libex29.so"

#define FN1 "uppercase"
#define FN2 "lowercase"
#define TEST "aABbccDD"

typedef int (*lib_function)(const char *data, int length);

int test(void *lib, const char *fn, const char *data)
{
    lib_function func = dlsym(lib, fn);
    check(func != NULL, "Did not find %s function in the library %s: %s", fn, LIB_FILE, dlerror());

    int rc = func(data, strlen(data));
    check(rc == 0, "Function %s return %d for data: %s", fn, rc, data);

    return 1;
error:
    return 0;
}

int main(int argc, char  *argv[])
{
    int rc = 0;

    void *lib = dlopen(LIB_FILE, RTLD_NOW);
    check(lib != NULL, "Failed to open the library %s: %s", LIB_FILE, dlerror());

    check(test(lib, FN1, TEST), "Failed on %s", FN1);
    check(test(lib, FN2, TEST), "Failed on %s", FN2);

    rc = dlclose(lib);
    check(rc == 0, "Failed to close %s", LIB_FILE);

    return 0;
error:
    return 1;
}
