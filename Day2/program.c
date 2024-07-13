#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int ALLOCATED_LETTERS = 25;
// const char FILENAME[] = "test.txt";
const char FILENAME[] = "input.txt";
const char LINE_FORMAT[] = "%[^\n]\n";

typedef struct
{
    char letter;
    int count;
} LetterCount;

int initializedLetters;
LetterCount *letterCounter;

int IndexOf(char letter)
{
    for (int i = 0; i < initializedLetters; ++i)
        if (letterCounter[i].letter == letter)
            return i;

    return -1;
}

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

    letterCounter = (LetterCount *)malloc(sizeof(LetterCount) * ALLOCATED_LETTERS);
    if (letterCounter == NULL)
    {
        printf("Can't allocated memory to store %d letters. Aborting . . .", ALLOCATED_LETTERS);
        exit(1);
    }

    int appearsTwice = 0, appearsTriple = 0;

    while (fscanf(filePtr, LINE_FORMAT, bufferStr) == 1)
    {
        int length = strlen(bufferStr);
        initializedLetters = 0;

        for (int i = 0; i < length; ++i)
        {
            char letter = bufferStr[i];
            int index = IndexOf(letter);

            if (index != -1)
                ++letterCounter[index].count;
            else
                letterCounter[initializedLetters++] = (LetterCount){.letter = letter, .count = 1};
        }

        int appearedTwice = 0, appearedTriple = 0;
        for (int i = 0; i < initializedLetters; ++i)
        {
            int count = letterCounter[i].count;

            if (count == 2 && !appearedTwice)
            {
                ++appearsTwice;
                appearedTwice = 1;
            }
            else if (count == 3 && !appearedTriple)
            {
                ++appearsTriple;
                appearedTriple = 1;
            }

            if (appearedTwice && appearedTriple)
                break;
        }

        printf("\"%s\": %li\n", bufferStr, strlen(bufferStr));
        ++total;
    }

    fclose(filePtr);
    free(letterCounter);

    printf("\nTotal: %d\n", total);
    printf("\nPart 1. Checksum is %d * %d = %d", appearsTwice, appearsTriple, appearsTwice * appearsTriple);

    return 0;
}