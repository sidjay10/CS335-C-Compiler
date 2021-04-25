int my_max(int a,int b){
    if(a>b){
        // a=c;
        return a;
    }
    return b;
}
int main(){
    // int a
    long x,g;
    int e[5];
    int c = my_max(3,4,5);
    int var=2.5;
    float var2=2.4;
    int d= my_max(0,1L);
    int *ptr = &var;
    float *ptr2=&var2;
    ptr+=1;
    ptr+=2.2;
    if(c<d){
        int f=4;
    }
    // g=f;
    e[0][1]=4;
    var2=(float *)var;
    // printf("%d",x);
}

// syntax error description (if needed :: wsl)
// long int not working - not needed
// funcinon inner typecasting  - will see later
