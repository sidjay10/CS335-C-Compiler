void print_int( int x);
void print_char( char x);
int scan_int();

void swap(int *xp, int *yp)
{
    int temp;
    temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
/* bubble sort */
void bubble_sort(int *arr, int n)
{
   int i, j;
   for (i = 0; i < n-1; i++)     
 
       // Last i elements are already in place  
       for (j = 0; j < n-i-1; j++)
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
    return;
}
 
/*print an array */
void print_array(int *arr, int size)
{
    int i;
    for (i=0; i < size; i++){
        print_int(arr[i]);
        print_char(' ');
    }
    print_char('\n');
}
 
int main()
{
    int arr[50];
    int n, i;
    // printf("Enter the number of elements: ");
    n=scan_int();
    // printf("Enter the array elements\n");
    for(i = 0; i<n; i++){
    	arr[i]=scan_int();
    }

    print_array(arr, n);

    bubble_sort(arr, n);
    
    // printf("Sorted array: \n");
    print_array(arr, n);
    
    return 0;
}