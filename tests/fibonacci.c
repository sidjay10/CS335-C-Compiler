void print_int(int i);
int scan_int();

int fib(int n) {
	if ( n == 1 ) {
		return 0;
	} else if ( n == 2 ) {
		return 1;
	}
	return fib(n-1) + fib(n-2);
}

int main() {
	int x;
	x = 5;
	x = fib(x);
	print_int(x);
	return 0;
}
