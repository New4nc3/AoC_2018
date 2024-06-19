#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//const char FILENAME[] = "input.txt";
const char FILENAME[] = "test.txt";

typedef struct Point
{
    int x;
    int y;
} point;

point *readFromFile(int *count)
{
    int allocated = 8;
    point *ptr = (point *)malloc(allocated * sizeof(point));
    FILE *file = fopen(FILENAME, "r");

    if (file == NULL)
    {
        printf("File \"%s\" can't be opened, abort . . .", FILENAME);
        exit(1);
    }

    int first, second;
    *count = 0;

    while (fscanf(file, "%3i, %3i", &first, &second) == 2)
    {
        if (*count >= allocated)
        {
            allocated *= 2;
            point* temp = (point *)realloc(ptr, allocated * sizeof(point));

            if (temp)
            {
                ptr = temp;
            }
            else
            {
                printf("Can't allocate memory for %d items, abort . . .", allocated);
                exit(1);
            }
        }

        (ptr + *count)->x = first;
        (ptr + *count)->y = second;
        *count = *count + 1;
    }

    fclose(file);
    return ptr;
}

void printItems(point* points, int count)
{
    for (int i = 0; i < count; ++i)
        printf("%d = (%d %d)\n", (i + 1), (points + i)->x, (points + i)->y);
    
    printf("\nTotal: %d points\n", count);
}

int main()
{
    int count;
    point *points = readFromFile(&count);

    printItems(points, count);

    free(points);
    return 0;
}
