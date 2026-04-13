#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Maze {
    char **grid;
    int rows;
    int cols;
    int startRow;
    int startCol;
    int playerRow;
    int playerCol;
};

static int isWalkable(char tileChar) {
    return tileChar != 'X';
}

static int isTeleporter(char tileChar) {
    return tileChar >= '0' && tileChar <= '9';
}

static int isIcy(char tileChar) {
    return tileChar == 'I';
}

static int isValidTile(char tileChar) {
    return tileChar == 'S' || tileChar == 'G' || tileChar == 'O' || tileChar == 'X' ||
           tileChar == 'I' || (tileChar >= '0' && tileChar <= '9');
}

struct Maze *readMaze() {
    int rowBufferCapacity = 8;
    char **gridRows = malloc(rowBufferCapacity * sizeof(char *));
    if (!gridRows) return NULL;

    int rowCount = 0;
    int columnCount = -1;
    char lineBuffer[4096];

    while (1) {
        if (fgets(lineBuffer, sizeof(lineBuffer), stdin) == NULL) break;

        // strip trailing newline
        int lineLength = (int)strlen(lineBuffer);
        while (lineLength > 0 &&
               (lineBuffer[lineLength - 1] == '\n' || lineBuffer[lineLength - 1] == '\r'))
            lineBuffer[--lineLength] = '\0';

        // empty line = end of maze
        if (lineLength == 0) break;

        for (int charIndex = 0; charIndex < lineLength; charIndex++) {
            if (!isValidTile(lineBuffer[charIndex])) {
                fprintf(stderr, "Invalid character found when reading maze: %c - this is invalid input\n", lineBuffer[charIndex]);
            }
        }

        // all rows must be the same width
        if (columnCount == -1) columnCount = lineLength;
        if (lineLength != columnCount) {
            for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) free(gridRows[rowIndex]);
            free(gridRows);
            return NULL;
        }

        if (rowCount == rowBufferCapacity) {
            rowBufferCapacity *= 2;
            char **resizedGridRows = realloc(gridRows, rowBufferCapacity * sizeof(char *));
            if (!resizedGridRows) {
                for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) free(gridRows[rowIndex]);
                free(gridRows);
                return NULL;
            }
            gridRows = resizedGridRows;
        }

        gridRows[rowCount] = malloc((lineLength + 1) * sizeof(char));
        if (!gridRows[rowCount]) {
            for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) free(gridRows[rowIndex]);
            free(gridRows);
            return NULL;
        }
        strcpy(gridRows[rowCount], lineBuffer);
        rowCount++;
    }

    if (rowCount == 0 || columnCount <= 0) {
        free(gridRows);
        return NULL;
    }

    // need exactly one S and at least one G
    int startTileCount = 0, goalTileCount = 0;
    int spawnRow = -1, spawnCol = -1;
    for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
        for (int columnIndex = 0; columnIndex < columnCount; columnIndex++) {
            if (gridRows[rowIndex][columnIndex] == 'S') {
                startTileCount++;
                spawnRow = rowIndex;
                spawnCol = columnIndex;
            } else if (gridRows[rowIndex][columnIndex] == 'G') {
                goalTileCount++;
            }
        }
    }

    if (startTileCount != 1 || goalTileCount < 1) {
        for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) free(gridRows[rowIndex]);
        free(gridRows);
        return NULL;
    }

    struct Maze *maze = malloc(sizeof(struct Maze));
    if (!maze) {
        for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) free(gridRows[rowIndex]);
        free(gridRows);
        return NULL;
    }

    maze->grid      = gridRows;
    maze->rows      = rowCount;
    maze->cols      = columnCount;
    maze->startRow  = spawnRow;
    maze->startCol  = spawnCol;
    maze->playerRow = spawnRow;
    maze->playerCol = spawnCol;

    return maze;
}

