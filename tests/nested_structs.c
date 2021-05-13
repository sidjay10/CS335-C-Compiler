void print_int(int c);
void print_char(char c);
int scan_int();
struct point_2d{
    int x,y;
};
struct point_3d{
    struct point_2d x;
    int z;
};
int main(){
    int a,b,c;
    struct point_2d f,g;
    struct point_3d h,i;
    f.x=4,f.y=3;
    h.x.x=7;
    h.x.y=31;
    h.z=5;
    print_int(h.x.x);
    print_int(h.x.y);
    print_int(h.z);
}