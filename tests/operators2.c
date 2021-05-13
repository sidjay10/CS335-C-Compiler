void print_int(int c);
void print_char(char c);
void print_string(char *);
void scan_string(char* str, int n);
int scan_int();

int main(){
    int a,b,c;
    print_string("Enter a and b:\n");
    a=scan_int();
    b=scan_int();
    if(a!=b){
        print_string("a!=b\n");
    }
    else{
        print_string("a=b\n");
    }
    print_string("Enter c:\n");
    c=scan_int();
    if(!c){
        print_string("c==0\n");
    }
    else{
        print_string("c!=0\n");
    }
    return 0;
}