#include "data.h"

#ifdef WIN32
/* windows specific */
#define SHARED __attribute__((section(".shr"), shared))
int kurk SHARED = 10;
char a[32] SHARED = "hello world";
#else
/* other platforms */
#endif

extern "C" __declspec(dllexport) int addNumbers(int no1, int no2) {
    kurk++;
    return kurk;
}

extern "C" __declspec(dllexport) char* getString() {
    return a;
}

extern "C" __declspec(dllexport) void setString(const char str[]) {
    strcpy(a, str);
}
