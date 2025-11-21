#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
} Node;

Node* createNode(int data){
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* takeInput(){
    int num, val;
    printf("Enter the Number of Nodes: ");
    scanf("%d", &num);

    Node * head = NULL, *tail = NULL;

    printf("Enter %d elements: ", num);
    for(int index = 0; index<num; index++){
        scanf("%d", &val);

        Node* newNode = createNode(val);


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

void printlist(Node *head){
    while(head!= NULL){
        printf("%d ->", head->data);
        head = head->next;
    }
}

Node* MergeTwoNodebtwZero(Node* head){
    Node* temp = head->next;
    Node* newHead = NULL;
    Node* tail = NULL;

    int sum = 0;

    while(temp != NULL){
        if(temp->data != 0){
            sum += temp->data;
        }else{
            Node* newNode = createNode(sum);
            sum =0;

            if(newHead == NULL){
                newHead = newNode;
                tail = newNode;
            }
            else{
                tail ->next = newNode;
                tail = newNode;
            }
        }
        temp = temp ->next;
    }
    return newHead;
}

int main(){
    Node *head = takeInput();
    printf("Original List is : ");
    printlist(head);
    
    Node *merged = MergeTwoNodebtwZero(head);
    printf("Merged sum : ");
    printlist(merged);
    return 0;
}