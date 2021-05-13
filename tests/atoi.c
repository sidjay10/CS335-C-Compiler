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
    char *arr;
    arr="14";
    print_int(htoi("14"));
}