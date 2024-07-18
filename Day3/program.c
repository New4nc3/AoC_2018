#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 1000

const char EMPTY = '.';
const char WALL = '#';
const char OVERLAP = 'X';

// const char FILENAME[] = "test.txt";
const char FILENAME[] = "input.txt";
const char FORMAT[] = "#%d @ %d,%d: %dx%d\n";

typedef struct
{
    int number;
    int row;
    int column;
    int width;
    int height;
} Instruction;

int allocatedInstructions = 8, initializedCount = 0;
char grid[GRID_SIZE][GRID_SIZE];
Instruction *instructions;

void clearGrid()
{
    for (int i = 0; i < GRID_SIZE; ++i)
        for (int j = 0; j < GRID_SIZE; ++j)
            grid[i][j] = EMPTY;
}

void initGrid()
{
    FILE *filePtr = fopen(FILENAME, "r");
    if (filePtr == NULL)
    {
        printf("Can't open file \"%s\"! Check if it exists . . .", FILENAME);
        exit(1);
    }

    instructions = (Instruction *)malloc(sizeof(Instruction) * allocatedInstructions);
    if (instructions == NULL)
    {
        printf("Can't allocate memory to store %d instructions. Abort . . .", allocatedInstructions);
        exit(1);
    }

    int number, column, row, width, height;
    while (fscanf(filePtr, FORMAT, &number, &column, &row, &width, &height) == 5)
    {
        int rowDest = row + height;
        int columnDest = column + width;

        for (int i = row; i < rowDest; ++i)
            for (int j = column; j < columnDest; ++j)
            {
                char current = grid[i][j];

                if (current == EMPTY)
                    grid[i][j] = WALL;
                else if (current == WALL)
                    grid[i][j] = OVERLAP;
            }

        if (initializedCount >= allocatedInstructions)
        {
            allocatedInstructions *= 2;
            Instruction *tempPtr = (Instruction *)realloc(instructions, sizeof(Instruction) * allocatedInstructions);
            if (tempPtr == NULL)
            {
                printf("Can't reallocate memory to store %d instructions. Abort . . .", allocatedInstructions);
                exit(1);
            }
            else
                instructions = tempPtr;
        }

        instructions[initializedCount++] = (Instruction){.number = number, .row = row, .column = column, .width = width, .height = height};
    }
}

void solvePart1()
{
    int totalOverlapped = 0;
    for (int i = 0; i < GRID_SIZE; ++i)
        for (int j = 0; j < GRID_SIZE; ++j)
            if (grid[i][j] == OVERLAP)
                ++totalOverlapped;

    printf("Part 1. Total overlapped: %d square inches\n", totalOverlapped);
}

void solvePart2()
{
    int foundNumber = -1;
    for (int i = 0; i < initializedCount; ++i)
    {
        Instruction ins = instructions[i];
        int rowDest = ins.row + ins.height;
        int columnDest = ins.column + ins.width;
        int skipInstruction = 0;

        for (int j = ins.row; j < rowDest; ++j)
        {
            for (int k = ins.column; k < columnDest; ++k)
                if (grid[j][k] == OVERLAP)
                {
                    skipInstruction = 1;
                    break;
                }

            if (skipInstruction)
                break;
        }

        if (!skipInstruction)
        {
            foundNumber = ins.number;
            break;
        }
    }

    if (foundNumber != -1)
        printf("Part 2. First non-overlapped instruction: #%d\n", foundNumber);
    else
        printf("Part 2. There is no non-overlapped instruction or something went wrong");
}

int main()
{
    clearGrid();
    initGrid();
    solvePart1();
    solvePart2();

    return 0;
}