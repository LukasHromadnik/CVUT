/* these functions performs some matrix input output */



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "memoryutilf.h"





/*	This function returns the sizes of a matrix in a file.
	The function returns the resulting integer:
		-1	file can't be opened
		0	no errors
		>0	line number in file where error occurred */


int info_mat(fn, length, width)

char *fn;
int *width,*length;


{
	
	FILE *in_fle;
	int endline,nothing,nonumbers,spcflag,varnum,c;
	int rownum,firstvarnum,badpt,i,j,line_num;
	float **mat;
	
	*width=0;
	*length=0;
	if((in_fle=fopen(fn,"r")) == NULL)
		return(-1);	/* no file exists */
	
	/* find the number of variables */
	
	rownum=0;
	firstvarnum=0;
	badpt=0;
	line_num=0;
	
	while(c != EOF && !badpt)
		{
		varnum=0;
		nonumbers=1;
		endline=0;
		spcflag=1;
		while (c != EOF && (nonumbers || !endline ))
			{
			c=fgetc(in_fle);
			
			nothing=1;
			endline=0;
			if (c=='\n')	/* end of line */
				{
				endline=1;
				line_num++;
				}
			else if((c>='0' && c<='9')||c =='.' || c == '-' || c == '+'  ) 	/* number */
			
				{
				if (spcflag)	/* space before */
					++varnum;
				endline=0;
				spcflag=0;
				nonumbers=0;
				nothing=0;
				}
				
			else if(c !='e')	/* delimiter or character */
			
				{
				endline=0;
				spcflag=1;
				nothing=0;
				}
				
				
			}
			if (firstvarnum==0 && varnum>0)	/* first row found */
				{
				firstvarnum=varnum;
				++rownum;
				}
			else if (firstvarnum==varnum && varnum>0) /*another row found */
				++rownum;
			else if (firstvarnum!=varnum && varnum>0) /*a bad row found */
				badpt=1;
				
			
		}
	printf("num of vars is %d\n",firstvarnum);
	printf("num of rows is %d\n",rownum);
	printf("bad pt=%d\n",badpt);
		
	fclose(in_fle);
	if (badpt)
		{
		*width=firstvarnum;
		*length=rownum+1;
		return(line_num);
		}
		
	*width=firstvarnum;
	*length=rownum;

	return(0);
}

/* this function retreives a matrix */


void get_mat(fn,m, ilength, flength, iwidth,fwidth)

char *fn;
int ilength,flength,iwidth,fwidth;
float **m;


{
	
	FILE *in_fle;
	int c;
	int i,j;
	

	in_fle=fopen(fn,"r");
	for(i = ilength; i <= flength; i++)
		{
    	for(j = iwidth; j <= fwidth; j++)
      		{
      		c=0;
      		while(!((c>='0' && c<='9')|| c =='.' || c=='-' || c=='+'))
      			c=fgetc(in_fle);
      		
      		ungetc(c,in_fle);
      		fscanf(in_fle,"%f",&m[i][j]);
      		/*printf("%f ",m[i][j]);*/ 
      			
			}
		}
	fclose(in_fle);
	

}
	

/* this function saves a matrix */

int put_mat(fn,m,rsize1,rsize2,csize1,csize2)
char *fn;
float **m;
int rsize1,rsize2,csize1,csize2;
{
	int i,j;
	FILE *out_fle;
	
	out_fle=fopen(fn,"w");
	
	for (i = rsize1; i <= rsize2; i++)
	{
		for (j = csize1; j < csize2; j++)
			fprintf(out_fle,"%f\t ",m[i][j]);
		fprintf(out_fle,"%f\n",m[i][csize2]);
	}
	fclose(out_fle);
}

/* this function appends a matrix */

int app_mat(fn,m,rsize1,rsize2,csize1,csize2)
char *fn;
float **m;
int rsize1,rsize2,csize1,csize2;
{
	int i,j;
	FILE *out_fle;
	
	out_fle=fopen(fn,"a+");
	
	for (i = rsize1; i <= rsize2; i++)
	{
		for (j = csize1; j < csize2; j++)
			fprintf(out_fle,"%f\t ",m[i][j]);
		fprintf(out_fle,"%f\n",m[i][csize2]);
	}
	fclose(out_fle);
}



