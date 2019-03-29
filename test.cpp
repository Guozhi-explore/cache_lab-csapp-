#include<iostream>
using namespace std;
void transpose_submit2(int M, int N, int A[][64], int B[][64])
{
    int i,ii,jj,o0,o1,o2,o3,o4,o5,iii,jjj;
    if(N==32&&M==32)
    {
        for(ii=0;ii<N/8;++ii)
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
                    o2=A[8*iii+i][8*jjj+2];
                    o3=A[8*iii+i][8*jjj+3];
                    o4=A[8*iii+i][8*jjj+4];
                    o5=A[8*iii+i][8*jjj+5];
                    ii=A[8*iii+i][8*jjj+6];
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
}
int main()
{
	int A[64][64],B[64][64];
	for(int i=0;i<64;++i)
	{
		for(int j=0;j<64;++j)
		{
			A[i][j]=64*i+j;
		}
	}
	transpose_submit2(64,64,A,B);
	for(int i=0;i<64;++i)
	{
		for(int j=0;j<64;++j)
		{
			cout<<B[j][i]<<" ";
		}
		cout<<endl;
	}
	return 0;
}

