void print_int(int x);
int scan_int();
void exit();
void print_char(char c);

union data { char a; int b; } u;


int main(){
	u.a = 'a';
	
	print_char(u.a);
	print_char('\n');
	print_int(u.b);
    return 0;
}

