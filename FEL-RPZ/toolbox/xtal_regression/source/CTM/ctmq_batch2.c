#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "memoryutilf.h"
#define RAND_MAX 2147483647




/* CTM code */



void presskey()

{
printf("press any key:");
getchar();
putchar('\n');

}


/*	given a coordinate index, this function places the value of the next coordinate
	in the standard ordering.	*/
	
void next_coord(coords,dim,units_p_dim,index)

int **coords;
int dim,units_p_dim,index;

{

int i,carry;

carry=1;
for (i = 1;i<=dim; i++)
	{
	
	coords[index+1][i]=coords[index][i]+carry;
	carry=coords[index+1][i] / (units_p_dim+1);
	if(carry>0)
		coords[index+1][i]=1;
		
	}
	
}


/* this function fills a coord matrix using the specified parameters */

void create_coords(coords, dim, units_p_dim,total_units)

int **coords;
int dim,units_p_dim,total_units;

{

int i,j;
	
		for (j = 1; j <= dim; j++)
			coords[1][j]=1;	/* initialize first unit coord */
		
	for (i = 1; i<=total_units-1; i++)
		{
		next_coord(coords,dim,units_p_dim,i);
		}
}


void create_map(map, dim, units_p_dim, ivars,total_units, coords)

int dim,units_p_dim,ivars,total_units;
int **coords;
float **map;


{

	int i,j;
for (i = 1; i<= total_units; i++)
	{
	
	for ( j = 1; j<= dim; j++)
		map[i][j]=2*(coords[i][j]-(.5*units_p_dim)-.5)/(units_p_dim-1);
	
	for (j=dim+1; j<=ivars; j++)
		map[i][j]=0;

	/*
	for(j=1; j<=ivars; j++)
		printf("%f,",map[i][j]);
	putchar('\n');
	*/
	
	}
}

void create_a(a, ivars,total_units)

int ivars,total_units;
float **a;


{

	int i,j;
for (i = 1; i<= total_units; i++)
	for (j=1; j<=ivars+1; j++)
		a[i][j]=0;
	
	
}

void create_win_list(win_list,map,data,ivars,total_units,samples)
int *win_list,total_units,samples,ivars;
float **map,**data;

{
int i,find_winner();

for (i=1; i<=samples; i++)
	win_list[i]=find_winner(map,data,i,ivars,total_units);
}





/* this function finds the winning unit its a little faster */

int find_winner(map,data,point,ivars,total_units)

float **map,**data;
int point,ivars, total_units;

{
	int i,j,best;
	float sum,diff,bestd;
	
	best=1;
	bestd=0.0;
	
	for (j = 1; j <= ivars ; j++)
			{
			diff=map[1][j]-data[point][j];
			bestd=bestd+diff*diff;
			}
	for (i = 2; i<=total_units; i++)
		{
		sum=0;
		j=1;
		while(j <= ivars && sum <= bestd)
			{
			diff=map[i][j]-data[point][j];
			sum=sum+diff*diff;
			j++;
			}
		if(sum<bestd)
			{
			bestd=sum;
			best=i;
			}
		}
		
	return(best);
}



/* this function finds the winning unit with variable weighing its a little faster */

int find_winnerw(map,data,weights,point,ivars,total_units)

float **map,**data,*weights;
int point,ivars, total_units;

{
	int i,j,best;
	float sum,diff,bestd;
	
	best=1;
	bestd=0.0;
	
	for (j = 1; j <= ivars ; j++)
			{
			diff=map[1][j]-data[point][j];
			bestd=bestd+diff*diff*weights[j];
			}
	for (i = 2; i<=total_units; i++)
		{
		sum=0;
		j=1;
		while(j <= ivars && sum <= bestd)
			{
			diff=map[i][j]-data[point][j];
			sum=sum+diff*diff*weights[j];
			j++;
			}
		if(sum<bestd)
			{
			bestd=sum;
			best=i;
			}
		}
		
	return(best);
}





