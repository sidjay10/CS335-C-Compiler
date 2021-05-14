void print_int(int c);
void print_char(char c);
int scan_int();
void print_string(char *);

int MAT[50][50],A[50][100],B[100][50];
int main(){
    int i,j,k;
    for(i=0;i<50;i++){
        for(j=0;j<100;j++){
            A[i][j]=i+1;
        }
    }
    for(i=0;i<100;i++){
        for(j=0;j<50;j++){
            B[i][j]=j+1;
        }
    }
    for(i=0;i<50;i++){    
        for(j=0;j<50;j++){    
            MAT[i][j]=0;    
            for(k=0;k<100;k++){    
                MAT[i][j]+=A[i][k]*B[k][j];    
            }    
        }    
    }
    print_string("Printing first row of matrix");
    for(i=0;i<50;i++){
        print_int(MAT[0][i]);
        print_char(' ');
    }

    return 0;
}