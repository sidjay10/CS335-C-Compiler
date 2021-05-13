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
	print_string("Enter file name\n");
	scan_string(str,10);
	fd = open_file(str,0,0);	
	print_int(fd);
	print_char('\n');
	read_file(fd,str,5);
	str[5] = '\0';
	print_string(str);
	return 0;
}
