int main()
{
	int t; 
	scanf("%d",&t);
	while(t--)
	{
    	int a,b;
    	int i; 
	long  ** s=0,za=a;
    	scanf("%d %d",&a,&b);
	for(i=a;i<=b;i++)
    	{
        	int reverse=0;
		int rem;
		int temp=i;
        	while(temp!=0)
            {
                rem=temp%10;
                reverse=reverse*10+rem;
                temp/=10;
            }
            if(reverse==i) s+=i;
        }
    	printf("%ld\n",s);
    }
    return 0;
}
