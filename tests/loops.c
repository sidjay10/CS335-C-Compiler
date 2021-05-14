// iteration statements, selection statements (without switch) and jump statements
void print_int(int c);
void print_char(char c);
void print_string(char *);
int scan_int();

int main(){
    int t,a;
    int *x;
    x = &a;
    t=3,a=1;
    // simple while and break and continue
    while( a < 7 && a > 9 || t != 4 ){
        a+=2;
        a*=3;
        t++;
        print_string("Inside while\n");
    }
    t++;
    print_int(a);
    print_char('\n');
    // do while
    a=7;
    do {
        a--;
        print_string("Inside do_while\n");
    }
    while(a<5);
    
    
    // for second catagory
    t=0;
    for(a=1;a<5;a++){
        t+=a;   
    }
    print_int(t);
    print_char('\n');
    // nested loops
    print_int(a);
    print_char('\n');
    while(a--){
        for(t=a;t>0;t--){
            print_string("Inner_loop\n");
        }
        print_string("Outer_loop\n");
    }


    return 0;
}