void print_int(int i);
void print_char(char c);
void exit();

int main() {
	int a,b,c,d,e,f,g,h,i,j,k,l,m;
	int x,y,z;
	a = 1;
	b = 2;
	c = 3;
	d = 4;
	e = 5;
	f = 6;
	g = 7;
	h = 8;
	i = 9;
	j = 10;
	k = 11;
	l = 12;
	m = 13;
	x = 24;
	y = 25;
	z = ((a + b) + c) + ((d + e) + f) + ((g + h) + (i + j)) + (((k + l) + m) + (x + y));
	print_int(z);
	print_char('\n');

	return 0;
}
