#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
} Node;

Node* createNewNode(int data){
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* takeInput(){
    int num, data;
    printf("Enter the Number of Nodes: ");
    scanf("%d", &num);

   
    Node* head = NULL, *tail = NULL;

    printf("Enter %d elements : ", num);

    for(int index = 0; index<num; index++){
        scanf("%d", &data);

        Node *newNode = createNewNode(data);

        if(head == NULL){
            head = newNode;
            tail = newNode;
        }
        else{
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

void printlist(Node* head){
    while(head != NULL){
        printf("%d->", head->data);
        head = head->next;
    }
}

Node *reverselist(Node* head){
    Node* prev = NULL;
    Node *curr = head;
    Node *next = NULL;

    while(curr != NULL){
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

Node* removeNthFromEnd(Node* head, int n){
    if(head == NULL) return NULL;

    head = reverselist(head);

    if(n == 1){
        Node* temp = head;
        head = head ->next;
        free(temp);
    }
    else{
        Node* curr = head;
        for(int index = 1; curr != NULL &&  index <n-1; index++){
            curr = curr->next;
        }
        if(curr != NULL && curr->next != NULL){
            Node *temp = curr->next;
            curr->next = curr->next->next;
            free(temp);
        }
    }
    head = reverselist(head);
    return head;
}

int main(){
    Node* head = takeInput();
    printf("Original List: ");
    printlist(head);
    int n;
    printf("Enter the n Node to delete: ");
    scanf("%d", &n);

    Node* removed = removeNthFromEnd(head, n);
    printf("New List: ");
    printlist(removed);
}