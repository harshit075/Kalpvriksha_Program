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

Node* middleNode(Node* head){
    Node* slow = head;
    Node* fast = head;
    while(fast!=NULL && fast->next!=NULL){
        fast = fast->next->next;
        slow = slow->next;
    }
    return slow;
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

    printf("Middle of the linkedlist: ");
    head = middleNode(head);
    printList(head);
    return 0;

}