struct Pos makeMove(struct Maze *m, char dir) {
    int deltaRow = 0, deltaCol = 0;
    if      (dir == 'n') deltaRow = -1;
    else if (dir == 's') deltaRow =  1;
    else if (dir == 'e') deltaCol =  1;
    else if (dir == 'w') deltaCol = -1;

    int currentRow = m->playerRow;
    int currentCol = m->playerCol;
    int candidateRow = currentRow + deltaRow;
    int candidateCol = currentCol + deltaCol;

    // out of bounds or wall — stay put
    if (candidateRow < 0 || candidateRow >= m->rows || candidateCol < 0 || candidateCol >= m->cols ||
        !isWalkable(m->grid[candidateRow][candidateCol])) {
        struct Pos unchangedPosition = {m->playerCol, m->playerRow};
        return unchangedPosition;
    }

    currentRow = candidateRow;
    currentCol = candidateCol;

    // teleporter: jump to the matching tile
    if (isTeleporter(m->grid[currentRow][currentCol])) {
        char teleporterToken = m->grid[currentRow][currentCol];
        for (int searchRow = 0; searchRow < m->rows; searchRow++) {
            for (int searchCol = 0; searchCol < m->cols; searchCol++) {
                if (m->grid[searchRow][searchCol] == teleporterToken &&
                    !(searchRow == currentRow && searchCol == currentCol)) {
                    currentRow = searchRow;
                    currentCol = searchCol;
                    goto doneTP;
                }
            }
        }
        doneTP:;
    }

    // icy tile: keep sliding until we'd hit a wall or go out of bounds
    while (isIcy(m->grid[currentRow][currentCol])) {
        int slideTargetRow = currentRow + deltaRow;
        int slideTargetCol = currentCol + deltaCol;

        if (slideTargetRow < 0 || slideTargetRow >= m->rows || slideTargetCol < 0 || slideTargetCol >= m->cols)
            break;
        if (!isWalkable(m->grid[slideTargetRow][slideTargetCol]))
            break;

        currentRow = slideTargetRow;
        currentCol = slideTargetCol;

        if (isTeleporter(m->grid[currentRow][currentCol])) {
            char teleporterToken = m->grid[currentRow][currentCol];
            for (int searchRow = 0; searchRow < m->rows; searchRow++) {
                for (int searchCol = 0; searchCol < m->cols; searchCol++) {
                    if (m->grid[searchRow][searchCol] == teleporterToken &&
                        !(searchRow == currentRow && searchCol == currentCol)) {
                        currentRow = searchRow;
                        currentCol = searchCol;
                        goto doneTPSlide;
                    }
                }
            }
            doneTPSlide:;
            break;
        }
    }

    m->playerRow = currentRow;
    m->playerCol = currentCol;

    if (m->grid[currentRow][currentCol] == 'G') {
        struct Pos goalReachedMarker = {-1, -1};
        return goalReachedMarker;
    }

    struct Pos updatedPosition = {m->playerCol, m->playerRow};
    return updatedPosition;
}

void reset(struct Maze *m) {
    m->playerRow = m->startRow;
    m->playerCol = m->startCol;
}

void printMaze(struct Maze *m) {
    for (int borderColumn = 0; borderColumn < m->cols + 2; borderColumn++) putchar('=');
    putchar('\n');

    for (int rowIndex = 0; rowIndex < m->rows; rowIndex++) {
        putchar('|');
        for (int columnIndex = 0; columnIndex < m->cols; columnIndex++) {
            if (rowIndex == m->playerRow && columnIndex == m->playerCol)
                putchar('P');
            else
                putchar(m->grid[rowIndex][columnIndex]);
        }
        putchar('|');
        putchar('\n');
    }

    for (int borderColumn = 0; borderColumn < m->cols + 2; borderColumn++) putchar('=');
    putchar('\n');
}

struct Maze *destroyMaze(struct Maze *m) {
    if (!m) return NULL;
    for (int rowIndex = 0; rowIndex < m->rows; rowIndex++) free(m->grid[rowIndex]);
    free(m->grid);
    free(m);
    return NULL;
}
