#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10007  



typedef struct Node {
    int key;
    char value[100];
    struct Node *prev, *next;
} Node;



typedef struct {
    int capacity;
    int size;
    Node *head;      // MRU
    Node *tail;      // LRU
    Node *map[HASH_SIZE];
} LRUCache;



int hash(int key) {
    return key % HASH_SIZE;
}



void removeNode(LRUCache *cache, Node *node) {
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

void insertAtHead(LRUCache *cache, Node *node) {
    node->prev = NULL;
    node->next = cache->head;

    if (cache->head)
        cache->head->prev = node;

    cache->head = node;

    if (cache->tail == NULL)
        cache->tail = node;
}



LRUCache* createCache(int capacity) {
    LRUCache *cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = cache->tail = NULL;

    for (int i = 0; i < HASH_SIZE; i++)
        cache->map[i] = NULL;

    return cache;
}



char* get(LRUCache *cache, int key) {
    int idx = hash(key);
    Node *node = cache->map[idx];

    while (node && node->key != key)
        node = node->next;

    if (!node)
        return NULL;

    removeNode(cache, node);
    insertAtHead(cache, node);

    return node->value;
}



void put(LRUCache *cache, int key, char *value) {
    int idx = hash(key);
    Node *node = cache->map[idx];

    while (node && node->key != key)
        node = node->next;

    if (node) {
        strcpy(node->value, value);
        removeNode(cache, node);
        insertAtHead(cache, node);
        return;
    }

    if (cache->size == cache->capacity) {
        Node *lru = cache->tail;
        int oldKey = lru->key;
        int oldIdx = hash(oldKey);

        Node *entry = cache->map[oldIdx];
        Node *prev = NULL;

        while (entry && entry != lru) {
            prev = entry;
            entry = entry->next;
        }

        if (prev)
            prev->next = entry->next;
        else
            cache->map[oldIdx] = entry->next;

        removeNode(cache, lru);
        free(lru);
        cache->size--;
    }

    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->prev = newNode->next = NULL;

    insertAtHead(cache, newNode);

    newNode->next = cache->map[idx];
    cache->map[idx] = newNode;

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
            if (res)
                printf("%s\n", res);
            else
                printf("NULL\n");
        } 
        else if (strcmp(command, "exit") == 0) {
            break;
        }
    }

    return 0;
}
