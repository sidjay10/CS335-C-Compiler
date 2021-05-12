void print_int( int x);
void print_char( char x);
int scan_int();
void exit();

int partition( int * data, int low, int high ) {
	int i,j,p,t;
	p = data[high];
	i = low - 1;
	for ( j = low; j <= high - 1; j++ ) {
		if ( data[j] < p ) {
			i++;
			t = data[i];
			data[i] = data[j];
			data[j] = t;
			
		}
		
	}
	t = data[i+1];
	data[i+1] = data[high];
	data[high] = t;
	return i + 1;
}





void quick_sort (int * data, int low, int high) {
        int i, j, p, t;
	if ( low >= high ) 
		return;
	p = partition( data, low, high);

	quick_sort( data, low, p-1);
	quick_sort( data, p+1, high);

}
int main() {
	int i;
	int j;
	int k;
	int a[512];
	j = 31;
	for ( i = 0; i < 512; i++) 
	{ 
		a[i] = j - i;
		print_int(a[i]);
		print_char(' ');
	}
	print_char('\n');
	for ( i = 0; i < 512; i++) 
	{ 
		print_int(a[i]);
		print_char(' ');
	}
	print_char('\n');
	quick_sort(a,0,511);
	for ( i = 0; i < 512; i++) 
	{ 
		print_int(a[i]);
		print_char(' ');
	}
	print_char('\n');
	return 0;
}
