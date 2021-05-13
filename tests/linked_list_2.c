void print_int(int c);
void print_char(char c);
void print_string(char *);
void scan_string(char* str, int n);
int scan_int();

struct node {
   int data;
   struct node *next;
};
struct node* malloc(int n);

// struct node* null;
void insert(struct node* head, struct node* current,int n, struct node* null){
    struct node* new_node;
    new_node=malloc(sizeof(struct node));
    new_node->data=n;
    new_node->next=null;
    if(head==null){
        head=new_node;
        current=new_node;
    }
    else{
        current->next=new_node;
        current=current->next;
        print_string("\nCurrent_updated\n");
    }
    return ;
}
void print_linked_list(struct node *head,struct node * null){
    struct node *temp;
    temp=head;
    while(temp->next!=null){
        print_int(temp->data);
        print_string(" -> ");
        temp=temp->next;
    }
    print_int(temp->data);
    print_string("\n"); 
}

int main() {
    struct node *null,*head,*current;
    null=malloc(0);
    head=null;
    current=null;
    insert(head,current,5,null);
    print_int(head->data);
    print_int(current->data);
    print_string("\n");
    // insert(head,current,10,null);
    print_int(head->data);
    print_int(current->data);
    print_string("\n");
    // print_int(current->data);
    // print_linked_list(head,null);
   return 0;
}