/* this routine finds the winner searching in neighborhood of map_winner from
last time */

int find_winner2(map,coords,data,map_winner,point,ivars,total_units,map_dim,nb)

float **map, **data;
int ivars,total_units,point,map_winner,**coords,nb,map_dim;

{
float bestd,diff,sum;

int i,j,k,ma=ivars+1,best,disti;


	
best=map_winner;
bestd=0.0;
	
for (j = 1; j <= ivars ; j++)
		{
		diff=map[best][j]-data[point][j];
		bestd=bestd+diff*diff;
		}

for(i=1; i<=total_units; i++)
	{
	
	/* check if neighbor to map_winner */
	disti=0;
	k=1;
	while(k <=map_dim && disti<=nb)
		{
		disti += abs(coords[map_winner][k]-coords[i][k]);
		k++;
		}
		
		
	if(disti<=nb)
		{
		sum=0;
		j=1;
		while(j <= ivars && sum <= bestd)
			{
			diff=map[i][j]-data[point][j];
			sum=sum+diff*diff;
			j++;
			}
		if(sum<bestd)
			{
			bestd=sum;
			best=i;
			}
		}
	}

return(best);

}







/* this routine finds the winner based on pred error in neighborhood of map_winner */

int find_winner_pe2(map,a,coords,data,point,ivars,map_winner,total_units,map_dim)

float **map,**a, **data;
int ivars,total_units,point,map_winner,**coords;

{
float d,bestf,diff,goodf,dist_a,dist_b,dist_c;

int i,j,k,ma=ivars+1,besti,diffi,disti;


diff=a[map_winner][1]-data[point][ma];
dist_b=0.0;
for(j=1; j<=ivars; j++)
	{
	diff += data[point][j]*a[map_winner][j+1];
	d=data[point][j]-map[map_winner][j];
	dist_b += d*d;
	}
	

goodf=bestf=fabs(diff);
besti=map_winner;


for(i=1; i<=total_units; i++)
	{
	
	/* check if neighbor to map_winner */
	disti=0;
	k=1;
	while(k <=map_dim && disti<2)
		{
		disti += abs(coords[map_winner][k]-coords[i][k]);
		k++;
		}
		
		
	if(disti<2)
		{
		dist_a=0.0;
		dist_c=0.0;
		diff=a[i][1]-data[point][ma];
		for(j=1; j<=ivars; j++)
			{
			diff += data[point][j]*a[i][j+1];
			d=map[map_winner][j]-map[i][j];
			dist_a += d*d;
			d=data[point][j]-map[i][j];
			dist_c += d*d;
			}
		diff=fabs(diff);
		
		if(diff<bestf && dist_b<dist_a && dist_c<dist_a)
			
			{
			bestf=diff;
			besti=i;
			/*printf("better\n");*/
			}
		}
	}

return(besti);

}




/* this function gives a weighing for a data point based on measuring the
second derivative */

float curve_weight(map,a,coords,data,point,ivars,map_winner,total_units,map_dim)

float **map,**a,**data;
int **coords,point,ivars,map_winner,total_units,map_dim;

{
float bestf,diffx,diffy,goodf,xsum,ysum,slopem;

int i,j,k,count,ma=ivars+1,besti,diffi,disti;



slopem=0;
count=0;
for(i=1; i<=total_units; i++)
	{
	
	/* check if neighbor to map_winner */
	disti=0;
	k=1;
	while(k <=map_dim && disti<=1)
		{
		disti += abs(coords[map_winner][k]-coords[i][k]);
		k++;
		}
		
		
	if(disti==1)
		{
		xsum=ysum=0;
		count++;
		for(j=2;j<=ma;j++)
			{
			diffy= a[i][j]-a[map_winner][j];
			/*diffx= map[i][j-1]-map[map_winner][j-1];
			xsum += diffx*diffx; */
			ysum += diffy*diffy;
			}
		/*slopem += ysum*ysum;*/
		slopem += sqrt(ysum);

		}
	}
/*slopem=atan(slopem/((float)count));*/
slopem=slopem/((float)count);
slopem=slopem*slopem;
return(slopem);

}



