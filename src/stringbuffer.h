#define MAXLEN 80
typedef struct {
    char retval[MAXLEN];
    char* current;
    int free;
} stringbuffer;

void stringbuffer_init(stringbuffer* sb);
void stringbuffer_append(stringbuffer* sb, char* value);
