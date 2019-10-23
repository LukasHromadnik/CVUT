#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "memoryutilf.h"
#include "matrix_iof.h"
#define MAX_UNITS 1000

float **tr,**map,**a,**te;
float sigmax,sigmax_init,sigmax_fin,sigmay,sigmay_init,sigmay_fin,mode,rms_error;
float old_rms_error,te_error,wt,nmin,nmax;
int trwidth,trlength,info_result,it,i,j,it2,tewidth,telength,ivars;
int **coord,*win_list;
int mapdim,knots,total_units,itnum,itnum2,doneflag,first;
float update_batch5(),rms_data_weights(),update_fit4();
float predict_pl(),predict_plw(),rms_plw();
char trainfn[50],testfn[50],mapfn[50],afn[50],fitfn[50];
float **u,*b,**dd,*diag,*atb,*workspace;
float press,d,volx,voly,map_rms_error,m_knots,l,mupd;
float bestsx,bestscore,score,bestpress,bestrms,best_t,bestsx2;
float rms_error,**bestmap,*weight_list,sum,*best_weights;
int first_time,conserv,bestunits,bestunits2,lev;
int unit_param,adapt_scale,last_knots;

main()
{	

printf("Batch CTM program with linear mapping \n");
printf("* predictor variables scaled according to \n");
printf("  average sensitivity \n");
printf("* adaptive unit location \n");
printf("* automatic number of units \n");
printf("* meta-smoothing parameter \n");
 


printf("training input file (filename) :");
gets(trainfn);	
putchar('\n');


info_result=info_mat(trainfn,&trlength,&trwidth);
	if(info_result!=0 || trlength==0 || trwidth==0)
		{
		printf("Problem with reading in training set at line %d\n",info_result);
		exit(1);
		}
tr=matrix(1,trlength,1,trwidth);
get_mat(trainfn,tr,1,trlength,1,trwidth);

ivars=trwidth-1;


printf("testing input file (filename) :");
gets(testfn);	
putchar('\n');


info_result=info_mat(testfn,&telength,&tewidth);
	if(info_result!=0 || telength==0 || tewidth==0)
		{
		printf("Problem with reading in test set at line %d \n",info_result);
		exit(1);
		}
te=matrix(1,telength,1,tewidth);
get_mat(testfn,te,1,telength,1,tewidth);

/*
printf("map output file (filename) :");
gets(mapfn);	
putchar('\n');

printf("coef output file (filename) :");
gets(afn);	
putchar('\n');

printf(" linear fitted data output file (filename) :");
gets(fitfn);	
putchar('\n');


*/

sprintf(mapfn,"map");
sprintf(afn,"coef");
sprintf(fitfn,"fit1");

/* initialize map and coefs */

printf("number of map dimensions:");
scanf("%d",&mapdim);
putchar('\n');

printf("smoothness level (0-9) :");
scanf("%d",&conserv);
putchar('\n');

printf("number of units per dimension (0=automatic determination):");
scanf("%d",&unit_param);		
putchar('\n');

printf("adaptive scaling (1=yes, 0=no):");
scanf("%d",&adapt_scale);
putchar('\n');

/*
printf("max number of iterations for regression :");
scanf("%d",&itnum);
putchar('\n');
*/

sigmax_init=1.0/pow((double)trlength,(1.0/(double)mapdim));

getchar();

/* rescale smoothness factor */
l=conserv/9.0;

win_list=ivector(1,trlength);

/*working memory */
b=vector(1,trlength);
u=matrix(1,trlength,1,trwidth);
dd=matrix(1,ivars+1,1,ivars+1);
diag=vector(1,ivars+1);
atb=vector(1,ivars+1);
workspace=vector(1,ivars+1);
weight_list=vector(1,ivars);
best_weights=vector(1,ivars);


if(unit_param==0)
	{
	/* automatic units per dim selection */
		

	first_time=1;
	itnum=30;

	/* find best solution for first pass */
	/* Try 1,2,3,4,5 units per dim */
	for(knots=1; knots<=5; knots++)
		{
		printf("Using %d units per dimension, %d-dim. map\n",knots,mapdim);

		total_units=1;
		for (i=1; i<=mapdim; i++)
			total_units=total_units*knots;

		coord=imatrix(1,total_units,1,mapdim);
		create_coords(coord, mapdim, knots,total_units);

		map=matrix(1,total_units,1,ivars);
		create_map(map, mapdim, knots, ivars,total_units, coord);

		a=matrix(1,total_units,1,ivars+1);
		create_a(a,ivars,total_units);

		create_win_list(win_list,map,tr,ivars,total_units,trlength);

		for(i=1;i<=ivars;i++)
			weight_list[i]=1.0/((float)ivars);

		sigmax=knots*.5;
		update_map1(map,a,coord,tr,win_list,
			sigmax,mapdim,ivars,total_units,trlength);

		score=0.0;
		it=1;
		while(it<=itnum && score>=0.0) 	
			{
			sigmax=(float)knots*
				pow(sigmax_init*.5,((double)it/((double) itnum-1)));
			press=update_fit4(map,a,coord,tr,win_list,weight_list,sigmax,
			mapdim,ivars,total_units,trlength,u,b,dd,diag,workspace,atb);
			rms_error=rms_plw(map,a,weight_list,tr,
				ivars,total_units,trlength);


			if(press>=0.0)			
				score=l*press+(1.0-l)*rms_error;
			else
				score=-1.0;
	
			if((bestscore>score && score>=0.0) || first_time)
				{
				first_time=0;
				bestpress=press;
				bestrms=rms_error;
				bestscore=score;
				bestsx=sigmax;
				bestunits=knots;
				printf("**>");
				printf(" sigma=%f xv=%f rms=%f score=%f\n",
					sigmax,press,rms_error,score);
				}


			if(adapt_scale)
				{
				for(i=1;i<=ivars;i++)
					{
					weight_list[i]=0;
					for(j=1;j<=total_units;j++)
						weight_list[i] +=fabs(a[j][i+1]);
					}
				sum=0;
				for(i=1;i<=ivars;i++)
					sum +=weight_list[i];
			
				sum =1.0/sum;
				/*printf("w="); */
				for(i=1;i<=ivars;i++)
					{
					weight_list[i] = weight_list[i]*sum;
					/*printf("%f,",weight_list[i]); */
					}
				}
					
			/*printf("iteration=%d sigmax=%f ",it,sigmax);*/
			update_map1(map,a,coord,tr,win_list,
				sigmax,mapdim,ivars,total_units,trlength);
			it++;
			}

		free_imatrix(coord,1,total_units,1,mapdim);
		free_matrix(map,1,total_units,1,ivars);
		free_matrix(a,1,total_units,1,ivars+1);

		}
	printf("Best model in first pass:\n");
	printf("map dimension=%d\n",mapdim);
	printf("units per dimension=%d\n",bestunits);
	printf("total iterations=%d\n",itnum);
	printf("final sigma of neighborhood=%f\n",bestsx);
	printf("cross-validation error=%f\n",bestpress);
	printf("training set error=%f\n",bestrms);
	printf("model score=%f\n\n",bestscore);

	printf("second pass\n");
	itnum=30;

	/* find best solution for second pass */
	mupd=pow((double)MAX_UNITS,1.0/((double)mapdim));
	knots=mupd;
	lev=0;
	last_knots=bestunits;
	while ( lev<=10 && knots<=mupd && knots>5 && knots!=last_knots)
		{
		printf("%d: Using %d units per dimension\n",lev,knots);

		total_units=1;
		for (i=1; i<=mapdim; i++)
			total_units=total_units*knots;

		coord=imatrix(1,total_units,1,mapdim);
		create_coords(coord, mapdim, knots,total_units);

		map=matrix(1,total_units,1,ivars);
		create_map(map, mapdim, knots, ivars,total_units, coord);

		a=matrix(1,total_units,1,ivars+1);
		create_a(a,ivars,total_units);

		create_win_list(win_list,map,tr,ivars,total_units,trlength);

		for(i=1;i<=ivars;i++)
			weight_list[i]=1.0/((float)ivars);

		sigmax=knots*.5;
		update_map1(map,a,coord,tr,win_list,
			sigmax,mapdim,ivars,total_units,trlength);

		score=0.0;
		it=1;
		while(it<=itnum && score>=0.0) 	
			{
			sigmax=(float)knots*
				pow(sigmax_init*.5,((double)it/((double) itnum-1)));
			press=update_fit4(map,a,coord,tr,win_list,weight_list,sigmax,
				mapdim,ivars,total_units,trlength,u,b,dd,diag,workspace,atb);
			rms_error=rms_plw(map,a,weight_list,tr,
				ivars,total_units,trlength);
	
	
	
			if(press>=0.0)
				score=l*press+(1.0-l)*rms_error;
			else
				score=-1.0;
				
			if((best_t>score && score>=0.0)|| it==1)
				{
				best_t=score;
				bestsx2=sigmax;
				}

			if(bestscore>score && score>=0.0)
				{
				bestpress=press;
				bestrms=rms_error;
				bestscore=score;
				bestsx=sigmax;
				bestunits=knots;
				printf("**>");
				printf(" sigma=%f xv=%f rms=%f score=%f\n",
                                        sigmax,press,rms_error,score);
				}
			
	
	
	
			if(adapt_scale)
				{
				for(i=1;i<=ivars;i++)
					{
					weight_list[i]=0;
					for(j=1;j<=total_units;j++)
						weight_list[i] +=fabs(a[j][i+1]);
					}
				sum=0;
				for(i=1;i<=ivars;i++)
					sum +=weight_list[i];
			
				sum =1.0/sum;
				/*printf("w="); */
				for(i=1;i<=ivars;i++)
					{
					weight_list[i] = weight_list[i]*sum;
					/*printf("%f,",weight_list[i]); */
					}
				}
				
			/*printf("iteration=%d sigmax=%f ",it,sigmax);*/
			update_map1(map,a,coord,tr,win_list,
				sigmax,mapdim,ivars,total_units,trlength);
			it++;
			}

		free_imatrix(coord,1,total_units,1,mapdim);
		free_matrix(map,1,total_units,1,ivars);
		free_matrix(a,1,total_units,1,ivars+1);
		printf("best sigma=%f\n",bestsx2);
		last_knots=knots;
		knots=(float)knots/((float)bestsx2)*1.4567;
		lev++;

		}
	printf("Best model in second pass:\n");
	printf("map dimension=%d\n",mapdim);
	printf("units per dimension=%d\n",bestunits);
	printf("total iterations=%d\n",itnum);
	printf("final sigma of neighborhood=%f\n",bestsx);
	printf("cross-validation error=%f\n",bestpress);
	printf("training set error=%f\n",bestrms);
	printf("model score=%f\n\n",bestscore);

		
	
	}	
	
else

	bestunits=unit_param;	
	

/* determine final model */
knots=bestunits;
itnum=30;
score=0.0;
first_time=1;
it=1;
total_units=1;
for (i=1; i<=mapdim; i++)
	total_units=total_units*knots;

coord=imatrix(1,total_units,1,mapdim);
create_coords(coord, mapdim, knots,total_units);

map=matrix(1,total_units,1,ivars);
create_map(map, mapdim, knots, ivars,total_units, coord);
bestmap=matrix(1,total_units,1,ivars);
a=matrix(1,total_units,1,ivars+1);
create_a(a,ivars,total_units);

create_win_list(win_list,map,tr,ivars,total_units,trlength);

for(i=1;i<=ivars;i++)
	weight_list[i]=1.0/((float)ivars);

sigmax=knots*.5;
update_map1(map,a,coord,tr,win_list,
	sigmax,mapdim,ivars,total_units,trlength);

while(it<=itnum && score>=0.0) 	
	{
	sigmax=(float)knots*
		pow(sigmax_init*.5,((double)it/((double) itnum-1)));
	press=update_fit4(map,a,coord,tr,win_list,weight_list,sigmax,
	mapdim,ivars,total_units,trlength,u,b,dd,diag,workspace,atb);
	rms_error=rms_plw(map,a,weight_list,tr,
                ivars,total_units,trlength);


	if(press>=0.0)
                score=l*press+(1.0-l)*rms_error;
        else
                score=-1.0;		

	if((bestscore>score && score>=0.0) || first_time)
		{
		first_time=0;
		bestpress=press;
		bestscore=score;
		bestrms=rms_error;
		bestsx=sigmax;
		for(i=1;i<=total_units;i++)
			for(j=1; j<=ivars; j++)
				bestmap[i][j]=map[i][j];
		for(i=1;i<=ivars;i++)
			best_weights[i]=weight_list[i];
		printf("**>");
		printf(" model score=%f\n",score);
		}



	
	if(adapt_scale)
		{
			
		for(i=1;i<=ivars;i++)
			{
			weight_list[i]=0;
			for(j=1;j<=total_units;j++)
				weight_list[i] +=fabs(a[j][i+1]);
			}
		sum=0;
		for(i=1;i<=ivars;i++)
			sum +=weight_list[i];
		
		sum =1.0/sum;
		/*printf("w="); */
		for(i=1;i<=ivars;i++)
			{
			weight_list[i] = weight_list[i]*sum;
			/*printf("%f,",weight_list[i]); */
			}
		}
	/*printf("iteration=%d sigmax=%f ",it,sigmax);*/
	update_map1(map,a,coord,tr,win_list,
		sigmax,mapdim,ivars,total_units,trlength);
	it++;
	}

press=update_fit4(bestmap,a,coord,tr,win_list,best_weights,bestsx,
	mapdim,ivars,total_units,trlength,u,b,dd,diag,workspace,atb);
rms_error=rms_plw(bestmap,a,best_weights,tr,ivars,total_units,trlength);

printf("\n\n\nFINAL STATISTICS\n");
printf("training set:%s\n",trainfn);
printf("test set:%s\n",testfn); 
printf("\n\n\nBest model found using %d total iterations:\n",itnum);
printf("%d-dimensional map with %d units per dimension\n",mapdim,knots);
printf("cross-validation error=%f\n",bestpress);
printf("training set error=%f\n",bestrms);
printf("model score=%f\n\n",bestscore);

printf("neighborhood width (sigma)=%f\n",bestsx);
printf("variable     scaling\n");
for(i=1;i<=ivars;i++)
	printf("%d     %f\n",i,best_weights[i]);
putchar('\n');

	
printf(" predicting ...\n");

rms_error=predict_plw(bestmap,a,best_weights,te,ivars,total_units,telength);

printf("saving results ..\n");

put_mat(fitfn,te,1,telength,1,tewidth);
put_mat(mapfn,bestmap,1,total_units,1,ivars);
put_mat(afn,a,1,total_units,1,ivars+1);
free_matrix(te,1,telength,1,tewidth);
doneflag=0;
while(!doneflag)
	{
	printf("testing input file (filename) :");
	gets(testfn);	
	putchar('\n');
	if(strcmp(testfn,"done")==0)
		printf("OK\n");
	if(strcmp(testfn,"done")!=0)
		{
	
		info_result=info_mat(testfn,&telength,&tewidth);
			if(info_result!=0 || telength==0 || tewidth==0)
				{
				printf("Problem with test set \n");
				exit(0);
				}
		te=matrix(1,telength,1,tewidth);
		get_mat(testfn,te,1,telength,1,tewidth);
		predict_plw(bestmap,a,best_weights,te,ivars,total_units,telength);
		printf("fit output file (filename) :");
		gets(fitfn);	
		putchar('\n');
		put_mat(fitfn,te,1,telength,1,tewidth);
		free_matrix(te,1,telength,1,tewidth);
		doneflag=0;
		}
	else
		doneflag=1;
	}
			



	


printf("releasing memory ...\n");

free_matrix(tr,1,trlength,1,trwidth);

free_imatrix(coord,1,total_units,1,mapdim);
free_matrix(map,1,total_units,1,ivars);
free_matrix(bestmap,1,total_units,1,ivars);
free_matrix(a,1,total_units,1,ivars+1);
free_ivector(win_list,1,trlength);


free_matrix(dd,1,ivars+1,1,ivars+1);
free_vector(diag,1,ivars+1);
free_vector(atb,1,ivars+1);
free_vector(workspace,1,ivars+1);


free_vector(b,1,trlength);
free_matrix(u,1,trlength,1,trwidth);

free_vector(weight_list,1,ivars);
free_vector(best_weights,1,ivars);
exit(0);





	
	
	
	
	
}
