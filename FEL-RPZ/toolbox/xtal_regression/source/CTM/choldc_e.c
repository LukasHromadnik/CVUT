/* This was taken from the new version of the book */

#include <math.h>

int choldc_e(a,n,p)

float **a,*p;
int n;

{

int i,j,k;
float sum;

for(i=1;i<=n;i++)
	{
	for(j=i;j<=n;j++)
		{
		
		for(sum=a[i][j],k=i-1;k>=1;k--)
			sum -= a[i][k]*a[j][k];
			
		if(i == j)
			{
			if(sum <=0.0)
				return(1);
			p[i]=sqrt(sum);
			}
		else
			a[j][i]=sum/p[i];
		}
	}
return(0);
}

