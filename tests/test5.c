int max(int num1, int num2) {
   /* Foo Bar**/
   int result;
 
   if (num1 > num2){
      result = num1;
   }
   /* This is a multi-
    line comments ****
    *********
    */ 
   else
      result = num2;
 
    // this is different type of comment.	
   return result; 
}

int main()
{
    int x=4,y=5;
    printf("The maximum of x and y is %d",max(x,y));
    return 0;
}
/*****
***
****
********/
//
