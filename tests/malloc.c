void print_int(int c);
void print_char(char c);
void print_string(char *);
void scan_string(char* str, int n);
int scan_int();
int* malloc(int n);
int main(){
    int n,*arr;
    // print_int(sizeof(int));
    arr=malloc(3*sizeof(int));
    for(n=0;n<3;n++){
        arr[n]=scan_int();
    }
    print_string("Numbers\n");
    for(n=0;n<3;n++){
        print_int(arr[n]);
    }
    return 0;
}