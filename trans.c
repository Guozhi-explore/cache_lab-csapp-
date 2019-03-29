/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 

/******************郭志 517021910503***********************/

//s = 5, E = 1, b = 5  S=32 B=32   e=1 not lru

#include <stdio.h>
#include "cachelab.h"


int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,ii,jj,o0,o1,o2,o3,o4,o5,iii,jjj;
    if(N==32&&M==32)
    {
        for(ii=0;ii<N/8;++ii)                                       //8*8分块，用了8个变量防止对角线处A、B来回访问时不断冲突
        {
            for(jj=0;jj<M/8;++jj)
            {
                for(i=0;i<8;++i)
                {
                    o0=A[8*ii+i][8*jj+0];
                    o1=A[8*ii+i][8*jj+1];
                    o2=A[8*ii+i][8*jj+2];
                    o3=A[8*ii+i][8*jj+3];
                    o4=A[8*ii+i][8*jj+4];
                    o5=A[8*ii+i][8*jj+5];
                    iii=A[8*ii+i][8*jj+6];
                    jjj=A[8*ii+i][8*jj+7];
                    B[8*jj+0][8*ii+i]=o0;
                    B[8*jj+1][8*ii+i]=o1;
                    B[8*jj+2][8*ii+i]=o2;
                    B[8*jj+3][8*ii+i]=o3;
                    B[8*jj+4][8*ii+i]=o4;
                    B[8*jj+5][8*ii+i]=o5;
                    B[8*jj+6][8*ii+i]=iii;
                    B[8*jj+7][8*ii+i]=jjj;
                }
            }
        }
    }
    if(N==64&&M==64)
    {
        for(iii=0;iii<N/8;++iii)
        {
            for(jjj=0;jjj<M/8;++jjj)
            {
                for(i=0;i<4;++i)
                {   
                    o0=A[8*iii+i][8*jjj+0];
                    o1=A[8*iii+i][8*jjj+1];
                    o2=A[8*iii+i][8*jjj+2];                                            //8*8分块，A的上面4*8全部移动到B的上面4*8
                    o3=A[8*iii+i][8*jjj+3];                                            //移动同时两个4*4分别转置，这样冲突8次
                    o4=A[8*iii+i][8*jjj+4];                                            //b右上一行四次放入变量中，从a左下竖着拿四个变量放入b空出来的四个中，冲突4次
                    o5=A[8*iii+i][8*jjj+5];                                            //继续上面的步骤三次，均不再次发生冲突，至此a左下转置b右上，
                    ii=A[8*iii+i][8*jjj+6];                                            //最后a右下转动至b右下
                    jj=A[8*iii+i][8*jjj+7];
                    B[8*jjj+0][8*iii+i]=o0;
                    B[8*jjj+1][8*iii+i]=o1;
                    B[8*jjj+2][8*iii+i]=o2;
                    B[8*jjj+3][8*iii+i]=o3;
                    B[8*jjj+0][8*iii+i+4]=o4;
                    B[8*jjj+1][8*iii+i+4]=o5;
                    B[8*jjj+2][8*iii+i+4]=ii;
                    B[8*jjj+3][8*iii+i+4]=jj;
                }
                for(i=0;i<4;++i)
                {
                    o0=B[jjj*8+i][8*iii+4];
                    o1=B[jjj*8+i][8*iii+5];
                    o2=B[jjj*8+i][8*iii+6];
                    o3=B[jjj*8+i][8*iii+7];
                    o4=A[iii*8+4+0][8*jjj+i];
                    o5=A[iii*8+4+1][8*jjj+i];
                    ii=A[iii*8+4+2][8*jjj+i];
                    jj=A[iii*8+4+3][8*jjj+i];
                    B[jjj*8+i][8*iii+4]=o4;
                    B[jjj*8+i][8*iii+5]=o5;
                    B[jjj*8+i][8*iii+6]=ii;
                    B[jjj*8+i][8*iii+7]=jj;
                    B[jjj*8+4+i][8*iii+0]=o0;
                    B[jjj*8+i+4][8*iii+1]=o1;
                    B[jjj*8+i+4][8*iii+2]=o2;
                    B[jjj*8+i+4][8*iii+3]=o3;
                } 
                for(i=0;i<4;++i)
                {   
                    o0=A[8*iii+i+4][8*jjj+4];
                    o1=A[8*iii+i+4][8*jjj+5];
                    o2=A[8*iii+i+4][8*jjj+6];
                    o3=A[8*iii+i+4][8*jjj+7];
                    B[8*jjj+4][8*iii+i+4]=o0;
                    B[8*jjj+5][8*iii+i+4]=o1;
                    B[8*jjj+6][8*iii+i+4]=o2;
                    B[8*jjj+7][8*iii+i+4]=o3;
                }  
            }
        }
    }
    if(N==67&&M==61)
    {
        for(iii=0;iii<N/16+1;++iii)
        {
            for(jjj=0;jjj<M/16+1;++jjj)
            {
                for(i=0;i<16&&i+16*iii<N;++i)
                {
                    for(j=0;j<16&&j+16*jjj<M;++j)
                    {
                        B[jjj*16+j][16*iii+i]=A[16*iii+i][16*jjj+j];
                    }
                }
            }
        }
    }

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char transpose_submit_desc1[] = "Transpose submission";
void transpose_submit1(int M, int N, int A[N][M], int B[M][N])
{
   int i,j,ii,jj,array[4];
   for(ii=0;ii<N/4;++ii)
   {
    for(jj=0;jj<M/4;++jj)
    {
        if(ii==jj)
        {
            for(i=0;i<4;++i)
            {
                for(j=0;j<4;++j)
                {
                    array[j]=A[4*ii+i][4*jj+j];
                }
                for(j=0;j<4;++j)
                {
                    B[4*jj+j][4*ii+i]=array[j];
                }
            } 
        }
        else{
        for(i=0;i<4;++i)
        {
            for(j=0;j<4;++j)
            {
                B[jj*4+j][ii*4+i]=A[ii*4+i][jj*4+j];
            }
        }
        }
    }
   }
}
/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc); 
    //registerTransFunction(transpose_submit1, transpose_submit_desc1); 
    //registerTransFunction(transpose_submit2, transpose_submit_desc2); 
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

