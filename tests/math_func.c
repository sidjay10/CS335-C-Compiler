void print_int(int c);
void print_char(char c);
int scan_int();
int pow(int b, int e);
int fpow(int b, int e);
int iseven(int n);
int gcd(int a, int b);
int max(int a, int b);
int min(int a, int b);


int main(){
    int a,b;
    a=scan_int();
    b=scan_int();
    print_string("power of a and b is: ");
    print_int(pow(a,b));
    print_string("\npower of a and b is: ");
    print_int(fpow(a,b));
    print_string("\nmax of a and b is: ");
    print_int(max(a,b));
    print_string("\nmin of a and b is: ");
    print_int(min(a,b));
    print_string("\ngcd of a and b is: ");
    print_int(gcd(a,b));
}