float error_weight(data,point,ivars,a,unit,dd,diag,atb,workspace)
float **data,**a,**dd,*diag,*atb,*workspace;
int point,ivars,unit;

{
float fact,fit1,e;

int i,j,k,ma=ivars+1;

atb[1]=1;
fit1=a[unit][1]-data[point][ma];
for(j=1; j<=ivars; j++)
	{
	fit1 += data[point][j]*a[unit][j+1];
	atb[j+1]=data[point][j];
	}
/*print_mat(dd,1,ma,1,ma);*/
cholsl(dd,ma,diag,atb,workspace);
fact=1;
for(j=1;j<=ma;j++)
	{
	fact -= workspace[j]*atb[j];
	/*printf("%f,",workspace[j]);*/
	}
/*putchar('\n');*/
if(fact<=0.0)
	e=-1.0;
else
	e=fit1/fact;
	
return(e);	

}



/* this function makes predictions, piecewise lin, original y in data is replaced
   and a weighing is used to find winning units */

float predict_plw(map,a,weights,data,ivars,total_units,samples)

float **map,**a, **data, *weights;
int ivars,total_units,samples;

{

int i,j,winner,ma=ivars+1;
float mean,std,rms_error,tmp;

mean=0;
for(i=1; i<=samples; i++)
	mean += data[i][ma];
mean=mean/((float)samples);

rms_error=0;
std=0;
for(i=1; i<=samples; i++)
	{
	tmp=data[i][ma];
	std += (tmp-mean)*(tmp-mean);
	winner=find_winnerw(map,data,weights,i,ivars,total_units);
	data[i][ma]=a[winner][1];
	for(j=1; j<=ivars; j++)
		data[i][ma] += data[i][j]*a[winner][j+1];
	rms_error += (data[i][ma]-tmp)*(data[i][ma]-tmp);
	}
std=sqrt(std/((float)samples));
rms_error=sqrt(rms_error/((float)samples));
printf("mean(y)=%f std(y)=%f rms error=%f ",mean,std,rms_error);
rms_error=rms_error/std;
printf("normalized rms err=%f\n",rms_error);
return(rms_error);

}

float rms_plw(map,a,weights,data,ivars,total_units,samples)

float **map,**a, **data, *weights;
int ivars,total_units,samples;

{

int i,j,winner,ma=ivars+1;
float rms_error,tmp;

rms_error=0;
for(i=1; i<=samples; i++)
        {
        winner=find_winnerw(map,data,weights,i,ivars,total_units);
        tmp=a[winner][1]-data[i][ma];
        for(j=1; j<=ivars; j++)
                tmp += data[i][j]*a[winner][j+1];
        rms_error += tmp*tmp;
        }
rms_error=sqrt(rms_error/((float)samples));
return(rms_error);

}

/* this function makes predictions, piecewise lin, original y in data is replaced */

float predict_pl(map,a,data,ivars,total_units,samples)

float **map,**a, **data;
int ivars,total_units,samples;

{

int i,j,winner,ma=ivars+1;
float mean,std,rms_error,tmp;

mean=0;
for(i=1; i<=samples; i++)
	mean += data[i][ma];
mean=mean/((float)samples);

rms_error=0;
std=0;
for(i=1; i<=samples; i++)
	{
	tmp=data[i][ma];
	std += (tmp-mean)*(tmp-mean);
	winner=find_winner(map,data,i,ivars,total_units);
	data[i][ma]=a[winner][1];
	for(j=1; j<=ivars; j++)
		data[i][ma] += data[i][j]*a[winner][j+1];
	rms_error += (data[i][ma]-tmp)*(data[i][ma]-tmp);
	}
std=sqrt(std/((float)samples));
rms_error=sqrt(rms_error/((float)samples));
printf("mean=%f std=%f err=%f ",mean,std,rms_error);
rms_error=rms_error/std;
printf("serr=%f\n",rms_error);
return(rms_error);

}







