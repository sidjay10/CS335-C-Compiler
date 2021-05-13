void print_int(int c);
void print_char(char c);
void print_string(char *);
int scan_int();
struct point{
    int x,y;
};
void foo(){
    int a,b;
    return;
}
int main(){
    int i,j,k,l,*ptr,**ptr2;
    const int a;
    struct point p;
    void *c;
    int arr[5];
    char d;
    // post_increment and decerement for cons int and struct
    a++;
    p.x=3,p.y=5;
    p--;
    // unary_increment and decerement for cons int and struct
    ++a;
    ++p;
    // function pointer not supported
    c=(&foo);
    // dereferencing int or an aaray
    *a=3;
    *arr=2;
    // unary + and - 
    j=i+(-ptr);

    // multiplicative
    k = (ptr * a)+(ptr/a);
    //relation
    if(a>ptr){
        j=10;
    }
    //and
    j=a&ptr;

    //assignment =
    ptr2=ptr;(//Warning)
    p=ptr;

    //*=
    ptr*=7;

    return 0;
}

