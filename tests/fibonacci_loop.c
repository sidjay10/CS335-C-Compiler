int print_int(int i);
int scan_int();

int fib(int n) {
    int x,a1,a2,temp;
	if ( n == 1 ) {
		return 0;
	}
    else{
        
        if( n == 2 ){
            return 1;
        }
        else{
            a1=0,a2=1;
            for(x=0;x<n-2;x++){
                temp=a1;
                a1=a2;
                a2=temp+a2;
            }
            return a2;
        }
    } 
    return n;

}

int main() {
	int x;
	x = scan_int();
	x = fib(x);
	print_int(x);
	return 0;
}
