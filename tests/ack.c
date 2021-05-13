void print_int(int c);
void print_char(char c);
int scan_int();
void exit();
int ack(int m, int n)
{
    if (m == 0){
        return n+1;
    }
    if((m > 0) && (n == 0)){
        return ack(m-1, 1);
    }    
    if((m > 0) && (n > 0)){
        return ack(m-1, ack(m, n-1));
    }
    return 0;
}
  
int main(){
    int A,b,c;
    b=scan_int();
    c=scan_int();
    A = ack(b,c);
    print_int(A);
    return 0;
}