/* this function makes predictions, piecewise const, original y in data is replaced */

void predict_pc(map,a,data,ivars,total_units,samples)

float **map,**a, **data;
int ivars,total_units,samples;

{

int i,j,winner,ma=ivars+1;





for(i=1; i<=samples; i++)
	{
	winner=find_winner(map,data,i,ivars,total_units);
	data[i][ma]=a[winner][1];
	for(j=1; j<=ivars; j++)
		data[i][ma] += map[winner][j]*a[winner][j+1];
	}



}






void som(map,coords,data,win_list,sigmax,dim,ivars,u_p_dim,total_units,samples)

float **map,**data,sigmax;
int **coords,*win_list;
int dim,ivars,total_units,samples,u_p_dim;


{
	int k,j,i;
	int winner,dist,diff,ndist;
	float weightx,weight_sumx,factorx;
		
	
	factorx=-.5/(sigmax*sigmax);	
	ndist=2*sigmax*sigmax;
	if (ndist<2)
		ndist=2;
	if(ndist<u_p_dim)
		for(k=1; k<=samples; k++)
			win_list[k]=find_winner2(map,coords,data,win_list[k],
				k,ivars,total_units,dim,ndist);
	else
		for(k=1; k<=samples; k++)
			win_list[k]=find_winner(map,data,k,ivars,total_units);
	
	for(j=1; j<=total_units; j++)
		{
		
		/* zero unit */
		for(k=1; k<=ivars; k++)
			map[j][k]=0;
		
		weight_sumx=0;
		
		for(i=1; i<=samples; i++)
			{
			
			dist=0;
			winner=win_list[i];
			for (k=1; k<=dim; k++)
				{
				diff=coords[winner][k]-coords[j][k];
				dist=dist+diff*diff;
				}
			weightx=exp(dist*factorx);
			if(weightx>.01)
				{
				weight_sumx += weightx;
				for(k=1; k<=ivars; k++)
					map[j][k] += data[i][k]*weightx;
				}
				
			}
		/* normalize map unit */
		weight_sumx =1.0/weight_sumx;
		for(k=1; k<=ivars; k++)
			map[j][k] = map[j][k]*weight_sumx;
			
		
		}	
}




/*      this function performs 2 step CTM in batch mode with local lin. fits
        The map has units x1,x2,..,xn  and
        the 0th and 1st order coef are stored in a1,a2,..,an+1
*/

        /* For fixed map positions, find optimal linear fits with neighborhood
           cutoff  uses faster Cholesky decomp */


float update_fit3(map,a,coords,data,win_list,sigmay,
        dim,ivars,total_units,samples,u,b,dd,diag,workspace,atb)

float **map,**data,**a,*b,**u,**dd,*diag,*workspace,*atb,sigmay;
int **coords,*win_list;
int dim,ivars,total_units,samples;


