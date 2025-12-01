#include<stdio.h>
#include<stdlib.h>


typedef struct Node {
    int data;
    struct Node* next;
} Node;


Node* createNode(int data){
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode -> data = data;
    newNode ->next = NULL;
    return newNode;
}

int loopDetection(Node* head){
    if(head == NULL){
        return NULL;
    }
    if(head->next = NULL){
        return 0;
    }
    Node* slow = head;
    Node* fast = head;
    int status = 0;
    while (fast != NULL && fast->next != NULL){
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast){
            status = 1;
            break;
        }
    }
    if(fast == NULL){
        return status;
    }
    return status;
}

int length(Node* head){
    Node* slow = head;
    Node* fast = head;
    while(fast != NULL){
        slow = slow->next;
        fast = fast ->next->next;
        if(slow == fast){
            break;
        }
    }
    slow = head;
    while(slow != fast){
        fast = fast->next;
        slow = slow ->next;
    }

    int count = 1;
    fast = fast->next;
    while(fast != slow){
        fast = fast->next;
        count++;
    }
    return count;
}
Node* takeInput(){
    int num, val;
    printf("Enter the number of Nodes: ");
    scanf("%d", &num);

    Node* head = NULL, *tail = NULL;
    printf("Enter the elements %d", num);
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
    while(temp != NULL){
        printf("%d ->", head->data);
        head = head->next;
    }
    printf("\n");
    
}

int main(){
    Node* head = takeInput();
    printf("Original list: ");
    printlist(head);

    int check = loopDetection(head);
    if(check == 1){
        int val = length(head);
        printf("Length of the Loop is : %d", val);
    }else{
        printf("No loop");
    }
}
