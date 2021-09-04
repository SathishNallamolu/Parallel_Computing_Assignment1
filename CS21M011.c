#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generate_random(int);
static int non_zero;

/*

1. take order of both matrices
2. take sparsity of the matrices.(add non-zeros based on the sparsity.)
3. generate random locations in the matrix to fill with non-zeros. Fill zeros in the remaining locations.--> line no:160
4. convert the matrices into sparse.(to reduce the space needed to store.)
5. add the elements if they satisfy the compatibility.(we have to a00 with b00 only.) --> refer addtion at line no:195, 218
6. multiplication is a bit different from additon. a00*b00+a01*b10 has to be done. check those conditions correctly to avoid wrong answers.
   iterate first matrix row wise and second matrix column wise. then add the result place in mul matrix.--> line no:221.

*/


int main(){
    int m,n1,n2,p,sp1,sp2,i,j,k;
    int zerocount,row,col;

	//entering of ordering of both matrices
	printf("enter order first matrix:");
	scanf("%d %d",&m,&n1);
    printf("enter order second matrix:");
    scanf("%d %d",&n2,&p);

    if(n1!=n2){
        printf("matrices cant be multiplied\n");
    	return 0;
    }

	int a[m][n1];
	int b[n2][p];
	
	printf("enter sparsity matrices:");
 	scanf("%d %d",&sp1,&sp2);
	
	/* genarating matrix A */ 
	int nza=m*n1*sp1/100; //no. of positons for non-zeros in matrix A. (decided based on sparsity value of A)
	int za=m*n1-nza;     //no. of positions for zeros in matrix A.
	
	non_zero=0; //no. of positions to be filled with non-zeros in matrix A. Helps in knowing whether to generate random nums or not
    srand(time(NULL));
    for(i=0;i<m;i++){
        for(j=0;j<n1;j++){
        	// the generate_random function is to determine the location randomly to store non-zeros.
            if(generate_random(nza)){
            	a[i][j]=rand()%10;   //taking value from 0-9 only for simplicity
            	if(!a[i][j]){        
	        		while(1){
	        			a[i][j]=rand()%10;  //storing non-zero values only into matrix
	        			if(a[i][j]) break;
					}
				}
			}
            else
            	a[i][j]=0;
        }
    }
    
    
    /* genarating matrix B */ 
	int nzb=n1*p*sp2/100; //no. of positons for non-zeros in matrix B.(decided based on sparsity value of B)
	int zb=n1*p-nzb;     //no. of positions for zeros in matrix B.
	non_zero=0;    
    for(i=0;i<n2;i++){
        for(j=0;j<p;j++){
        	// the generate_random function is to determine the location randomly to store non-zeros.
        	if(generate_random(nzb)){
        	b[i][j]=rand()%10;
        	if(!b[i][j]){
        		while(1){
        			b[i][j]=rand()%10; //storing non-zero values only into matrix
        			if(b[i][j]) break; 
				}
			}     		
		}
        else
           	b[i][j]=0;
		}
    }
    
    //the below matrices are in spase form. it is our(human) view of matrices.
    printf("\nMATRIX A\n");
    for(i=0;i<m;i++){
        for(j=0;j<n1;j++)
            printf("%d ",a[i][j]);
        printf("\n");
    }
    printf("\nMATRIX B\n");
    for(i=0;i<n2;i++){
        for(j=0;j<p;j++)
            printf("%d ",b[i][j]);
        printf("\n");
    }
    
    
    //forming of sparse matrices
    int d1[nza][3];
	int d2[nzb][3];
    
	//forming sparse matrix of A.
	//every row of a sparse matrix contains 3 fields.(row,column,value). #rows=no. of non-zeros
	k=0;
    for(i=0;i<m;i++){
        for(j=0;j<n1;j++){
            if(a[i][j]){
                d1[k][0]=i;
                d1[k][1]=j;
                d1[k++][2]=a[i][j];
            }
        }
    }

    //forming sparse matrix of B
    int l=0;
    for(i=0;i<n2;i++){
        for(j=0;j<p;j++){
            if(b[i][j]){
                d2[l][0]=i;
                d2[l][1]=j;
                d2[l++][2]=b[i][j];
            }
        }
    }


    printf("\n SPARSE MATRIX OF A(CSR representation)\n");
    for(i=0;i<nza;i++){
        for(j=0;j<3;j++)
            printf("%d ",d1[i][j]);
        printf("\n");
    }

    printf("\n SPARSE MATRIX OF B(CSR representation)\n");
    for(i=0;i<nzb;i++){
        for(j=0;j<3;j++)
            printf("%d ",d2[i][j]);
        printf("\n");
    }
    
    // matrix addition
    int row_addition=nza+nzb;
    int c[row_addition][3];
	i=0,j=0;
	int cr=0,t1=0,t2=0;
	
	/* this loop adds sum of both elements into addition matrix.
		we are checking in both sparse matrices for non-zero values. 
		if a non-zero value comes in first matrix,then we check for the same element(same row,same column). if they match,we will add.
		if non-zero element only comes in first matrix,then at that location only that value is present. other elemnt is 0.
		the same case for secind matrix also.
	 */
	 
	l=0;
	while(i<nza && j<nzb){
		if(d1[i][2] && d2[j][2]){
			if(d1[i][0]==d2[j][0] && d2[i][1]==d2[j][1]){
                k=d1[i][2]+d2[j][2];
                c[l][0]=d1[i][0];   //row of element
                c[l][1]=d1[i][1];   //column of element
                c[l++][2]=k;       //placing the sum in the exact location
        	}
			i++; j++;   
			cr++;
        }
        //if the other element is zero,then the sum is the number itself.
        else if(d1[i][2]){
        	k=d1[i][2];
            c[l][0]=d1[i][0];
            c[l][1]=d1[i][1];
            c[l++][2]=k;
            i++;
            cr++;
		}
		else if(d2[j][2]){
            k=d2[j][2];
            c[l][0]=d2[j][0];
            c[l][1]=d2[j][1];
            c[l++][2]=k;
            j++;
            cr++;
		}
	}
	
	
	printf("\n SPARSE MATRIX OF Addition matrix\n");
    for(i=0;i<cr;i++){
        for(j=0;j<3;j++)
            printf("%d ",c[i][j]);
        printf("\n");
    }
		
	printf("\nMATRIX A+B\n");
	for(i=0;i<m;i++){
		for(j=0;j<n1;j++){
    		//this if conditon checks the matching of row and column of sparse matrix with actual matrix 
            if(c[k][0]==i && c[k][1]==j && c[k][2]){
                printf("%d", c[k][2]);
        		break;
			}
            else
            	printf("0 ");
        }
        printf("\n");
	}
	
	
	
	//multiplication
	/*
	    first we are checking whether the elements are compatible to multiplication or not.
	    because in multiplication, row elements of first matrix has to multiply with column elemnts of secnd matrix.
	    if the above condtion satisfoes,then onlt we have to multiply. otherwise ignore.
	*/
	
	l=0,cr=0,i=0,j=0;
	int mul[row_addition][3];
	while(i<nza && j<nzb){
		if(d1[i][2] && d2[j][2]){
			//checking for row and column compatibiliy
			if(d1[i][0]==d2[j][1] && d2[i][1]==d2[j][0]){
                k=d1[i][2]*d2[j][2];
                mul[l][0]=d1[i][0];   //row of element
                mul[l][1]=d1[i][1];   //column of element
                mul[l++][2]=k;       //placing the sum in the exact location
        	}
			i++; j++;   
			cr++;
        }
        //if the other eleemnt is not present,then it will be 0. so,the product will alos be zero.
        else if(d1[i][2]){
            mul[l][0]=d1[i][0];
            mul[l][1]=d1[i][1];
            mul[l++][2]=0;
            i++;
            cr++;
		}
		else if(d2[j][2]){
            mul[l][0]=d2[j][0];
            mul[l][1]=d2[j][1];
            mul[l++][2]=0;
            j++;
            cr++;
		}
	}
	
	printf("\n SPARSE MATRIX OF Multiplication matrix\n");
    for(i=0;i<cr;i++){
        for(j=0;j<3;j++)
            printf("%d ",mul[i][j]);
        printf("\n");
    }
		
	printf("\nMATRIX AxB\n");
	for(i=0;i<m;i++){
		for(j=0;j<n1;j++){
    		//this if conditon checks the matching of row and column of sparse matrix with actual matrix 
            if(mul[k][0]==i && mul[k][1]==j && mul[k][2]){
                printf("%d", mul[k][2]);
        		break;
			}
            else
            	printf("0 ");
        }
        printf("\n");
	}
}

//function for generating random locations
int generate_random(int n){
	int k=rand()%2;
	if(k){
		if(non_zero++<n)   //checking no. of non-zeros generated till now. if count exceeds,we fill remaining values with 0.
			return 1;
		return 0;
	}
	return 0;
}


/*
 
 in general,the matrix multiplication takes O(n^3) time incase of dense multiplication and  O(n^2) space.

 here in this case, it is taking O(n)+O(n) for conversion into sparse. O(n^3) for checking element compatibility,multiplying and 
 placing them in respective positions.

 But here the same complexity was reduced to a considerable amount. it takes O(n) only. 
 why because,we are storing only non-zero elemtnts. NOT the entire matrix.

*/