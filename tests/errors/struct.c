void print_int(int c);
void print_char(char c);
void print_string(char *);
int scan_int();

struct point{
    int x,y;
};
struct point3d;
int main(){
    struct point a;
    struct point *p;
    a.z=4;
    p.x=4;
    a->z=3;
    p->z=7;

}