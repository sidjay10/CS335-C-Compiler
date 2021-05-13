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

    int fd,fd2;
    char buf[10];
    char *input;
    
    // file open and read
    input="README.d";
    // scan_string(input,8);
    fd=open_file(input,0,0);
    read_file(fd,buf, 10);
    close_file(fd);


    // print_string("Enter file to be opened name :\n");
    // scan_string(input,8);
    // // input="a.txt";
    // print_string(input);
    // print_char(input[6]);
    // fd=open_file(input,0,0);
    // print_int(fd);
    // read_file(fd,buf, 10);

    // file open(existing) and write
    fd2=open_file("b.txt",0x41,0x1ff);
    print_int(fd2);
    print_char('\n');
    write_file(fd2,"hello_world",9);
    
    // fd3=open_file("b.txt",1,0);

    // write_file(fd2)
    close_file(fd2);
    
    print_string(buf);
    return 0;
}