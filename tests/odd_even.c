void print_int( int x);
void print_char( char x);
int scan_int();
int absolute(int x){
    if(x < 0){
        return -1*x;
    }
    return x;
}
int odd(int number);
int even(int number);

int main() {
  int T,n; 
  T = scan_int();
  while (T) {
    T = T-1; 
    n = scan_int();
    print_char('\n');
    // prints("odd: ");
    print_int(odd(n));
    // prints("even: ");
    print_char('\n');
    print_int(even(n));
    print_char('\n');
  }
  return 0;
}

int even(int number) {    
  if (number == 0){
    return 1;
  }
  else
    return odd(absolute(number)-1);
}

int odd(int number) {
  if (number == 0){
    return 0;
  }
  else
    return even(absolute(number)-1);
} 