void print_int( int x);
void print_char( char x);
void print_string(char * );
void scan_string( char *, int );
int scan_int();
void print_char(char c);
void print_string(char *);
void scan_string(char* str, int n);
int scan_int();
void exit( int );


void * sbrk(int);


struct node {
	int key;
	struct node * left;
	struct node * right;
};

void print_btree( struct node * root ) {
	if (root == (struct node *)NULL){
		return;
	}
	print_btree(root->left);
	print_int(root->key);
	print_char(' ');
	print_btree(root->right);
	return;
}

void add_to_btree(struct node * root, int key ) {
	if ( key < root->key ){
		struct node * new;
		if (root->left != (struct node *)NULL){
			add_to_btree(root->left,key);
			return;
		}
		new = (struct node * )sbrk(sizeof(struct node));
		if (new == (struct node *) NULL ){
			print_string("sbrk failed\n");
			exit(0);
		}
		new->key = key;
		new->left = (struct node *)NULL;
		new->right = (struct node *)NULL;
		root->left = new;
		return;
	} else {
		struct node * new;
		if (root->right != (struct node *)NULL){
			add_to_btree(root->right,key);
			return;
		}
		new = (struct node * )sbrk(sizeof(struct node));
		if (new == (struct node *) NULL ){
			print_string("sbrk failed\n");
			exit(0);
		}
		new->key = key;
		new->left = (struct node *)NULL;
		new->right = (struct node *)NULL;
		root->right = new;
		return;
	}
}

struct node * find_in_btree( struct node * head, int value );


int main(){
	struct node * root;
	int i,n,k;
	print_string("How many nodes to create?\n");
	n = scan_int();
	if ( n== 0 ) {
		return 0;
	}
	root = (struct node * )sbrk(sizeof(struct node));
	if (root == (struct node *) NULL ){
		print_string("sbrk failed\n");
		exit(0);
	}
	print_string("Enter values to insert\n");
	k = scan_int();
	root->key = k;
	root->left = (struct node *) NULL;
	root->right = (struct node *) NULL;
	for(i = 0; i < n-1; i++ ) {
		k = scan_int();
		add_to_btree(root,k);
	}
	print_btree(root);
	print_char('\n');
//	print_string("Enter value to find?\n");
//	n = scan_int();
//	ptr = find_in_ll(head,n);
//	if (ptr == (struct node *) NULL ){
//		print_string("Value not found in linked list\n");
//	} else {
//		print_string("Value found in linked list\n");
//		print_int(ptr->key);
//		print_char('\n');
//	}
	return 0;
}


