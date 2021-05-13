void print_int(int c);
void print_char(char c);
void print_string(char *);
void scan_string(char* str, int n);
int scan_int();
int open_file(char *name, int flags, int mode);
int read_file(int fd, char *buf, int max_length);
int write_file(int fd, char *buf, int len);
void close_file(int fd);

int main(){

    int fd,fd2,k,n;
    char buf[50];
    char input[20];
    
    // file open and read
    print_string("Enter file name from which to be read: ");
    scan_string(input,20);
    fd=open_file(input,0,0);
    // print_string("Enter no. of characters(<50) to be read: ");
    // n=scan_int();
    while(read_file(fd,buf,50)!=0){
        print_string(buf);
    }
    close_file(fd);
    
    
    return 0;
}