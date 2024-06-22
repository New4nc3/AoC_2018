#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// const char FILENAME[] = "input.txt";
const char FILENAME[] = "test.txt";

typedef struct Point
{
    int x;
    int y;
    long int area;
} point;

int count;
point* points;

void readPointsFromFile()
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
    count = 0;

    while (fscanf(file, "%3i, %3i", &first, &second) == 2)
    {
        if (count >= allocated)
        {
            allocated *= 2;
            point *temp = (point *)realloc(ptr, allocated * sizeof(point));

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

        (ptr + count)->x = first;
        (ptr + count)->y = second;
        ++count;
    }

    fclose(file);
    points = ptr;
}

void printItems()
{
    for (int i = 0; i < count; ++i)
        printf("%d = (%d %d)\n", (i + 1), (points + i)->x, (points + i)->y);

    printf("\nTotal: %d points\n", count);
}

int hasCorners(point *current)
{
    int upperLeft = 0, upperRight = 0, lowerLeft = 0, lowerRight = 0;

    for (int i = 0; i < count; ++i)
    {
        point iter = *(points + i);

        if (upperLeft != 1 && iter.x < current->x && iter.y < current->y)
            upperLeft = 1;
        if (upperRight != 1 && iter.x > current->x && iter.y < current->y)
            upperRight = 1;
        if (lowerLeft != 1 && iter.x < current->x && iter.y > current->y)
            lowerLeft = 1;
        if (lowerRight != 1 && iter.x > current->x && iter.y > current->y)
            lowerRight = 1;

        if (upperLeft == 1 && upperRight == 1 && lowerLeft == 1 && lowerRight == 1)
            return 1;
    }

    return 0;
}

int distance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

int closestPointIndexToCoords(int x, int y)
{
    int closestIndex = -1;
    int closestDistance = INT_MAX;

    for (int i = 0; i < count; ++i)
    {
        point currentPoint = *(points + i);
        int currentDistance = distance(x, y, currentPoint.x, currentPoint.y);

        if (currentDistance < closestDistance)
        {
            closestDistance = currentDistance;
            closestIndex = i;
        }
        else if (currentDistance == closestDistance)
        {
            closestIndex = -1;
        }
    }

    return closestIndex;
}

int calculateArea(point *current, int currentIndex)
{
    int area = 1;
    int offset = 1;
    int atLeastOneFound;

    do
    {
        atLeastOneFound = 0;

        for (int i = current->y - offset; i < current->y + offset; ++i)
            if (closestPointIndexToCoords(current->x - offset, i) == currentIndex)
            {
                ++area;
                atLeastOneFound = 1;
            }

        for (int i = current->x - offset; i < current->x + offset; ++i)
            if (closestPointIndexToCoords(i, current->y + offset) == currentIndex)
            {
                ++area;
                atLeastOneFound = 1;
            }

        for (int i = current->y + offset; i > current->y - offset; --i)
            if (closestPointIndexToCoords(current->x + offset, i) == currentIndex)
            {
                ++area;
                atLeastOneFound = 1;
            }

        for (int i = current->x + offset; i > current->x - offset; --i)
            if (closestPointIndexToCoords(i, current->y - offset) == currentIndex)
            {
                ++area;
                atLeastOneFound = 1;
            }

        ++offset;
    } while (atLeastOneFound == 1);

    return area;
}

void solvePart1()
{
    int maxArea = -1;

    for (int i = 0; i < count; ++i)
    {
        point *current = (points + i);

        if (hasCorners(current))
        {
            int currentArea = calculateArea(current, i);
            printf("Point %d has finite area: %d\n", i + 1, currentArea);

            if (currentArea > maxArea)
                maxArea = currentArea;
        }
        else
            current->area = -1;
    }

    if (maxArea != -1)
        printf("Part 1. Max finite area is %d\n", maxArea);
    else
        printf("Part 1. There is no finite areas there or something went wrong . . .\n");
}

int main()
{
    readPointsFromFile();

    printItems();
    solvePart1();

    free(points);
    return 0;
}
