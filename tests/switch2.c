/// switch
void print_int(int);
void print_char(char );
void print_string(char *);
int scan_int();
int main()
{
   char y;
   char arr[10];
   int x;
   print_string("Enter the integer for switch case\n");
   x=scan_int();
   y = 'a'; 

    switch(x)
    {
       case 'a': x++;
                break;
       case 'b' :x+=2;
                break; 
    }
    print_int(x);
    return 0;

}