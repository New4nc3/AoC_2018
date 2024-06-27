#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char FILENAME[] = "test.txt";
// const char FILENAME[] = "input.txt";
const char LINE_FORMAT[] = "%[^\n]\n";

int main()
{
    FILE *filePtr = fopen(FILENAME, "r");
    if (filePtr == NULL)
    {
        printf("Can't open file \"%s\". Aborting . . .", FILENAME);
        exit(1);
    }

    int total = 0;
    char bufferStr[64];

    while (fscanf(filePtr, LINE_FORMAT, bufferStr) == 1)
    {
        printf("\"%s\": %li\n", bufferStr, strlen(bufferStr));
        ++total;
    }

    fclose(filePtr);
    printf("\nTotal: %d\n", total);

    return 0;
}