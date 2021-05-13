void print_int(int c);
void print_char(char c);
void print_string(char *);
int scan_int();

int main(){

    int a,b,c,d,e,f,g;
    a=3;
    if(0){
        a*=7;
        c=34;
        b=41;
        d=a^c*(b);
        print_int(a);
        print_int(b);
        print_int(c);
        print_int(d);
    }
    b=a^3;
    return 0;
}