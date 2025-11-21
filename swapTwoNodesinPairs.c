#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
} Node;

Node* createNode(int data){
    Node * newNode = (Node*) malloc(sizeof(newNode));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* takeInput(){
    int num, data;
    printf("Enter the Number of Nodes: ");
    scanf("%d", &num);

    Node *head = NULL, *tail = NULL;

    printf("Enter %d elements :", num);

    for(int index = 0; index<num; index++){
        scanf("%d", &data);

        Node* newNode = createNode(data);

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

Node* swapPairs(Node* head){
   if(head == NULL && head ->next == NULL){
    return head;
   }

   Node *prev = NULL;
   Node *first = head;
   Node *second = head->next;
   while(first != NULL && second != NULL){
    Node *third = second->next;
    second->next = first;
    first->next = third;

    if(prev!= NULL){
        prev->next = second;
    }
    else{
        head = second;
    }
    prev = first;
    first = third;
    if(third != NULL){
        second = third ->next;
    }
    else{
        second = NULL;
    }
   }
   return head;
}

int main(){
    Node *head = takeInput();
    printf("Original List : ");
    printlist(head);

    Node* swapped = swapPairs(head);
    printf("Swapped pairs");
    printlist(swapped);
}