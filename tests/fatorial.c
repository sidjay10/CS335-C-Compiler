void print_int(int );
void print_char(char );
int scan_int();
void exit();

int fact( int n ) {
	if ( n <= 1 ) {
		return 1;
	}
	return n*fact(n-1);
}

int main() {
	int x;
	x = scan_int();
	x = fact(x);
	print_int(x);
	print_char('\n');
	exit();
	return 0;
}
