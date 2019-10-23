#include "memoryutilf.h"
#include <stdio.h>

void regress_fit(data,a,ndata,ivar)
/* data is data matrix, a is the coefficients (first is 0th), ndata is number
of samples, ivar is number of variables x  in d the values of y are replaced
with the fitted values */

float **data,*a;
int ndata,ivar;

{

int i,j,ma=ivar+1;

for(i=1; i<=ndata; i++)
	{
	data[i][ma]=a[1];
	for(j=2; j<=ma; j++)
		data[i][ma] += data[i][j-1]*a[j];
	}
	
}



void regress_ch(data,b,a,ndata,ivar)
/* data is data matrix, weight is a vector of weights for the data,
a is returned coefficients (first is 0th), ndata is number
of samples, ivar is number of variables x  in d 
uses faster Cholesky decomp*/

float **data,*a,*b;
int ndata,ivar;

{
int p=ivar+1,j,i,k;
float **d,sum,*diag,*atb;

d=matrix(1,p,1,p);
diag=vector(1,p);
atb=vector(1,p);


/*
d[1][1]=ndata;

for(j=1;j<=ivar; j++)
	{
	sum=0;
	for(k=1;k<=ndata;k++)
		sum += data[k][j];
	d[1][j+1]=sum;
	}

*/

for(i=1; i<=p; i++)
	for(j=i;j<=p;j++)
		{
		sum=0;
		for(k=1;k<=ndata;k++)
			sum +=data[k][j]*data[k][i];
		d[i][j]=sum;
		}

for(i=1;i<=p; i++)
	{
	sum=0;
	for(k=1;k<=ndata;k++)
		sum += data[k][i]*b[k];
	atb[i]=sum;
	}

/*
sum=0;
for(k=1;k<=ndata;k++)
	sum += data[k][p];
b[1]=sum;

for(i=1;i<=ivar; i++)
	{
	sum=0;
	for(k=1;k<=ndata;k++)
		sum += data[k][p]*data[k][i];
	b[i+1]=sum;
	}
	
*/		
/*print_mat(d,1,p,1,p);	
printf("\n");
*/	
choldc_e(d,p,diag);
/*
print_mat(d,1,p,1,p);
printf("\n");

for(k=1;k<=p;k++)
	{
	printf("%d %f  \n",k,diag[k]);
	}
printf("\n");

presskey();
*/
cholsl(d,p,diag,atb,a);

free_matrix(d,1,p,1,p);
free_vector(diag,1,p);
free_vector(atb,1,p);
}		


void regress_ch2(data,b,a,ndata,ivar,d,diag,atb)
/* data is data matrix, weight is a vector of weights for the data,
a is returned coefficients (first is 0th), ndata is number
of samples, ivar is number of variables x  in d 
uses faster Cholesky decomp*/

float **data,*a,*b,**d,*diag,*atb;
int ndata,ivar;

{
int p=ivar+1,j,i,k;
float sum;



/*
d[1][1]=ndata;

for(j=1;j<=ivar; j++)
	{
	sum=0;
	for(k=1;k<=ndata;k++)
		sum += data[k][j];
	d[1][j+1]=sum;
	}

*/

for(i=1; i<=p; i++)
	for(j=i;j<=p;j++)
		{
		sum=0;
		for(k=1;k<=ndata;k++)
			sum +=data[k][j]*data[k][i];
		d[i][j]=sum;
		}

for(i=1;i<=p; i++)
	{
	sum=0;
	for(k=1;k<=ndata;k++)
		sum += data[k][i]*b[k];
	atb[i]=sum;
	}

/*
sum=0;
for(k=1;k<=ndata;k++)
	sum += data[k][p];
b[1]=sum;

for(i=1;i<=ivar; i++)
	{
	sum=0;
	for(k=1;k<=ndata;k++)
		sum += data[k][p]*data[k][i];
	b[i+1]=sum;
	}
	
*/		
/*print_mat(d,1,p,1,p);	
printf("\n");
*/	
choldc_e(d,p,diag);
/*
print_mat(d,1,p,1,p);
printf("\n");

for(k=1;k<=p;k++)
	{
	printf("%d %f  \n",k,diag[k]);
	}
printf("\n");

presskey();
*/
cholsl(d,p,diag,atb,a);

}		
			

float add_point(r_bar,diag,theta_bar,weight,drow,ivars)

float *r_bar,*diag,*theta_bar,*drow;
int ivars;

{


float ss_error=0.0,c_bar,s_bar,xi,xk,di,d_prime_i;
int i,k,next_r;

if(weight!=0.0)
	{
	for(i=1;i<=ivars;i++)
		{
		if(drow[i]!=0)
			{
			xi = drow[i];
			di = diag[i];
			d_prime_i=di+weight*xi*xi;
			c_bar=di/d_prime_i;
			s_bar=weight*xi/d_prime_i;
			weight=c_bar*weight;
			diag[i]=d_prime_i;
			next_r=(i-1)*(2*ivars-i)/2+1;
			for(k=i+1;k<=ivars;k++)
				{
				xk=drow[k];
				drow[k]=xk-xi*r_bar[next_r];
				r_bar[next_r]=c_bar*r_bar[next_r]+s_bar*xk;
				next_r++;
				}
			xk=drow[ivars+1];
			drow[ivars+1]=xk-xi*theta_bar[i];
			theta_bar[i]=c_bar*theta_bar[i]+s_bar*xk;
			}
		}
	ss_error=weight*drow[ivars+1]*drow[ivars+1];
	}	

return(ss_error);
}


void step_regress(r_bar,theta_bar,beta,ivars)
float *r_bar,*theta_bar,*beta;
int ivars;

{
int i,k,next_r;

for(i=ivars;i<=1;i--)
	{
	beta[i]=theta_bar[i];
	next_r=(i-1)*(2*ivars-i)/2+1;
	for(k=i+1;k<=ivars;k++)
		{
		beta[i] = beta[i]-r_bar[next_r]*beta[k];
		next_r++;
		}
	}
}






void wregress_ch(data,weight,a,ndata,ivar)
/* data is data matrix, weight is a vector of weights for the data,
a is returned coefficients (first is 0th), ndata is number
of samples, ivar is number of variables x  in d 
uses faster Cholesky decomp*/

float **data,*a,*weight;
int ndata,ivar;

{
int p=ivar+1,j,i,k;
float **d,*b,sum,*diag;

d=matrix(1,p,1,p);
b=vector(1,p);
diag=vector(1,p);

sum=0;

for(k=1;k<=ndata;k++)
	sum += weight[k]*weight[k];
d[1][1]=sum;

for(j=1;j<=ivar; j++)
	{
	sum=0;
	for(k=1;k<=ndata;k++)
		sum += weight[k]*data[k][j];
	d[1][j+1]=sum;
	}



for(i=1; i<=ivar; i++)
	for(j=i;j<=ivar;j++)
		{
		sum=0;
		for(k=1;k<=ndata;k++)
			sum +=data[k][j]*data[k][i]*weight[k]*weight[k];
		d[i+1][j+1]=sum;
		}

sum=0;
for(k=1;k<=ndata;k++)
	sum += weight[k]*data[k][p];
b[1]=sum;

for(i=1;i<=ivar; i++)
	{
	sum=0;
	for(k=1;k<=ndata;k++)
		sum += data[k][p]*data[k][i];
	b[i+1]=sum;
	}
	
		
	
		
	
choldc_e(d,p,diag);

cholsl(d,p,diag,b,a);

free_matrix(d,1,p,1,p);
free_vector(b,1,p);
free_vector(diag,1,p);
}		
			
