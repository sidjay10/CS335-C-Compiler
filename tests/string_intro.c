void print_int(int c);
void print_char(char c);
void print_string(char *);
void scan_string(char* str, int n);
int scan_int();
int main(){
    char c,*str,str2[10];
    c='a';
    str="hello world\n";
    // str2="hey abhay";
    scan_string(str2,10);
    print_string(str);
    print_string("Your compiler is ready\n");
    // print_str("\n");
    print_string(str2);
    return 0;
}