/* these functions prints  matrices */

void print_mat(m,rsize1,rsize2,csize1,csize2)
float **m;
int rsize1,rsize2,csize1,csize2;
{
	int i,j;
	
	putchar('{');
	for (i = rsize1; i <= rsize2; i++)
	{
		putchar('{');
		for (j = csize1; j < csize2; j++)
			{
			printf("%f,  ",m[i][j]);
			}
		printf("%f}\n",m[i][csize2]);
	}
	putchar('}');
}

void print_imat(m,rsize1,rsize2,csize1,csize2)
int **m;
int rsize1,rsize2,csize1,csize2;
{
	int i,j;
	
	putchar('{');
	for (i = rsize1; i <= rsize2; i++)
	{
		putchar('{');
		for (j = csize1; j < csize2; j++)
			printf("%d  ",m[i][j]);
		printf("%d}\n",m[i][csize2]);
	}
	putchar('}');
}


void print_ivect(m,rsize1,rsize2)
int *m;
int rsize1,rsize2;
{
	int i;
	
	putchar('{');
	for (i = rsize1; i < rsize2; i++)
		printf("%d  ",m[i]);
	printf("%d}\n",m[rsize2]);
	
}

void print_vect(m,rsize1,rsize2)
float *m;
int rsize1,rsize2;
{
	int i;
	
	putchar('{');
	for (i = rsize1; i < rsize2; i++)
		printf("%f  ",m[i]);
	printf("%f}\n",m[rsize2]);
	
}



void get_scale(m,rsize1,rsize2,csize1,csize2,mean_vect,spread_vect)
float **m,*mean_vect,*spread_vect;
int rsize1,rsize2,csize1,csize2;
{
	int i,j;
	
	

	for(j=csize1; j<=csize2; j++)
		{
		mean_vect[j]=0;
		spread_vect[j]=0;
		for (i= rsize1; i<=rsize2 ; i++)
		
			{
			mean_vect[j]=mean_vect[j]+m[i][j];
			spread_vect[j]=spread_vect[j]+(m[i][j]*m[i][j]);
			/*printf("m=%f, s=%f\n",mean_vect[j],spread_vect[j]);*/
			}
		
		mean_vect[j]=mean_vect[j]/(rsize2-rsize1+1);
		spread_vect[j]=spread_vect[j]/(rsize2-rsize1+1);
		spread_vect[j]=sqrt(spread_vect[j]-(mean_vect[j]*mean_vect[j]));
		}
}

void scale_mat(m,rsize1,rsize2,csize1,csize2,mean_vect,spread_vect)
float **m,*mean_vect,*spread_vect;
int rsize1,rsize2,csize1,csize2;
{
	int i,j;
	
	

	for(j=csize1; j<=csize2; j++)
		for (i= rsize1; i<=rsize2 ; i++)
		
			m[i][j]=(m[i][j]-mean_vect[j])/spread_vect[j];
			
}

void unscale_mat(m,rsize1,rsize2,csize1,csize2,mean_vect,spread_vect)
float **m,*mean_vect,*spread_vect;
int rsize1,rsize2,csize1,csize2;
{
	int i,j;
	
	

	for(j=csize1; j<=csize2; j++)
		for (i= rsize1; i<=rsize2 ; i++)
		
			m[i][j]=(m[i][j]*spread_vect[j])+mean_vect[j];
			
}




/* Sample program
 main()

{
int width,length,res,i,j;
float **matr;
res=info_mat("test.dat",&length,&width);
if(res==0)
	{
	matr=matrix(0,length-1,0,width-1);
	get_mat("test.dat",matr,0,length-1,0,width-1);
	put_mat("test2.dat",matr,0,length-1,0,width-1);
	free_matrix(matr,0,length-1,0,width-1);
	}
else
	{
	printf("error=%d\n",res);
	}
	



}

*/

