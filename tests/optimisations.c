void print_int(int c);
void print_char(char c);
int scan_int();
void print_string(char *);
int main(){
    int a,b,c;
    a = 14*3+5+(4+(8*12/73))-23*76+48;
    print_int(a);

    // multiply and divide by power of 2 is shift
    b = a*2;
    b= a/16;

    b=a*0;
    b=a+0;

    while(1){
        print_string("Inside while\n");
        b++;
        if(b==7){
            break;
        }
    }

    return 0;
}