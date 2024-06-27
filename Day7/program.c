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

requirement *mallocRequirement(int count)
{
    requirement *ptr = (requirement *)malloc(count * sizeof(requirement));

    if (ptr == NULL)
    {
        printf("Can't allocate memory. Aborting . . .");
        exit(1);
    }

    return ptr;
}

requirement *reallocRequirement(requirement *original, int count)
{
    requirement *ptr = (requirement *)realloc(original, count * sizeof(requirement));

    if (ptr == NULL)
    {
        printf("Can't reallocate memory for storing %d nodes. Aborting . . .", count);
        exit(1);
    }

    return ptr;
}

void initialize(char first, char second)
{
    if (root == NULL)
    {
        rootCount = 1;
        root = mallocRequirement(rootCount);
        *root = (requirement){.value = first, .next = NULL, .count = 1};
        root->next = mallocRequirement(1);
        *(root->next) = (requirement){.value = second, .next = NULL, .count = 0};
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
                    reqWithValue->next = mallocRequirement(1);
                else
                    reqWithValue->next = reallocRequirement(reqWithValue->next, reqWithValue->count);

                *(reqWithValue->next + (reqWithValue->count) - 1) = (requirement){.value = second, .next = NULL, .count = 0};
                found = 1;
                break;
            }
        }

        if (found == 0)
        {
            ++rootCount;
            root = reallocRequirement(root, rootCount);
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
    if (node->next == NULL)
    {
        printf("Disposing: %c\n", node->value);
        free(node);
        return;
    }

    for (int i = 0; i < node->count; ++i)
        disposeNodeRecursively(node->next + i);
}

void disposeRequirements()
{
    for (int i = 0; i < rootCount; ++i)
    {
        disposeNodeRecursively(root + i);
        free(root + i);
    }
}

void printRecursively(requirement *node)
{
    if (node->next == NULL)
    {
        printf("%c <- ", node->value);
        return;
    }

    for (int i = 0; i < node->count; ++i)
        printRecursively(node->next + i);
}

int main()
{
    initRootFromFile();

    // for (int i = 0; i < rootCount; ++i)
    //     printRecursively(root + i);

    disposeRequirements();
    printf("DONT CRASH");
    return 0;
}