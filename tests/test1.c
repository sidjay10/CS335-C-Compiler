//#include <stdio.h>
//
void quick_sort (int * data, int n) 
    {
        int i, j, p, t;
        if (n < 2)
            return;
        p = data[n / 2];
        for (i = 0, j = n - 1;; i++, j--) {
            while (data[i] < p)
                i++;
            while (p < data[j])
                j--;
            if (i >= j)
                break;
            t = data[i];
            data[i] = data[j];
            data[j] = t;
        }
        quick_sort(data, i);
        quick_sort(data + i, n - i);
        return;
    }
    

//int main(void) {
//        int t; scanf("%ld",&t);
//        while(t--)
//        {
//            int c[3],k; 
//            scanf("%lld%lld%lld");
//            quick_sort(c+1,3);
//            scanf("%lld");
//            if(k<=c[0]) printf("%lld\n");
//            else if(k>c[0]&&k<=c[1]) printf("%lld\n");
//            else if(k>c[1]&&k<=c[2]) printf("%lld\n");
//        }
//    	return 0;
//    }
