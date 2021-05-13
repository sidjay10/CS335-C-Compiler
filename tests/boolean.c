void print_int(int x);
int main(){
    bool c,d;
    c=true;
    d=false;
    if(!c)
    {print_int(1);}
    if(d)
    {print_int(2);}
    if(c && !d){
        print_int(3);
    }
    if(!c || d){
        print_int(4);
    }
    return 0;
}