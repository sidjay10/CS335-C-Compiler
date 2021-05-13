void print_int(int c);
void print_char(char c);
void print_string(char *);
void scan_string(char* str, int n);
int scan_int();

int x;
int foo(){
    x*=6;
    x++;
    // print_int(x);
    return x;
}
int main(){
    x=5;
    print_string("Initial value of x : ");
    print_int(x);
    foo();
    print_string("\nFinal value of x : ");
    print_int(x);
    return 0;
}