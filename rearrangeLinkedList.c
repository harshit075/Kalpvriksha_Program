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

Node* rearrangeList(Node* head){
    if (head == NULL){
        return NULL;
    }

    Node *zeroHead = createNode(-1);
    Node *oneHead = createNode(-1);
    Node *twoHead = createNode(-1);

    Node *zero = zeroHead;
    Node *one = oneHead;
    Node *two = twoHead;

    Node* temp = head;

    while(temp){
        if (temp->data == 0)
        {
            zero->next = temp;
            zero = temp;
        }
        else if (temp->data == 1)
        {
            one->next = temp;
            one = temp;
        }
        else
        {
            two->next = temp;
            two = temp;
        }
        temp = temp->next;
    }
    if (oneHead->next)
        zero->next = oneHead->next;
    else
        zero->next = twoHead->next;

    if (twoHead->next)
        one->next = twoHead->next;
    else
        one->next = NULL;

    two->next = NULL;

    Node *list = zeroHead->next;
    free(zeroHead);
    free(oneHead);
    free(twoHead);
    return list;
}


Node* takeInput(){
    int num, val;
    printf("Enter the Nodes : ");
    scanf("%d", &num);

    Node *head = NULL, *tail = NULL;

    printf("Enter %d elements: ", num);
    for(int index = 0; index < num; index++){
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

void printlist(Node* head){
    Node* temp = head;
    while(temp!= NULL){
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main(){
    Node* head = takeInput();
    printf("Original List: ");
    printlist(head);
    return 0;
}