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


void printList(Node* head){
    while(head!=NULL){
        printf("%d->",head->data );
        head= head->next;
    }
    
} 

Node* mergedList(Node* head1, Node* head2){
    if(head1 == NULL){
        return head2;
    }
    if(head2 == NULL){
        return head1;
    }
    Node* dummy = (Node*) malloc(sizeof(Node));
    dummy->next = NULL;

    Node* tail = dummy;

    while(head1 != NULL && head2 != NULL){
        if(head1->data < head2->data){
            tail->next = head1;
            head1 = head1->next;
        }
        else{
            tail->next = head2;
            head2 = head2->next;
        }
        tail = tail->next;
    }
    if(head1!=NULL){
        tail->next = head1;
    }
    else{
        tail->next = head2;
    }
    Node* mergedHead = dummy->next;
    free(dummy);
    return mergedHead;
}
int main(){
    Node* head1 = takeInput();
    printf("first Node: ");
    printList(head1);
    
     Node* head2 = takeInput();
    printf("Second Node: ");
    printList(head2);

    printf("Merged linkedlist: ");
    Node* mergedHead  = mergedList(head1, head2);
    printList(mergedHead);
    return 0;

}