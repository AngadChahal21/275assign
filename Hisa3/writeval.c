#include "writeval.h"
#include <stdio.h>
#include <string.h>

// type tags stored as the first byte before each value
#define TAG_INT  0
#define TAG_CHAR 1
#define TAG_PTR  2
#define TAG_STR  3
#define TAG_END  0xFF

// round address up to the next multiple of size (same idea as q3's nextAlignedAddress)
static void *alignUp(void *unalignedPtr, size_t alignment) {
    size_t rawAddress = (size_t)unalignedPtr;
    return (void *)(((rawAddress + alignment - 1) / alignment) * alignment);
}

void *writeValue(void *writeLoc, const char *type, void *payload) {
    char *writeCursor = (char *)writeLoc;

    if (strcmp(type, "int") == 0) {
        *writeCursor = TAG_INT;
        int *intWritePtr = (int *)alignUp(writeCursor + 1, sizeof(int));
        *intWritePtr = *(int *)payload;
        return (void *)(intWritePtr + 1);

    } else if (strcmp(type, "char") == 0) {
        *writeCursor = TAG_CHAR;
        // char is size 1, always aligned
        char *charWritePtr = (char *)(writeCursor + 1);
        *charWritePtr = *(char *)payload;
        return (void *)(charWritePtr + 1);

    } else if (strcmp(type, "ptr") == 0) {
        *writeCursor = TAG_PTR;
        void **pointerWritePtr = (void **)alignUp(writeCursor + 1, sizeof(void *));
        *pointerWritePtr = *(void **)payload;
        return (void *)(pointerWritePtr + 1);

    } else if (strcmp(type, "str") == 0) {
        *writeCursor = TAG_STR;
        // strings are just bytes, no alignment needed
        char *stringWritePtr = writeCursor + 1;
        const char *sourceText = (const char *)payload;
        while (*sourceText) *stringWritePtr++ = *sourceText++;
        *stringWritePtr++ = '\0';
        return (void *)stringWritePtr;
    }

    // unknown type — shouldn't happen, just return unchanged
    return writeLoc;
}

void printValues(void *dataStart) {
    char *readCursor = (char *)dataStart;
    int entryIndex = 0;

    while (1) {
        unsigned char tag = (unsigned char)*readCursor;

        if (tag == TAG_END) break;

        if (tag == TAG_INT) {
            int *intValuePtr = (int *)alignUp(readCursor + 1, sizeof(int));
            printf("Value %d at %p: %d\n", entryIndex, (void *)readCursor, *intValuePtr);
            readCursor = (char *)(intValuePtr + 1);

        } else if (tag == TAG_CHAR) {
            char *charValuePtr = (char *)(readCursor + 1);
            printf("Value %d at %p: '%c'\n", entryIndex, (void *)readCursor, *charValuePtr);
            readCursor = charValuePtr + 1;

        } else if (tag == TAG_PTR) {
            void **pointerValuePtr = (void **)alignUp(readCursor + 1, sizeof(void *));
            printf("Value %d at %p: %p\n", entryIndex, (void *)readCursor, *pointerValuePtr);
            readCursor = (char *)(pointerValuePtr + 1);

        } else if (tag == TAG_STR) {
            char *stringValuePtr = readCursor + 1;
            printf("Value %d at %p: \"%s\"\n", entryIndex, (void *)readCursor, stringValuePtr);
            readCursor = stringValuePtr + strlen(stringValuePtr) + 1;

        } else {
            // hit something unexpected — stop
            break;
        }
        entryIndex++;
    }
}
