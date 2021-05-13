void print_int( int x);
void print_char( char x);
void print_string(char * );
void scan_string( char *, int );
int scan_int();
void print_char(char c);
void print_string(char *);
void scan_string(char* str, int n);
int scan_int();


void * sbrk(int);


struct node {
	int key;
	struct node * next;
};

void print_ll( struct node * head ) {
	struct node * ptr;
	ptr = head;
	while ( ptr != (struct node *) NULL ) {
		print_int(ptr->key);
		print_char(' ');
		ptr = ptr->next;
	}
	print_char('\n');
	return;
}

struct node * find_in_ll( struct node * head, int value ) {
	struct node * ptr;
	ptr = head;
	while ( ptr != (struct node *) NULL && ptr->key != value ) {
		ptr = ptr->next;
	}
	return ptr;
}
int main(){
	struct node * head;
	struct node * ptr;
	int i,n;
	print_string("How many nodes to create?\n");
	n = scan_int();
	head = (struct node * )sbrk(sizeof(struct node));
	if (head == (struct node *) NULL ){
		print_string("sbrk failed\n");
		return 0;
	}
	ptr = head;
	ptr->key = 0;
	ptr->next = (struct node * ) NULL;
	for(i = 1; i < n; i++ ) {
		struct node * new;
		new = (struct node * )sbrk(sizeof(struct node));
		if (new == (struct node *) NULL ){
			print_string("sbrk failed\n");
			return 0;
		}
		new->key = (i+1)*(n) + i;
		ptr->next = new;
		new->next = NULL;
		ptr = new;
	}
	print_ll(head);
	print_string("Enter value to find?\n");
	n = scan_int();
	ptr = find_in_ll(head,n);
	if (ptr == (struct node *) NULL ){
		print_string("Value not found in linked list\n");
	} else {
		print_string("Value found in linked list\n");
		print_int(ptr->key);
		print_char('\n');
	}
	return 0;
}


