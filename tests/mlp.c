void print_int( int x);
void print_char( char x);
int scan_int();

int main(){
    int a,b,c,x;
    int *ptra,*ptrb,*ptrc;
    int **pptra,**pptrb,**pptrc;
    a=71;
    b=57,c=b*a+34;

    ptra=&a,ptrb=&b,ptrc=&c;
    pptra=&ptra,pptrb=&ptrb;
    *(*pptra)=23;
    print_int(a);
    print_char('\n');
    x=scan_int();
    (*pptra)+=x;
    print_int(*ptra);
    return 0;
}