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
Node* deleteDuplicates(Node* head) {
    if (!head || !head->next) return head;

    Node* dummy = createNode(0);
    dummy->next = head;
    Node* prev = dummy;
    Node* curr = head;

    while (curr) {
        int duplicate = 0;
        while (curr->next && curr->data == curr->next->data) {
            duplicate = 1;
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }

        if (duplicate) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
            prev->next = curr;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    Node* newHead = dummy->next;
    free(dummy);
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

    head = deleteDuplicates(head);
    printf("After removing duplicates: ");
    printList(head);
    return 0;

}