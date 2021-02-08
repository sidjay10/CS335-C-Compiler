#include <stdio.h>

struct Point 
{ 
   int x, y; 
}; 

int main()
{
    struct Point p1={0,1},p2={1,2};
    p1.x=30,p2.x=45;
    printf("Point pi is (%d, %d)\n and Point p2 is (%d, %d)",p1.x,p1.y,p2.x,p2.y);
    return 0;
}
