void print_int(int x);
int scan_int();
void exit();
void print_char(char c);
void print_string(char* c);
void scan_string(char *, int);
int open_file( char * , int , int );
int read_file( int , char *, int );


int main(){
	int x;
	int *p;
	int fd;
	char str[10];
	if ( false || true) {
		print_int(1);
	}

	if ( false && true) {
		print_int(2);
	}
	return 0;
}
