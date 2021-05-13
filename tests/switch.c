void print_int(int c);
void print_char(char c);
void print_string(char *);
int scan_int();
int main(){
    int x;
    x=scan_int();
    switch(x){
        case 1 : print_string("x==1\n");
                break;
        case 2: print_string("x==2\n");
                break;
        case 3: print_string("x==3\n");
                break;
        default :
                print_string("x>3\n");
    }

    x=scan_int();
    switch(x){
        case 1 : print_string("case1\n");
                break;
        default :
                print_string("default\n");

        case 2: print_string("case2\n");
                
        case 3: print_string("case3\n");
                break;
       
    }

    return 0 ;

}