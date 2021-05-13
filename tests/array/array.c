void print_int(int x);
void print_char(char c);
void exit();

int main() {
	int a[10];
	int i;
	for ( i = 0; i < 10; i ++ ) {
		a[i] = i;
	}
	for ( i = 0; i < 10; i ++ ) {
		print_int(a[i]);
		print_char('\n');
	}
	exit();
	return 0;
}
