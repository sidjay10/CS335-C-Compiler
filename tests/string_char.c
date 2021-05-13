void print_int(int c);
void print_char(char c);
void print_string(char *);
void scan_string(char* str, int n);
int scan_int();


int main(){
    char *arr2,*arr3,*arr4;
    char arr6[20],arr7[20],arr8[20];
    char c;
    c='a';
    arr3="hello_world";
    arr4="ell";

    print_string(arr3);
    print_char('\n');
    print_string(arr4);
    return 0;
}