#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME 50
#define MAX_NAME_LEN 100
#define MAX_BLOCKS_PER_FILE 20
#define MAX_BLOCKS 100

typedef struct FreeBlock {
    int index;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

typedef enum { FILE_NODE, DIR_NODE } NodeType;

typedef struct FileNode {
    char name[MAX_NAME];
    NodeType type;
    struct FileNode *next;
    struct FileNode *prev;
    struct FileNode *child; 
    struct FileNode *parent;
    int blockPointers[MAX_BLOCKS_PER_FILE];
    int blockCount;
} FileNode;

unsigned char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock *freeListHead = NULL;
FileNode *root = NULL;
FileNode *cwd = NULL;
int usedBlocks = 0;



void initFreeList() {
    for (int index = 0; index < NUM_BLOCKS; index++) {
        FreeBlock *node = (FreeBlock *)malloc(sizeof(FreeBlock));
        node->index = index;
        if (!freeListHead) {
            freeListHead = node;
            node->next = node->prev = node;
        } else {
            FreeBlock *tail = freeListHead->prev;
            tail->next = node;
            node->prev = tail;
            node->next = freeListHead;
            freeListHead->prev = node;
        }
    }
}

FreeBlock *allocateBlock() {
    if (!freeListHead)
        return NULL;

    FreeBlock *block = freeListHead;

    if (block->next == block) {
        freeListHead = NULL;
    } else {
        block->prev->next = block->next;
        block->next->prev = block->prev;
        freeListHead = block->next;
    }

    usedBlocks++;
    return block;
}

void freeBlock(int index) {
    FreeBlock *node = (FreeBlock *)malloc(sizeof(FreeBlock));
    node->index = index;
    if (!freeListHead) {
        freeListHead = node;
        node->next = node->prev = node;
    } else {
        FreeBlock *tail = freeListHead->prev;
        tail->next = node;
        node->prev = tail;
        node->next = freeListHead;
        freeListHead->prev = node;
    }
    usedBlocks--;
}



FileNode *createNode(const char *name, NodeType type) {
    FileNode *node = (FileNode *)malloc(sizeof(FileNode));
    strcpy(node->name, name);
    node->type = type;
    node->next = node->prev = node->child = NULL;
    node->parent = cwd;
    node->blockCount = 0;
    memset(node->blockPointers, -1, sizeof(node->blockPointers));
    return node;
}

void insertNode(FileNode **list, FileNode *node) {
    if (!*list) {
        *list = node;
        node->next = node->prev = node;
    } else {
        FileNode *tail = (*list)->prev;
        tail->next = node;
        node->prev = tail;
        node->next = *list;
        (*list)->prev = node;
    }
}

FileNode *findNode(FileNode *list, const char *name) {
    if (!list)
        return NULL;
    FileNode *curr = list;
    do {
        if (strcmp(curr->name, name) == 0)
            return curr;
        curr = curr->next;
    } while (curr != list);
    return NULL;
}

void removeNode(FileNode **list, FileNode *node) {
    if (!*list || !node)
        return;
    if (node->next == node)
        *list = NULL;
    else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        if (*list == node)
            *list = node->next;
    }
    free(node);
}



int isValidDirName(const char *name) {
    if (name == NULL || strlen(name) == 0)
        return 0;

    for (int index = 0; name[index] != '\0'; index++) {
        char c = name[index];
        if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' ||
            c == '"' || c == '<' || c == '>' || c == '|' || isspace(c))
            return 0;
    }

    if (strlen(name) > MAX_NAME_LEN)
        return 0;

    return 1;
}



void mkdirCmd(char *name) {
    if (!isValidDirName(name)) {
        printf("Error: Invalid directory name.\n");
        return;
    }

    if (findNode(cwd->child, name)) {
        printf("Directory '%s' already exists.\n", name);
        return;
    }

    FileNode *dir = createNode(name, DIR_NODE);
    insertNode(&cwd->child, dir);
    printf("Directory '%s' created successfully.\n", name);
}

void createCmd(char *name) {
    if (findNode(cwd->child, name)) {
        printf("File '%s' already exists.\n", name);
        return;
    }
    FileNode *file = createNode(name, FILE_NODE);
    insertNode(&cwd->child, file);
    printf("File '%s' created successfully.\n", name);
}

void writeCmd(char *name, char *data) {
    FileNode *file = findNode(cwd->child, name);
    if (!file || file->type != FILE_NODE) {
        printf("File '%s' not found.\n", name);
        return;
    }

    int size = strlen(data);
    int neededBlocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    if (neededBlocks > MAX_BLOCKS_PER_FILE) {
        printf("File too large.\n");
        return;
    }

    for (int index = 0; index < neededBlocks; index++) {
        FreeBlock *blk = allocateBlock();
        if (!blk) {
            printf("Disk full.\n");
            return;
        }
        file->blockPointers[index] = blk->index;
        memcpy(virtualDisk[blk->index], data + index * BLOCK_SIZE, BLOCK_SIZE);
        free(blk);
    }

    file->blockCount = neededBlocks;
    printf("Data written successfully (size=%d bytes).\n", size);
}

