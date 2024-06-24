#include <stdio.h>
#include <stdlib.h>

// const char FILENAME[] = "input.txt";
const char FILENAME[] = "test.txt";
const char INPUT_FORMAT[] = "Step %1c must be finished before step %1c can begin.\n";

typedef struct Requirement
{
    char value;
    struct Requirement *next;
    int count;
} requirement;

requirement *root;
int rootCount;

requirement *findRequirementWithValue(requirement *current, char value)
{
    if (current->value == value)
        return current;

    for (int i = 0; i < current->count; ++i)
        return findRequirementWithValue(current->next + i, value);
}

void initialize(char first, char second)
{
    if (root == NULL)
    {
        root = (requirement *)malloc(sizeof(requirement));
        *root = (requirement){.value = first, .next = NULL, .count = 1};
        requirement *tempPtr = (requirement *)malloc(sizeof(requirement));
        root->next = tempPtr;
        *(root->next) = (requirement){.value = second, .next = NULL, .count = 0};
        rootCount = 1;
    }
    else
    {
        int found = 0;
        for (int i = 0; i < rootCount; ++i)
        {
            requirement *reqWithValue = findRequirementWithValue(root + i, first);
            if (reqWithValue != NULL)
            {
                ++reqWithValue->count;
                if (reqWithValue->count == 1)
                {
                    requirement *nextPtr = (requirement *)malloc(sizeof(requirement));
                    if (nextPtr == NULL)
                    {
                        printf("Can't allocate memory for storing next nodes. Aborting . . .");
                        exit(1);
                    }
                    else
                        reqWithValue->next = nextPtr;
                }
                else
                {
                    requirement *tempRealloc = (requirement *)realloc(reqWithValue->next, reqWithValue->count * sizeof(requirement));
                    if (tempRealloc == NULL)
                    {
                        printf("Can't reallocate memory for storing %d subnodes. Aborting . . .", reqWithValue->count);
                        exit(1);
                    }
                    else
                        reqWithValue->next = tempRealloc;
                }

                *(reqWithValue->next + (reqWithValue->count) - 1) = (requirement){.value = second, .next = NULL, .count = 0};
                found = 1;
                break;
            }
        }

        if (found == 0)
        {
            ++rootCount;
            requirement *tempPtr = (requirement *)realloc(root, rootCount * sizeof(requirement));
            if (tempPtr == NULL)
            {
                printf("Can't reallocate memory for storing root %d nodes. Aborting . . .", rootCount);
                exit(1);
            }
            else
                root = tempPtr;

            *(root + rootCount - 1) = (requirement){.value = second, .next = NULL, .count = 0};
        }
    }
}

void initRootFromFile()
{
    FILE *filePtr = fopen(FILENAME, "r");
    if (filePtr == NULL)
    {
        printf("Can't open file \"%s\", abort . . .", FILENAME);
        exit(1);
    }

    int total = 0;
    char first, second;
    while (fscanf(filePtr, INPUT_FORMAT, &first, &second) == 2)
    {
        printf("%c -> %c\n", first, second);
        initialize(first, second);

        ++total;
    }

    printf("\nTotal: %d\n", total);
    fclose(filePtr);
}

void disposeNodeRecursively(requirement *node)
{
    if (node->next != NULL)
    {
        for (int i = 0; i < node->count; ++i)
        {
            disposeNodeRecursively(node->next + i);
            printf("Disposing: %c\n", (node->next + i)->value);
            free(node->next + i);
        }
    }
}

void disposeRequirements()
{
    for (int i = 0; i < rootCount; ++i)
        disposeNodeRecursively(root + i);
}

int main()
{
    initRootFromFile();

    disposeRequirements();
    return 0;
}