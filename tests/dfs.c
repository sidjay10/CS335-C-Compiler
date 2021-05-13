//using matrices

void print_int(int c);
void print_char(char c);
void print_string(char *);
int scan_int();
// void DFS(int i, int G[10][10], int n, int* visited);
   //n is no of vertices and graph is sorted in array G[10][10]
 
void DFS(int ver, int G[10][10], int n, int* visited)
{
    int j;
	print_int(ver);
    visited[ver]=1;
	
	for(j=0;j<n;j++)
       if(visited[j]==0&&G[ver][j]==1)
            DFS(j, G, n, visited);
    return;
}

void main()
{
    int i,j;
    int G[10][10],visited[10],n; 
    print_string("Enter number of vertices:");
   
	n=scan_int();
 
    //read the adjecency matrix
	print_string("\nEnter adjecency matrix of the graph:");
   
	for(i=0;i<n;i++)
       for(j=0;j<n;j++)
		G[i][j]=scan_int();
 
    //visited is initialized to zero
   for(i=0;i<n;i++)
        visited[i]=0;
    print_string("\n DFS Traversal is\n");
    DFS(0, G, n, visited);
}
 
/*
n=5 
0 0 1 1 0
1 0 0 0 1
0 1 0 0 0
0 0 0 0 0 
0 0 0 0 0
*/
