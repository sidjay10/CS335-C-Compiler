// iteration statements

int main(){
     int t,a;
     int *x = &a;
    // simple while
    // while( a < 7 && a > 9 || t != 5 ){
    //     a=2;
    //     a*=5;
    //     if ( a > 9 ) {
    //         break;
    //     } else {
    //         continue;
    //     }
    //     a++;
    // }
    // t++;
    do {
        x++;
        if(a<4){
            goto error;
        }
        continue;
    }
    while(a<5);
    
    // for(a;a<5;){
    //     t^=3;
    //     return 0;
    // }
    for(a=1;a<5;a++){
        t+=4;
        t*=3;
    }
    goto error;
        return 0;

    error:
        return 1;
}