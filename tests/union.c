void print_int(int x);
int scan_int();
char scan_char();
void exit();
void print_char(char c);
void print_string(char * c);

union data { char a; int b; } u;


int main(){

	print_string("Sizeof union: ");
	print_int(sizeof(u));
	print_char('\n');
	u.b = 0;
	print_string("Enter a character: ");
	u.a = scan_char();
	print_int(u.b);
	print_string("Enter an integer : ");
	u.b = scan_int();
	print_char(u.a);
	print_char('\n');
	
    return 0;
}

