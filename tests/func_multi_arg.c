void print_int(int c);
void print_char(char c);
int scan_int();
int funct(int a, int b, int c, int d, int e, int f, int g, int h, int i){
    int z,x,y;
    z = a*b*c;
    x=  c*d*e;
    y= f*g*h;
    return x*y+z;
}
int f10(int a, int b, int c, int d, int e,
    int f, int g, int h, int i, int j) {
  return a+b+c+d+e+f+g+h+i+j;
}



int main(){
    int a,b,c,d,e,f,g,h,i,j,k,l;
    a=1,b=2,c=3,d=4,e=5,f=6,g=7,h=8,i=9;
    j=funct(a,b,c,d,e,f,g,h,i);
    print_int(j);
    print_char('\n');
    print_int(f10(a,b,c,d,e,f,g,h,i,j));
    
    return 0;
}