void print_int(int c);
void print_char(char c);
int scan_int();

// return index at which the no. is present else -1
int binary_search(int * arr,int low, int high,int to_find){
    int mid,i;
    if(low>high){
        return -1;
    }
    else if(low==high){
        if(arr[low]==to_find){
            return low;
        }
        return -1;
    }
    else {
        mid = (low+high)/2;
        if(arr[mid]>to_find){
            return binary_search(arr,low,mid-1,to_find);
        }
        else if(arr[mid]==to_find){
            return mid;
        }
        else {
            return binary_search(arr,mid+1,high,to_find);
        }
    }
    return -1;
}
void print_array(int * arr){
    int x;
    for(x=0;x<50;x++){
        print_int(arr[x]);
        print_char(' ');
    }
    print_char('\n');
    return;
}
int main(){
    int i,x,y,z;
    int arr[100];
    x=10;
    for(i=0;i<50;i++){
        arr[i]=x;
        x+=i;
    }
    print_array(arr);
    z=scan_int();
    y=binary_search(arr,0,49,z);
    print_int(y);
    return 0;
}