#include <stdio.h>
#include <stdlib.h>

const char FILENAME[] = "input.txt";
// const char FILENAME[] = "test.txt";
const char LINE_FORMAT[] = "%d";

int *hashSetPtr;
int count = 0, allocated = 8;

int addToHashSet(int item)
{
    for (int i = 0; i < count; ++i)
        if (hashSetPtr[i] == item)
            return 0;

    if (count >= allocated)
    {
        allocated *= 2;
        int *tempPtr = (int *)realloc(hashSetPtr, allocated * sizeof(int));
        if (tempPtr == NULL)
        {
            printf("Can't allocate memory for storing %d ints. Aborting program . . .", allocated);
            exit(1);
        }

        hashSetPtr = tempPtr;
    }

    hashSetPtr[count] = item;
    ++count;

    return 1;
}

int main()
{
    int sum = 0, index = 0, savedPart1 = 0, foundPart2 = 0, sumPart1, current, firstReachedTwice;
    FILE *filePtr = fopen(FILENAME, "r");
    hashSetPtr = (int *)malloc(allocated * sizeof(int));
    addToHashSet(0);

    do
    {
        while (fscanf(filePtr, LINE_FORMAT, &current) == 1)
        {
            sum += current;

            if (!foundPart2 && !addToHashSet(sum))
            {
                firstReachedTwice = sum;
                foundPart2 = 1;
                break;
            }
        }

        if (!savedPart1)
        {
            sumPart1 = sum;
            savedPart1 = 1;
        }

        rewind(filePtr);
    } while (!foundPart2);

    fclose(filePtr);
    free(hashSetPtr);

    printf("Part 1. Result sum of signals is %d\n", sumPart1);

    if (foundPart2)
        printf("Part 2. First frequency reached twice is %d\n", firstReachedTwice);
    else
        printf("Part 2. Nothing were found, something is wrong . . .");

    return 0;
}