void readCmd(char *name) {
    FileNode *file = findNode(cwd->child, name);
    if (!file || file->type != FILE_NODE) {
        printf("File '%s' not found.\n", name);
        return;
    }

    if (file->blockCount <= 0 || file->blockCount > MAX_BLOCKS_PER_FILE) {
        printf("Invalid or empty file '%s'.\n", name);
        return;
    }

    for (int index = 0; index < file->blockCount; index++) {
        int blockIndex = file->blockPointers[index];
        if (blockIndex < 0 || blockIndex >= NUM_BLOCKS) {
            printf("Error: Corrupted file data.\n");
            return;
        }

        char buffer[BLOCK_SIZE + 1];
        strncpy(buffer, (char *)virtualDisk[blockIndex], BLOCK_SIZE);
        buffer[BLOCK_SIZE] = '\0';
        printf("%s", buffer);
    }
    printf("\n");
}

void deleteCmd(char *name) {
    FileNode *file = findNode(cwd->child, name);
    if (!file || file->type != FILE_NODE) {
        printf("File '%s' not found.\n", name);
        return;
    }
    for (int index = 0; index < file->blockCount; index++)
        if (file->blockPointers[index] != -1)
            freeBlock(file->blockPointers[index]);
    removeNode(&cwd->child, file);
    printf("File deleted successfully.\n");
}

void rmdirCmd(char *name) {
    FileNode *dir = findNode(cwd->child, name);
    if (!dir || dir->type != DIR_NODE) {
        printf("Directory '%s' not found.\n", name);
        return;
    }
    if (dir->child) {
        printf("Directory not empty.\n");
        return;
    }
    removeNode(&cwd->child, dir);
    printf("Directory removed successfully.\n");
}

void lsCmd() {
    if (!cwd->child) {
        printf("(empty)\n");
        return;
    }
    FileNode *curr = cwd->child;
    do {
        printf("%s%s\n", curr->name, curr->type == DIR_NODE ? "/" : "");
        curr = curr->next;
    } while (curr != cwd->child);
}

void cdCmd(char *name) {
    if (strcmp(name, "..") == 0) {
        if (cwd->parent)
            cwd = cwd->parent;
        printf("Moved to %s\n", cwd == root ? "/" : cwd->name);
        return;
    }
    FileNode *dir = findNode(cwd->child, name);
    if (!dir || dir->type != DIR_NODE) {
        printf("Directory '%s' not found.\n", name);
        return;
    }
    cwd = dir;
    printf("Moved to %s\n", cwd->name);
}

void pwdCmd(FileNode *node) {
    if (node == root) {
        printf("/");
        return;
    }
    pwdCmd(node->parent);
    printf("%s", node->name);
    if (node != cwd)
        printf("/");
}

void dfCmd() {
    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", NUM_BLOCKS - usedBlocks);
    float usage = ((float)usedBlocks / NUM_BLOCKS) * 100.0;
    printf("Disk Usage: %.2f%%\n", usage);
}



void freeFileSystem(FileNode *node) {
    if (!node)
        return;

    if (node->child) {
        FileNode *start = node->child;
        FileNode *curr = start;
        do {
            FileNode *next = curr->next;
            freeFileSystem(curr);
            curr = next;
        } while (curr != start);
    }
    free(node);
}



int main() {
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    initFreeList();
    root = createNode("/", DIR_NODE);
    root->parent = NULL;
    cwd = root;

    char cmd[200], arg1[100], arg2[512];

    while (1) {
        printf("%s > ", cwd == root ? "/" : cwd->name);
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = 0;

        if (strncmp(cmd, "mkdir ", 6) == 0)
            mkdirCmd(cmd + 6);
        else if (strncmp(cmd, "create ", 7) == 0)
            createCmd(cmd + 7);
        else if (strncmp(cmd, "write ", 6) == 0) {
            char *p = cmd + 6;
            sscanf(p, "%s", arg1);
            char *textStart = strstr(p, arg1) + strlen(arg1);
            while (*textStart == ' ')
                textStart++;
            if (*textStart == '"') {
                textStart++;
                char *endQuote = strrchr(textStart, '"');
                if (endQuote)
                    *endQuote = '\0';
            }
            strcpy(arg2, textStart);
            writeCmd(arg1, arg2);
        } else if (strncmp(cmd, "read ", 5) == 0)
            readCmd(cmd + 5);
        else if (strncmp(cmd, "delete ", 7) == 0)
            deleteCmd(cmd + 7);
        else if (strncmp(cmd, "rmdir ", 6) == 0)
            rmdirCmd(cmd + 6);
        else if (strcmp(cmd, "ls") == 0)
            lsCmd();
        else if (strncmp(cmd, "cd ", 3) == 0)
            cdCmd(cmd + 3);
        else if (strcmp(cmd, "pwd") == 0) {
            pwdCmd(cwd);
            printf("\n");
        } else if (strcmp(cmd, "df") == 0)
            dfCmd();
        else if (strcmp(cmd, "exit") == 0) {
            printf("Releasing memory...\n");
            freeFileSystem(root);
            printf("Memory released. Exiting program...\n");
            break;
        } else
            printf("Invalid command.\n");
    }

    return 0;
}
