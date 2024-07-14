#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX_SIZE 64

const int ALLOCATED_LETTERS = 25;

// const char FILENAME[] = "test.txt";
const char FILENAME[] = "input.txt";
const char LINE_FORMAT[] = "%[^\n]\n";

typedef struct
{
    char letter;
    int count;
} LetterCount;

typedef struct
{
    const char *string;
} String;

int initializedLetters, initializedStrings = 0, allocatedStrings = 8;

LetterCount *letterCounter;
String *strings;

void InitializeLettersCounter()
{
    letterCounter = (LetterCount *)malloc(sizeof(LetterCount) * ALLOCATED_LETTERS);
    if (letterCounter == NULL)
    {
        printf("Can't allocated memory to store %d letters. Aborting . . .", ALLOCATED_LETTERS);
        exit(1);
    }
}

void InitializeStrings()
{
    strings = (String *)malloc(sizeof(String) * allocatedStrings);
    if (strings == NULL)
    {
        printf("Can't allocate memory to store %d strings. Aborting . . .");
        exit(1);
    }
}

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

    InitializeLettersCounter();
    InitializeStrings();

    int appearsTwice = 0, appearsTriple = 0;
    char bufferStr[LINE_MAX_SIZE];

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

        if (initializedStrings >= allocatedStrings)
        {
            allocatedStrings *= 2;
            String *reallocPtr = (String *)realloc(strings, sizeof(String) * allocatedStrings);
            if (reallocPtr == NULL)
            {
                printf("Can't realloc memory for storing %d strings. Aborting . . .", allocatedStrings);
                exit(1);
            }
            else
                strings = reallocPtr;
        }

        char copiedBuffer[64];
        strcpy(copiedBuffer, bufferStr);
        strings[initializedStrings++] = (String){.string = copiedBuffer};

        printf("\"%s\": %li\n", strings[initializedStrings - 1].string, strlen(bufferStr));
    }

    fclose(filePtr);
    free(letterCounter);

    printf("\nTotal: %d\n", initializedStrings);
    printf("\nPart 1. Checksum is %d * %d = %d\n", appearsTwice, appearsTriple, appearsTwice * appearsTriple);

    free(strings);
    return 0;
}