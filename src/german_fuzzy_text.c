#include <stdio.h>

#define MAXLEN 80
static char retval[MAXLEN];

static void addstring(char* dest, char* src, int* used, int* free) {
    (*used) += snprintf(dest + (*used), (*free), "%s", src);
    (*free) = MAXLEN - 1 - (*used);
}

char* german_fuzzy_text(int hour, int minute) {
    int used = 0;
    int free = MAXLEN - 1 - used;
    retval[used] = '\0';

    minute -= 8;
    if (minute < 0) {
        minute += 60;
        hour -= 1;
    }
    int section = minute / 5;
    if (section % 3 == 0)
        addstring(retval, "fünf vor\n", &used, &free);
    if (section % 3 == 2)
        addstring(retval, "fünf nach\n", &used, &free);
    section = section / 3;
    if (section % 4 == 0)
        addstring(retval, "viertel\n", &used, &free);
    if (section % 4 == 1)
        addstring(retval, "halb\n", &used, &free);
    if (section % 4 == 2)
        addstring(retval, "dreiviertel\n", &used, &free);
    switch (hour+1) {
        case 0:  addstring(retval, "zwölf", &used, &free); break;
        case 1:  addstring(retval, "eins", &used, &free); break;
        case 2:  addstring(retval, "zwei", &used, &free); break;
        case 3:  addstring(retval, "drei", &used, &free); break;
        case 4:  addstring(retval, "vier", &used, &free); break;
        case 5:  addstring(retval, "fünf", &used, &free); break;
        case 6:  addstring(retval, "sechs", &used, &free); break;
        case 7:  addstring(retval, "sieben", &used, &free); break;
        case 8:  addstring(retval, "acht", &used, &free); break;
        case 9:  addstring(retval, "neun", &used, &free); break;
        case 10: addstring(retval, "zehn", &used, &free); break;
        case 11: addstring(retval, "elf", &used, &free); break;
        case 12: addstring(retval, "zwölf", &used, &free); break;
    }
    addstring(retval, "\n", &used, &free);

    //printf("used: %d, free: %d, section: %d\n", used, free, section);
    return retval;
}

//int main() {
//    int h;
//    for (h = 0; h < 12; h++) {
//        int m;
//        for (m = 0; m < 60; m++) {
//            printf("%d:%d -> \n%s\n", h, m, german_fuzzy_text(h, m));
//        }
//    }
//}
