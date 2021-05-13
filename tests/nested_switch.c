void print_int(int x);
int scan_int();
void print_string(char*);
char scan_char();

int main(){
    int a;
    char b;
    a=scan_int();
    b=scan_char();
    
switch(a) {
   case 1: 
      print_string("This is part of outer switch" );
		
      switch(b) {
         case 'a':
            print_string("This A is part of Inner switch" );
            break;
         case 'b': 
         default : print_string("End of Inner switch");
      }
   case 2:
   default:  print_string("End of Nested Switch");
}
return 0;
}