#include<stdio.h>
#include<stdlib.h>

typedef struct Node {
    int data;
    struct Node * next;
}Node;

Node* createNode(int data){
    Node* newNode= (Node*) malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* takeInput(){
    int num, val;
    printf("Enter number of nodes: ");
    scanf("%d", &num);

    Node *head = NULL, *tail = NULL;
    printf("Enter %d elements: ", num);
    for(int index=0; index<num; index++){
        scanf("%d", &val);
        Node* newNode = createNode(val);

        if(head == NULL){
            head = newNode;
            tail = newNode;
        }else{
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}
Node* rotateRight(Node* head, int k){
    if (!head || k == 0) return head;

    Node* temp = head;
    int len = 1;
    while (temp->next) {
        temp = temp->next;
        len++;
    }
    temp->next = head;  // make circular

    k = k % len;
    int stepsToNewHead = len - k;

    Node* newTail = head;
    for (int i = 1; i < stepsToNewHead; i++)
        newTail = newTail->next;

    Node* newHead = newTail->next;
    newTail->next = NULL;

    return newHead;
}
void printList(Node* head){
    while(head!=NULL){
        printf("%d->",head->data );
        head= head->next;
    }
    
} 

int main(){
    Node* head = takeInput();
    printf("Original Linked List: ");
    printList(head);

    int k;
    printf("Enter k (rotation steps): ");
    scanf("%d", &k);

    head = rotateRight(head, k);

    printf("Rotated Linked List: ");
    printList(head);
    return 0;

}