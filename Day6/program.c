#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char FILENAME[] = "input.txt";
const int TOTAL_DISTANCE_LIMIT = 10000;
// const char FILENAME[] = "test.txt";
// const int TOTAL_DISTANCE_LIMIT = 32;

typedef struct Point
{
    int x;
    int y;
    long int area;
} point;

int count;
point *points;
point minCoords, maxCoords;

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

void initializeEdgeCoords()
{
    minCoords = *points;
    maxCoords = *points;

    for (int i = 0; i < count; ++i)
    {
        point current = *(points + i);

        if (current.x < minCoords.x)
            minCoords.x = current.x;
        else if (current.x > maxCoords.x)
            maxCoords.x = current.x;

        if (current.y < minCoords.y)
            minCoords.y = current.y;
        else if (current.y > maxCoords.y)
            maxCoords.y = current.y;
    }
}

int distanceBetween(int x1, int y1, point point)
{
    return abs(x1 - point.x) + abs(y1 - point.y);
}

int closestPointIndexToCoords(int x, int y)
{
    int closestIndex = -1;
    int closestDistance = INT_MAX;

    for (int i = 0; i < count; ++i)
    {
        point currentPoint = *(points + i);
        int currentDistance = distanceBetween(x, y, currentPoint);

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

int hasBorders(point *current, int currentIndex)
{
    int leftBordered = 0, upBordered = 0, rightBordered = 0, bottomBordered = 0;

    for (int i = current->x - 1; i >= minCoords.x; --i)
        if (closestPointIndexToCoords(i, current->y) != currentIndex)
        {
            leftBordered = 1;
            break;
        }

    if (leftBordered == 0)
        return 0;

    for (int i = current->y - 1; i >= minCoords.y; --i)
        if (closestPointIndexToCoords(current->x, i) != currentIndex)
        {
            upBordered = 1;
            break;
        }

    if (upBordered == 0)
        return 0;

    for (int i = current->x + 1; i <= maxCoords.x; ++i)
        if (closestPointIndexToCoords(i, current->y) != currentIndex)
        {
            rightBordered = 1;
            break;
        }

    if (rightBordered == 0)
        return 0;

    for (int i = current->y + 1; i <= maxCoords.y; ++i)
        if (closestPointIndexToCoords(current->x, i) != currentIndex)
        {
            bottomBordered = 1;
            break;
        }

    return bottomBordered;
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
    } while (atLeastOneFound);

    return area;
}

int isSafeLocation(int x, int y)
{
    int sumOfDistances = 0;

    for (int i = 0; i < count; ++i)
    {
        sumOfDistances += distanceBetween(x, y, *(points + i));

        if (sumOfDistances >= TOTAL_DISTANCE_LIMIT)
            return 0;
    }

    return 1;
}

void solvePart1()
{
    int maxArea = -1;

    for (int i = 0; i < count; ++i)
    {
        point *current = (points + i);

        if (hasCorners(current) && hasBorders(current, i))
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
        printf("\nPart 1. Max finite area is %d\n", maxArea);
    else
        printf("\nPart 1. There is no finite area there or something went wrong . . .\n");
}

void solvePart2()
{
    int safeRegionArea = 0;

    for (int i = minCoords.x; i <= maxCoords.x; ++i)
        for (int j = minCoords.y; j <= maxCoords.y; ++j)
            if (isSafeLocation(i, j))
                ++safeRegionArea;

    if (safeRegionArea > 0)
        printf("Part 2. Safe region area is %d\n", safeRegionArea);
    else
        printf("Part 2. There is no safe region there or something went wrong . . .\n");
}

int main()
{
    readPointsFromFile();
    initializeEdgeCoords();

    solvePart1();
    solvePart2();

    free(points);
    return 0;
}
