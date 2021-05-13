void print_int( int x);
void print_char( char x);
int scan_int();
void print_string(char *);
//for printing matrix 
void print_mat(int mat[10][10], int row, int col){
    int i,j;
    for(i=0;i<row;i++){    
        for(j=0;j<col;j++){    
            print_int(mat[i][j]);
            print_char(' ');
        }    
        print_char('\n');
    }  
    return ;
}
int main(){  
    int a[10][10],b[10][10],mul[10][10],row,col,i,j,k;    
    // printf("enter the number of row=");    
    row=5;    
    // printf("enter the number of column=");    
    col=5;    
    // printf("enter the first matrix element=\n");    
    
    for(i=0;i<row;i++){    
        for(j=0;j<col;j++){    
            a[i][j]=i+1;
        }    
    }    
    // printf("enter the second matrix element=\n");    
    for(i=0;i<row;i++){    
        for(j=0;j<col;j++){    
            b[i][j]=j+1;
        }    
    }    
    print_string("A\n");
    print_mat(a,row,col);
    print_string("B\n");
    print_mat(b,row,col);
    
    // printf("multiply of the matrix=\n");    
    for(i=0;i<row;i++){    
        for(j=0;j<col;j++){    
            mul[i][j]=0;    
            for(k=0;k<col;k++){    
                mul[i][j]+=a[i][k]*b[k][j];    
            }    
        }    
    }
    print_string("C\n");    
    print_mat(mul,row,col);      
    return 0;  
}  