{

        int i,j,k,p,winner,dist,diff,s_count;
        float factory,c,distf,sum,d;
        int d_count;
        float weight,rms_error,error_weight();
        float wmax,thresh,press;
        int ma=ivars+1;

        factory=-.5/(sigmay*sigmay);

        /* For fixed map positions, find optimal linear fits */
        /* get winner list */

        for(i=1; i<=samples; i++)
                win_list[i]=winner=find_winner(map,data,
                        i,ivars,total_units);

        /*determine weights for regression for each unit */

        press=0;
        s_count=0;
        for(j=1; j<=total_units; j++)
                {
                p=1;
                for(i=1; i<=samples; i++)
                        {

                        dist=0;
                        winner=win_list[i];
                        for (k=1; k<=dim; k++)
                                {
                                diff=coords[winner][k]-coords[j][k];
                                dist=dist+diff*diff;
                                }

                        weight=exp(dist*factory);
                        if(weight>.01)
                                {
                                u[p][1]=weight;
                                for(k=1; k<=ivars; k++)
                                        u[p][k+1]=data[i][k]*weight;
                                b[p]=data[i][ivars+1]*weight;
                                p++;
                                }

                        }
                if(p>ivars+1)
                        regress_ch2(u,b,a[j],p-1,ivars,dd,diag,atb);
                sum=0;
                k=0;
                for(i=1;i<=samples;i++)
                        {
                        if(win_list[i]==j)
                                {
                                distf=error_weight(data,i,ivars,a,j,dd,diag,atb,workspace);
                                if(distf>=0.0)
                                        {
                                        sum += distf*distf;
                                        k++;
                                        }
                                }
                        }
                press += sum;
                s_count += k;

                }
        if(s_count*4>samples)
                press=sqrt(press/((float)s_count));
        else
                press=-1.0;

return(press);

}












/*	this function performs 2 step CTM in batch mode with local lin. fits
	The map has units x1,x2,..,xn  and
	the 0th and 1st order coef are stored in a1,a2,..,an+1
	Adaptive scaling is done */
	
	/* For fixed map positions, find optimal linear fits with neighborhood
	   cutoff  uses faster Cholesky decomp */
	

float update_fit4(map,a,coords,data,win_list,weight_list,sigmay,
	dim,ivars,total_units,samples,u,b,dd,diag,workspace,atb)

float **map,**data,**a,*b,**u,**dd,*diag,*workspace,*atb,*weight_list,sigmay;
int **coords,*win_list;
int dim,ivars,total_units,samples;


{

	int i,j,k,p,winner,dist,diff,s_count;
	float factory,c,distf,sum,d;
	int d_count;
	float weight,rms_error,error_weight();
	float wmax,thresh,press;
	int ma=ivars+1;
	
	factory=-.5/(sigmay*sigmay);
		
	/* For fixed map positions, find optimal linear fits */
	/* get winner list */
		
	for(i=1; i<=samples; i++)
		win_list[i]=winner=find_winnerw(map,data,weight_list,
			i,ivars,total_units);
		
	/*determine weights for regression for each unit */
		
	press=0;
	s_count=0;
	for(j=1; j<=total_units; j++)
		{
		p=1;	
		for(i=1; i<=samples; i++)
			{
			
			dist=0;
			winner=win_list[i];
			for (k=1; k<=dim; k++)
				{
				diff=coords[winner][k]-coords[j][k];
				dist=dist+diff*diff;
				}
			
			weight=exp(dist*factory);
			if(weight>.01)
				{
				u[p][1]=weight;
				for(k=1; k<=ivars; k++)
					u[p][k+1]=data[i][k]*weight;
				b[p]=data[i][ivars+1]*weight;
				p++;
				}
			
			}
		if(p>ivars+1)
			regress_ch2(u,b,a[j],p-1,ivars,dd,diag,atb);
		sum=0;
		k=0;
		for(i=1;i<=samples;i++)
			{
			if(win_list[i]==j)
				{
				distf=error_weight(data,i,ivars,a,j,dd,diag,atb,workspace);
				if(distf>=0.0)
					{
					sum += distf*distf;
					k++;
					}
				}
			}
		press += sum;
		s_count += k;
		
		}
	if(s_count*4>samples)
		press=sqrt(press/((float)s_count));
	else
		press=-1.0;

return(press);		
		
}


/*      this function performs CTM in batch mode with local lin. fits
        The map has units x1,x2,..,xn  and
        the 0th and 1st order coef are stored in a1,a2,..,an+1
*/



void update_map(map,a,coords,data,win_list,sigmax,dim,ivars,total_units,samples)

float **map,**data,**a,sigmax;
int **coords,*win_list;
int dim,ivars,total_units,samples;


