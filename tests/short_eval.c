void print_int(int c);
void print_char(char c);
void print_string(char *);
int scan_int();

int func(){
    print_string('Short Circuit Failed');
    return 1;
}
int main(){
    int a,b,c,d;
    a=2,b=5,c=7;
    if( a==3 && func() ){
        b=3;
    }
    print_int(a);
    print_int(b);
    print_int(c);
    print_char('\n');
    if( a==2 || func() ){
        b=6;
    }
    print_int(a);
    print_int(b);
    print_int(c);
    print_char('\n');
}