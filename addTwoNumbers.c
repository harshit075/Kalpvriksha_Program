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
Node* addTwoNumbers(Node* head1, Node* head2){
if(head1 == NULL && head2 == NULL){
    return NULL;
}
Node *dummy = createNode(0);
Node *temp = dummy;
int carry = 0;

while (head1 != NULL || head2 != NULL || carry != 0){
    int sum = carry;

    if(head1 != NULL ){
        sum += head1->data;
        head1 = head1->next;
    }
    if(head2 != NULL ){
        sum += head2->data;
        head2 = head2->next;
    }

    carry = sum / 10;
    int digit = sum % 10;

    temp ->next = createNode(digit);
    temp = temp->next;
}
Node* result = dummy->next;
free(dummy);
return result;
}
Node* takeInput(){
    int num, data;
    printf("Enter the value of Nodes : ");
    scanf("%d", &num);

    Node *head = NULL, *tail = NULL;

    printf("Enter the elements %d :", num);
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

void printlist(Node *head){
    while(head != NULL){
        printf("%d ->", head->data);
        head = head->next;
    }
}

int main(){

    Node * head1 = takeInput();
    Node * head2 = takeInput();
    printf("Original List1: ");
    printlist(head1);
    printf("Original List2: ");
    printlist(head2);
}