{
        int i,j,k,winner,dist,diff,time_count;
        float factorx,c,distf,sum,ypred,d,weight_sumx;
        float weightx,tot_error,tot_error2,rms_error,rms_data_weights();

        factorx=-.5/(sigmax*sigmax);

        for(j=1; j<=total_units; j++)
                {

                /* zero unit */
                for(k=1; k<=ivars; k++)
                        map[j][k]=0;

                weight_sumx=0;

                for(i=1; i<=samples; i++)
                        {

                        dist=0;
                        winner=win_list[i];
                        for (k=1; k<=dim; k++)
                                {
                                diff=coords[winner][k]-coords[j][k];
                                dist=dist+diff*diff;
                                }

                        weightx=exp(dist*factorx);
                        weight_sumx += weightx;

                        for(k=1; k<=ivars; k++)
                                map[j][k] += data[i][k]*weightx;

                        }

                /* normalize map unit */
                weight_sumx =1.0/weight_sumx;
                for(k=1; k<=ivars; k++)
                        map[j][k] = map[j][k]*weight_sumx;
                }


}








/*	this function performs CTM in batch mode with local lin. fits
	The map has units x1,x2,..,xn  and
	the 0th and 1st order coef are stored in a1,a2,..,an+1
	map optimization is done based on prediction error */
	
	
/*	For fixed linear fits, find optimal map positions among neighbors of map
	winner */


void update_map1(map,a,coords,data,win_list,sigmax,dim,ivars,total_units,samples)

float **map,**data,**a,sigmax;
int **coords,*win_list;
int dim,ivars,total_units,samples;


{
	int i,j,k,winner,dist,diff,time_count;
	float factorx,c,distf,sum,ypred,d,weight_sumx;
	float weightx,tot_error,tot_error2,rms_error,rms_data_weights();
	
	factorx=-.5/(sigmax*sigmax);	
	
	/* For fixed linear fits, find optimal map positions among neighbors of map
	winner */
			
		
	for(i=1; i<=samples; i++)
		win_list[i]=find_winner_pe2(map,a,coords,data,
			i,ivars,win_list[i],total_units,dim);
		
	for(j=1; j<=total_units; j++)
		{
		
		/* zero unit */
		for(k=1; k<=ivars; k++)
			map[j][k]=0;
		
		weight_sumx=0;
			
		for(i=1; i<=samples; i++)
			{
			
			dist=0;
			winner=win_list[i];
			for (k=1; k<=dim; k++)
				{
				diff=coords[winner][k]-coords[j][k];
				dist=dist+diff*diff;
				}
				
			weightx=exp(dist*factorx);
			weight_sumx += weightx;
			
			for(k=1; k<=ivars; k++)
				map[j][k] += data[i][k]*weightx;
				
			}
				
		/* normalize map unit */
		weight_sumx =1.0/weight_sumx;
		for(k=1; k<=ivars; k++)
			map[j][k] = map[j][k]*weight_sumx;
		}	

		
}




/*	this function performs CTM in batch mode with local lin. fits
	The map has units x1,x2,..,xn  and
	the 0th and 1st order coef are stored in a1,a2,..,an+1
	Two neighborhood widths are used for X and for Y
	map optimization is done based on prediction error */
	
	
/*	For fixed linear fits, find optimal map positions among neighbors of map
	winner */


float update_map2(map,a,coords,data,win_list,weight_list,sigmax,dim,ivars,total_units,samples,mode)

float **map,**data,**a,sigmax,*weight_list,mode;
int **coords,*win_list;
int dim,ivars,total_units,samples;


