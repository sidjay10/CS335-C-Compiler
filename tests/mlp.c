void print_int( int x);
void print_char( char x);
int scan_int();
void print_string(char *);
int main(){
    int a,b,c,x;
    int *ptra,*ptrb,*ptrc;
    int **pptra,**pptrb,**pptrc,***ppptrb;
    a=71;
    b=57,c=b*a+34;

    ptra=&a,ptrb=&b,ptrc=&c;
    pptra=&ptra,pptrb=&ptrb;
    ppptrb=&pptrb;
    *(*pptra)=23; // Setting a = 23
    ***ppptrb= **pptra+4; // setting b = a + 4

    print_string("pptra and a are: \n");
    print_int(**pptra);
    print_char('\n');
    print_int(a);
    print_char('\n');

    print_string("ppptrb and b are: \n");
    print_int(***ppptrb);
    print_char('\n');
    print_int(b);

    return 0;
}