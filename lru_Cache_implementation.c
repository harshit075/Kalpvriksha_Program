#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10007


typedef struct LRUNode {
    int key;
    char value[100];
    struct LRUNode *prev, *next;
} LRUNode;


typedef struct HashEntry {
    int key;
    LRUNode *node;              
    struct HashEntry *next;
} HashEntry;


typedef struct {
    int capacity;
    int size;
    LRUNode *head;              
    LRUNode *tail;              
    HashEntry *map[HASH_SIZE];  
} LRUCache;


int hash(int key) {
    return key % HASH_SIZE;
}


void removeNode(LRUCache *cache, LRUNode *node) {
    if (!node) return;

    if (node->prev)
        node->prev->next = node->next;
    else
        cache->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        cache->tail = node->prev;
}


void insertAtHead(LRUCache *cache, LRUNode *node) {
    node->prev = NULL;
    node->next = cache->head;

    if (cache->head)
        cache->head->prev = node;

    cache->head = node;

    if (!cache->tail)
        cache->tail = node;
}


HashEntry* findEntry(LRUCache *cache, int key) {
    int idx = hash(key);
    HashEntry *cur = cache->map[idx];
    while (cur) {
        if (cur->key == key) return cur;
        cur = cur->next;
    }
    return NULL;
}


void insertHash(LRUCache *cache, int key, LRUNode *node) {
    int idx = hash(key);
    HashEntry *entry = malloc(sizeof(HashEntry));
    if (!entry) {
        printf("Memory error\n");
        exit(1);
    }
    entry->key = key;
    entry->node = node;
    entry->next = cache->map[idx];
    cache->map[idx] = entry;
}


void removeHash(LRUCache *cache, int key) {
    int idx = hash(key);
    HashEntry *cur = cache->map[idx];
    HashEntry *prev = NULL;

    while (cur) {
        if (cur->key == key) {
            if (prev) prev->next = cur->next;
            else cache->map[idx] = cur->next;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}


LRUCache* createCache(int capacity) {
    LRUCache *cache = malloc(sizeof(LRUCache));
    if (!cache) {
        printf("Memory error.\n");
        exit(1);
    }

    cache->capacity = capacity;
    cache->size = 0;
    cache->head = cache->tail = NULL;

    for (int i = 0; i < HASH_SIZE; i++)
        cache->map[i] = NULL;

    return cache;
}


char* get(LRUCache *cache, int key) {
    HashEntry *entry = findEntry(cache, key);
    if (!entry) return NULL;

    LRUNode *node = entry->node;

    // Move to MRU
    removeNode(cache, node);
    insertAtHead(cache, node);

    return node->value;
}


void put(LRUCache *cache, int key, char *value) {
    HashEntry *entry = findEntry(cache, key);

    if (entry) {
        LRUNode *node = entry->node;
        strcpy(node->value, value);
        removeNode(cache, node);
        insertAtHead(cache, node);
        return;
    }

    
    if (cache->size == cache->capacity) {
        int oldKey = cache->tail->key;
        removeHash(cache, oldKey);

        LRUNode *lru = cache->tail;
        removeNode(cache, lru);
        free(lru);
        cache->size--;
    }

   
    LRUNode *newNode = malloc(sizeof(LRUNode));
    if (!newNode) {
        printf("Memory error\n");
        exit(1);
    }

    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->prev = newNode->next = NULL;

    insertAtHead(cache, newNode);
    insertHash(cache, key, newNode);
    cache->size++;
}


int main() {
    LRUCache *cache = NULL;
    char command[20];

    while (scanf("%s", command) != EOF) {
        if (strcmp(command, "createCache") == 0) {
            int cap;
            scanf("%d", &cap);
            cache = createCache(cap);
        }
        else if (strcmp(command, "put") == 0) {
            int key;
            char val[100];
            scanf("%d %s", &key, val);
            put(cache, key, val);
        }
        else if (strcmp(command, "get") == 0) {
            int key;
            scanf("%d", &key);
            char *res = get(cache, key);
            if (res) printf("%s\n", res);
            else printf("NULL\n");
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
    }

    return 0;
}
