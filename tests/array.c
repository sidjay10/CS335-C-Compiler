void print_int(int x);
void print_char(char c);
void exit();

int main() {
	int a[10][10];
	int i,j;
	for ( i = 0; i < 10; i++ ) {
		for ( j = 0; j < 10; j++ ) {
			a[i][j] = i + j;
		}
	}
	for ( i = 0; i < 10; i++ ) {
		for ( j = 0; j < 10; j++ ) {
			print_int(a[i][j]);
			print_char(' ');
		}
		print_char('\n');
	}
	exit();
	return 0;
}
