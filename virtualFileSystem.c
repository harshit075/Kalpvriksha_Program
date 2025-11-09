#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME 50
#define MAX_BLOCKS_PER_FILE 20



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
    for (int i = 0; i < NUM_BLOCKS; i++) {
        FreeBlock *node = (FreeBlock *)malloc(sizeof(FreeBlock));
        node->index = i;
        node->next = node->prev = NULL;
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
    if (!freeListHead) return NULL;
    FreeBlock *block = freeListHead;
    if (block->next == block) {
        freeListHead = NULL;
    } else {
        freeListHead->prev->next = freeListHead->next;
        freeListHead->next->prev = freeListHead->prev;
        freeListHead = freeListHead->next;
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
    if (!list) return NULL;
    FileNode *curr = list;
    do {
        if (strcmp(curr->name, name) == 0)
            return curr;
        curr = curr->next;
    } while (curr != list);
    return NULL;
}

void removeNode(FileNode **list, FileNode *node) {
    if (!*list || !node) return;
    if (node->next == node)
        *list = NULL;
    else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        if (*list == node) *list = node->next;
    }
    free(node);
}


void mkdirCmd(char *name) {
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

    for (int i = 0; i < neededBlocks; i++) {
        FreeBlock *blk = allocateBlock();
        if (!blk) {
            printf("Disk full.\n");
            return;
        }
        file->blockPointers[i] = blk->index;
        memcpy(virtualDisk[blk->index], data + i * BLOCK_SIZE, BLOCK_SIZE);
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
    for (int i = 0; i < file->blockCount; i++) {
        printf("%s", virtualDisk[file->blockPointers[i]]);
    }
    printf("\n");
}

void deleteCmd(char *name) {
    FileNode *file = findNode(cwd->child, name);
    if (!file || file->type != FILE_NODE) {
        printf("File '%s' not found.\n", name);
        return;
    }
    for (int i = 0; i < file->blockCount; i++)
        if (file->blockPointers[i] != -1)
            freeBlock(file->blockPointers[i]);
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
        if (curr->type == DIR_NODE)
            printf("%s/\n", curr->name);
        else
            printf("%s\n", curr->name);
        curr = curr->next;
    } while (curr != cwd->child);
}

void cdCmd(char *name) {
    if (strcmp(name, "..") == 0) {
        if (cwd->parent)
            cwd = cwd->parent;
        printf("Moved to /%s\n", cwd == root ? "" : cwd->name);
        return;
    }
    FileNode *dir = findNode(cwd->child, name);
    if (!dir || dir->type != DIR_NODE) {
        printf("Directory '%s' not found.\n", name);
        return;
    }
    cwd = dir;
    printf("Moved to /%s\n", cwd->name);
}

void pwdCmd(FileNode *node) {
    if (node == root) {
        printf("/\n");
        return;
    }
    pwdCmd(node->parent);
    printf("%s/", node->name);
}

void dfCmd() {
    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", NUM_BLOCKS - usedBlocks);
    float usage = ((float)usedBlocks / NUM_BLOCKS) * 100.0;
    printf("Disk Usage: %.2f%%\n", usage);
}



int main() {
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    initFreeList();
    root = createNode("/", DIR_NODE);
    root->parent = NULL;
    cwd = root;

    char cmd[100], arg1[100], arg2[512];

    while (1) {
        printf("%s > ", cwd == root ? "/" : cwd->name);
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = 0;

        if (strncmp(cmd, "mkdir ", 6) == 0)
            mkdirCmd(cmd + 6);
        else if (strncmp(cmd, "create ", 7) == 0)
            createCmd(cmd + 7);
        else if (strncmp(cmd, "write ", 6) == 0) {
            sscanf(cmd + 6, "%s \"%[^\"]\"", arg1, arg2);
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
            printf("Memory released. Exiting program...\n");
            break;
        } else
            printf("Invalid command.\n");
    }
    return 0;
}