{

	int i,j,k,winner,dist,diff,time_count;
	float factorx,c,distf,sum,ypred,d,weight_sumx;
	float weightx,tot_error,tot_error2,rms_error,rms_data_weights(),curve_weight();
	float mean,std,maxw,minw;
		
	
	factorx=-.5/(sigmax*sigmax);	
	
	/* For fixed linear fits, find optimal map positions among neighbors of map
	winner */
			
	
	
	mean=0;
	maxw=0;
	minw=0;	
	for(i=1; i<=samples; i++)
		{
		win_list[i]=find_winner_pe2(map,a,coords,data,i,ivars,win_list[i],total_units,dim);
		/*weightx=curve_weight(map,a,coords,data,i,ivars,win_list[i],total_units,dim);
		weight_list[i] *= weightx;
		mean += weight_list[i];
		if(i==1 || maxw<weight_list[i])
			maxw=weight_list[i];
		if(i==1 || minw>weight_list[i])
			minw=weight_list[i];
		*/	
		/*printf("W%f,",weight_list[i]);*/
		}
	/*
	mean=mean/((float)samples);
	std=0;
	for(i=1; i<=samples; i++)
		std +=(weight_list[i]-mean)*(weight_list[i]-mean);
	std=sqrt(std/((float)samples));
	printf("mean=%f std=%f max=%f min=%f\n",mean,std,maxw,minw);
	*/
	/*	
	putchar('\n');
	presskey();	*/
	for(j=1; j<=total_units; j++)
		{
		
		/* zero unit */
		for(k=1; k<=ivars; k++)
			map[j][k]=0;
		
		weight_sumx=0;
	

		
			
		for(i=1; i<=samples; i++)
			{
			
			dist=0;
			winner=win_list[i];
			for (k=1; k<=dim; k++)
				{
				diff=coords[winner][k]-coords[j][k];
				dist=dist+diff*diff;
				}
			
			
			
				
			/*weightx=exp(dist*factorx)*((1-mode)+mode*(weight_list[i]-minw)/(maxw-minw));*/
			
			weightx=exp(dist*factorx);
			
			weight_sumx += weightx;
			
			
			
			for(k=1; k<=ivars; k++)
				map[j][k] += data[i][k]*weightx;
				
			}
			
			
				
		/* normalize map unit */
		weight_sumx =1.0/weight_sumx;
		for(k=1; k<=ivars; k++)
			map[j][k] = map[j][k]*weight_sumx;
			
		
		}	
	
	
	
	rms_error=rms_data_weights(map,a,data,ivars,total_units,samples);
		

return(rms_error);		
		
}







/*	this function performs something like LVQ in batch mode with local lin. fits
	The map has units x1,x2,..,xn  and
	the 0th and 1st order coef are stored in a0,a2,..,an
	Two neighborhood widths are used for X and for Y
	map optimization is done based on prediction error */
	
	
/*	For fixed linear fits, find optimal map positions among neighbors of map
	winner */


float lvq_map(map,a,coords,data,wt,dim,ivars,total_units,samples)

float **map,**data,**a,wt;
int **coords;
int dim,ivars,total_units,samples;


