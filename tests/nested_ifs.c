void print_int(int x);

int main()
{  int i;
i=100;
if (i<10)
    if(i<5)
        if(i==0)
            print_int(i+1);
else{
    if(i<20)
        i--;
    else{
        if(i<30)
            i=i-2;
        else
            print_int(i);
    }
}
    return 0;
}