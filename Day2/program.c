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

int initializedLetters, initializedStrings = 0, allocatedStrings = 8;

LetterCount *letterCounter;
char **strings;

void InitializeLettersCounter()
{
    letterCounter = (LetterCount *)malloc(sizeof(LetterCount) * ALLOCATED_LETTERS);
    if (letterCounter == NULL)
    {
        printf("Can't allocate memory to store %d letters. Aborting . . .", ALLOCATED_LETTERS);
        exit(1);
    }
}

void InitializeStrings()
{
    strings = (char **)malloc(sizeof(char *) * allocatedStrings);
    if (strings == NULL)
    {
        printf("Can't allocate memory to store %d strings. Aborting . . .");
        exit(1);
    }
}

void InitializeString(int index)
{
    char *stringPtr = (char *)malloc(sizeof(char) * LINE_MAX_SIZE);
    if (stringPtr == NULL)
    {
        printf("Can't allocate memory to store %d chars. Aborting . . .", LINE_MAX_SIZE);
        exit(1);
    }
    else
        strings[index] = stringPtr;
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
            char **reallocPtr = (char **)realloc(strings, sizeof(char *) * allocatedStrings);
            if (reallocPtr == NULL)
            {
                printf("Can't realloc memory for storing %d strings. Aborting . . .", allocatedStrings);
                exit(1);
            }
            else
                strings = reallocPtr;
        }

        InitializeString(initializedStrings);
        strcpy(strings[initializedStrings], bufferStr);
        ++initializedStrings;

        printf("\"%s\": %li\n", strings[initializedStrings - 1], strlen(bufferStr));
    }

    fclose(filePtr);
    free(letterCounter);

    int foundSimilar = 0;
    char similarResult[LINE_MAX_SIZE];

    for (int i = 0; i < initializedStrings - 1; ++i)
    {
        char *string1 = strings[i];
        int length = strlen(string1);
        int diffIndex = -1;

        for (int j = i + 1; j < initializedStrings; ++j)
        {
            char *string2 = strings[j];

            if (length != strlen(string2))
                break;

            int differences = 0;
            for (int k = 0; k < length; ++k)
                if (string1[k] != string2[k])
                {
                    diffIndex = k;
                    ++differences;

                    if (differences > 1)
                        break;
                }

            if (differences == 1)
            {
                foundSimilar = 1;
                break;
            }
        }

        if (foundSimilar)
        {
            int tmp = 0;
            for (int k = 0; k < length; ++k)
                if (k != diffIndex)
                    similarResult[tmp++] = string1[k];

            similarResult[length - 1] = '\0';
            break;
        }
    }

    free(strings);

    printf("\nTotal: %d\n", initializedStrings);
    printf("\nPart 1. Checksum is %d * %d = %d\n", appearsTwice, appearsTriple, appearsTwice * appearsTriple);

    if (foundSimilar)
        printf("Part 2. Similar strings with exactly one diff (without diff char): \"%s\"\n", similarResult);
    else
        printf("Part 2. Similar strings are not found\n");

    return 0;
}