void print_int(int c);
void print_char(char c);
void print_string(char *);
void scan_string(char* str, int n);
int scan_int();
int strlen(char *str);
int str_index_of(char *str, char c);
int substr(char *str, char *search_str);
int strncpy(char *dest, char *src, int b);
int strcmp(char *first, char *second);
int strncmp(char *first, char *second, int n);
void memcpy(char *dest, char *src, int num);
void memset(char *buf, char b, int n);
int atoi(char *str);
int htoi(char *str);
void str_append(char *prefix, char *suffix, char *dest);
void str_nappend(char *prefix, char *suffix, char *dest, int n);

int main(){
    char *arr,*arr2,*arr3,*arr4,*arr5;
    char arr6[20],arr7[20],arr8[20],arr10[20];
    // char c;
    arr="hello_world";
    arr2="ell";

    print_string("\nTesting strlen\n");
    print_int(strlen(arr));
    // print_char(' ');
    print_string("\nTesting str_index_of\n");
    print_int(str_index_of(arr,'a'));
    // print_string(" ");

    print_string("\nTesting substr\n");
    print_int(substr(arr,arr2));
    
    // strncpy : Number of characters written out (including null byte).
    print_string("\nTesting strncpy\n");
    strncpy(arr6,arr,5);
    print_string(arr6);

    //strcmp arr6=hell, arr2 = ell
    print_int(strcmp(arr6,arr2));
    print_char('\n');
    print_int(strcmp(arr6,"hell"));

    //strncmp
    print_int(strncmp(arr6,arr2,4));
    print_char('\n');
    print_int(strncmp(arr6,arr,4));

    //memcpy
    print_string("\nTesting memcpy\n");
    memcpy(arr7,arr,3);
    print_string(arr);
    print_char('\n');
    print_string(arr7);

    //memset
    // c='a';
    // print_string("\nTesting memset\n");
    // memset(arr8,c,6);
    // print_string(arr8);

    //atoi
    print_string("\nTesting atoi\n");
    arr4="1023";
    print_int(atoi(arr4));

    //htoi
    print_string("\nTesting htoi\n");
    arr4="23";
    print_int(htoi(arr4));

    //str_append arr6=hell,arr hello
    print_string("\nTesting str_append\n");
    str_append(arr6,arr,arr8);
    print_string(arr8);

    // str_nappend 
    // print_string("\nTesting str_nappend\n");
    // print_string(arr6);
    // print_char('\n');
    // print_string(arr8);
    // str_nappend(arr6,arr2,arr10,15);
    // print_string(arr2);
}