{

	int i,j,k,winner,dist,diff,time_count;
	float c,distf,sum,ypred,d,weight_sumx;
	int *win_list,*lose_list,d_count,*win_count;
	float weightx,lose_weightx,*coef,tot_error,rms_error,rms_data_weights();
	
	
	
	win_list=ivector(1,samples);
	lose_list=ivector(1,samples);
	
	for(i=1;i<=total_units; i++)
		win_count[i]=0;
		
	
	


	/* For fixed linear fits, find optimal map positions among neighbors of map
	winner */
	
	
			
	
	d_count=0;
	tot_error=0;
	for(i=1; i<=samples; i++)
		{
		winner=find_winner(map,data,i,ivars,total_units);
		lose_list[i]=winner;
		win_count[winner]++;
		ypred=a[winner][0];
		for(j=1; j<=ivars; j++)
			ypred += data[i][j]*a[winner][j];
		distf=ypred-data[i][ivars+1];
		tot_error += (distf*distf);
		
		
		win_list[i]=find_winner_pe2(map,a,coords,data,i,ivars,winner,total_units,dim);
		if(win_list[i]!=winner)
			d_count++;	
		}
		
		
	tot_error=sqrt(tot_error/((float)samples));

	printf("shifts=%d\n",d_count);
	for(j=1; j<=total_units; j++)
		{
		/*
		presskey();
		printf("Unit %d\n",j);
		*/
		
		
		
		/* zero unit 
		for(k=1; k<=ivars; k++)
			map[j][k]=0;
		*/
		
		weight_sumx=1;
	

		
		/*
		printf("weight list=\n");
		*/	
		for(i=1; i<=samples; i++)
			{
			
			dist=0;
			winner=lose_list[i];
			
			if(winner ==j)
				if(win_list[i]!=winner)
					{
					
					weight_sumx -= wt;
					
					/* move away */
					for(k=1; k<=ivars; k++)
						map[j][k] += -data[i][k]*wt;
					}
				else
			
					{
					weight_sumx += wt;
					/* move towards */
					for(k=1; k<=ivars; k++)
						map[j][k] += data[i][k]*wt;
					}
				
			}
			winner=win_list[i];
			if(winner==j && lose_list[i]!=winner)
				{
				weight_sumx += wt;
				/* move towards */
				for(k=1; k<=ivars; k++)
					map[j][k] += data[i][k]*wt;
				}
				
			
		/* normalize map unit */
		weight_sumx =1.0/weight_sumx;
		for(k=1; k<=ivars; k++)
			map[j][k] = map[j][k]*weight_sumx;
			
		
		
		
		}	
	
	
	
	rms_error=rms_data_weights(map,a,data,ivars,total_units,samples);
	printf("Update map error after lvq =%f ",rms_error);
	if(rms_error<=tot_error)
		printf("down %f\n",tot_error-rms_error);
	else
		printf("up %f\n",rms_error-tot_error);	

		
	free_ivector(win_list,1,samples);
	free_ivector(lose_list,1,samples);

	
		

return(rms_error);		
		
}












    
/* this function returns the  rms error given data and units */

float rms_data_weights(map,a,data,ivars,total_units,samples)			

float **map,**a, **data;
int ivars,total_units,samples;

{

int i,j,winner,ma=ivars+1;
float sum,dist,diff,mean,std;

sum=0;
for(i=1; i<=samples; i++)
	sum += data[i][ma];
mean=sum/((float)samples);

sum=0;
dist=0;
for(i=1; i<=samples; i++)
	{
	winner=find_winner(map,data,i,ivars,total_units);
	diff=a[winner][1]-data[i][ma];
	for(j=1; j<=ivars; j++)
		diff += data[i][j]*a[winner][j+1];
	sum += diff*diff;
	diff=data[i][ma]-mean;
	dist += diff*diff;
	
	}
	
	/*mean=sqrt(sum/dist);*/
	mean=sqrt(sum/((float)samples));

	return(mean);
	

}


/* this function makes predictions, piecewise lin, original y in data is replaced
   and a weighing is used to find winning units */

float rms_data_weightsw(map,a,weights,data,ivars,total_units,samples)

float **map,**a, **data, *weights;
int ivars,total_units,samples;

{

int i,j,winner,ma=ivars+1;
float rms_error,diff;
rms_error=0;
for(i=1; i<=samples; i++)
        {
        winner=find_winnerw(map,data,weights,i,ivars,total_units);
        diff=a[winner][1]-data[i][ma];
        for(j=1; j<=ivars; j++)
                diff += data[i][j]*a[winner][j+1];
        rms_error += diff*diff;
        }
rms_error=sqrt(rms_error/((float)samples));
return(rms_error);

}






/* simple random number generator */

int randomi(min,max)

int min,max;

{

return(rand()/(RAND_MAX/(max-min+.5))+min);
}




/* Sample program
main()
{
int **crd;
double **map;

crd=imatrix(0,16,0,1);
map=matrix(0,16,0,3);
create_coords(crd, 2, 4,16);
create_map(map,2,4,4,16,crd);
free_imatrix(crd,0,16,0,1);
free_matrix(map,0,16,0,3);

}
*/
