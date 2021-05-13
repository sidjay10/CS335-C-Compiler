void print_int(int c);
void print_char(char c);
void exit();
int main(){
    int a,b,c,d;
    a = 2;
    b = -3;
    c = 5;
    d = 7;
    d=a+b; 
    print_int(d);
    print_char('\n'); 
    
    // uunary operators

    print_int(c); 
    print_char('\n');
    c = (-a) ;
    print_int(c);
    // print_char('\n');


    // bitwise operators : &, |, ^, <<, >>
    c = c^b;
    print_int(c);
    print_char('\n');
    c = c&a | (c|b);
    print_int(c);
    print_char('\n');
    c = 2>>1;
    print_int(c);
    print_char('\n');
    c = 2<<1;
    print_int( c);
    print_char('\n');
    c = c>>b;
    print_int(c);
    print_char('\n');




    // comparison operators, >, <, <=, >=

    c = (b >= a);
    print_int(c);
    print_char('\n');
    c = (b <= a);
    print_int(c);
    print_char('\n');
    c = (a < b);
    print_int(c);
    print_char('\n');
    c = (a > b);
    print_int(c);
    print_char('\n');
    // c = !c;
    // print_int(c);
    print_char('\n');
    c += 10*(-1);
    print_int(c);
    print_char('\n');
    c *= 15;
    print_int(c);
    print_char('\n');
    c /= 3;
    print_int(c);
    print_char('\n');
    c <<= 1;
    print_int(c);
    print_char('\n');
    c >>= 2;
    print_int(c);
    print_char('\n');
    c ^= 1000;
    print_int(c);
    print_char('\n');
    c &= 10;
    print_int(c);
    print_char('\n');
    c |= 15;
    print_int(c);
    print_char('\n');
    
   return 0;
} 
    

    
    // unary operators : -, ~
    // c = -c;
    // print_int(c);
    // print_char('\n');
    // // c = ~c;
    // print_int(c);
    
    /*
    // bitwise operators : &, |, ^, <<, >>
    // c = c^b;
    // print_int(c);
    c = c&a | (c|b);
    print_int(c);
    // c = c<<a;
    // print_int(c);
    print_int( a);
    // c = c>>a;
    // print_int(c);
    print_char('\n');

    // comparison operators, >, <, <=, >=
    c = (b >= a);
    print_int(c);
    print_char('\n');
    c = (b <= a);
    print_int(c);
    print_char('\n');
    c = (a < b);
    print_int(c);
    print_char('\n');
    c = (a > b);
    print_int(c);
    print_char('\n');
    // c = !c;
    // print_int(c);
    print_char('\n');
    c += 10*(-1);
    print_int(c);
    print_char('\n');
    c *= 15;
    print_int(c);
    print_char('\n');
    c /= 3;
    print_int(c);
    print_char('\n');
    // c <<= 1;
    // print_int(c);
    // print_char('\n');
    // c >>= 2;
    print_int(c);
    print_char('\n');
    c ^= 1000;
    print_int(c);
    print_char('\n');
    c &= 10;
    print_int(c);
    print_char('\n');
    c |= 15;
    print_int(c);
    print_char('\n');
    */
