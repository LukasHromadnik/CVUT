/*****************************************************************************/
/*    XTAL - A Multimethod Program for Multivariate Regression               */
/*                                                                           */
/*           Don Gehring - April 14,1994 - Version 5.1                       */
/*****************************************************************************/



/* ------------------------------------------------------------------------ */
/* These parameters can be changed to alternate settings                    */
#define SPLINE 1   /* spline parameter for MARS */
#define RETRIES 9  /* simulated annealling retries for ANN */
#define ITERATIONS 50  /* iteration parameter for CTM */
char ANNINITIALIZATION[] = "ANNEAL NOREGRESS"; /*  ANN init procedure */
/* ------------------------------------------------------------------------ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

double pardub ( char **str ) ;
int cntcol( char **str ) ;
void star_bar_plot(FILE *pfile, double y , double yscale );
int lookup_cmd(char *cmd);
int one_param(int *a);
int two_param( int *a, int *b);
void ann1();
void ctm1();
void mars1();
void ppr1();
void knn1();

char parameter_line[241];
char *para_ptr;
char trainname[16],testname[16] ;
int trn_samp_num,tst_samp_num,predx ;
char out_file_name[] = "rms.out";
char maxerr_name[] = "max.out" ;
double response_mean_rms,response_mean_variance;
double tst_diff,tst_mean ;
double trn_min,trn_max,tst_min,tst_max ;
double trnx_min[11],tstx_min[11],trnx_max[11],tstx_max[11];
double response_min,response_max,scale ;
double xmin[11],xmax[11],scalex[11];
char temptrainname[] = "tempor.trn";
char temptestname[] = "tempor.tst";
char tem2trainname[] = "tempor2.trn";
char tem2testname[] = "tempor2.tst" ;


int main()
{
  char filenamecontrol[] = "filename.con" ;
  char operationcontrol[] = "operate.con" ;
  int i,j,k,n;
  char *str ;
  char **pstr ;
  double mse_sum,rms,y,ytemp,y_est,diff,x ;
  int flag ,command_num;
  char line[241] , command[5];
  FILE *trainfile , *testfile , *outfile ,*maxerr;
  FILE *namefile , *operfile;
  double response_sum,response_mean,response;
  FILE *temptrainfile, *temptestfile;
  FILE *tem2trainfile, *tem2testfile;
  char s[30] ;
  /* create file to write program output into
     this file is first opened and reclosed after use to 
     allow the user to examine it during program execution.
     Its contents allows the user to determine how far the 
     program is from completion*/
  outfile = fopen(out_file_name, "w");
  fclose(outfile);
  /* create file to write the worst case error values into
     this file is first opened and reclosed to erase it */
  maxerr = fopen(maxerr_name, "w");
  fclose(maxerr);
  /* open filename control file */
  namefile = fopen(filenamecontrol,"r+");
  if(namefile == NULL) {
    printf("\nCan't open %s - job terminated\n",filenamecontrol);
    return 0 ;
  }

  /* open operation control file */
  operfile = fopen(operationcontrol,"r+");
  if(operfile == NULL) {
    printf("\nCan't open %s - job terminated\n",operationcontrol);
    return 0 ;
  }

  /******* MAIN OUTER LOOP BEGINS HERE ****************/
  do{
    /* open file to write program output into
       this file is first opened and reclosed after use to 
       allow the user to examine it during program execution.
       Its contents allows the user to determine how far the 
       program is to completion */  
    outfile = fopen(out_file_name,"a");
  loop1:  /* jump back to here if illegal input line */
    
    /* read a line from the filename control file */
    flag = fscanf(namefile,"%s %s",trainname,testname);
    if(flag == EOF) break;  /* no more data in file - break do loop */

    /* check to make sure there were two filenames read */
    if(flag != 2) {
      fprintf(outfile,"Wrong number of filenames on a line in filename control file");
      goto loop1;
    }
    trainfile = fopen(trainname,"r");
    if(trainfile == NULL) {
      fprintf(outfile,"Can't open file %s \n",trainname);
      goto loop1;
    }
    
    trn_samp_num = 0;
    response_sum = 0.0 ;
    trn_min = 9999999999.0 ;
    trn_max = -9999999999.0 ;
    for (i=1 ; i<=10 ; i++) {
      trnx_min[i] = 99999999999.9;
      tstx_min[i] = 99999999999.9;
      trnx_max[i] = -99999999999.9;
      tstx_max[i] = -99999999999.9;
    }

    do{
      flag = fscanf(trainfile,"%[^\n]\n",line);
      if(flag == EOF) 
	break;
      if(trn_samp_num == 0) {
	str = line;
	pstr = &str ;
	predx = cntcol(pstr) - 1 ;
      }
      str = line;
      pstr = &str ;
      for(i=1; i<=predx; i++) {
	x = pardub(pstr);
	if(x < trnx_min[i]) trnx_min[i] = x;
	if(x > trnx_max[i]) trnx_max[i] = x;
      }

      /* get the response variable from train file input line */
      /* use it to compute the response mean */
      response = pardub(pstr);
      response_sum += response ;
        
      if(response < trn_min) 
	trn_min = response;
      if(response > trn_max) 
	trn_max = response;
      trn_samp_num++;
    }while(1);
    fclose(trainfile);
    if((predx <= 0) || (predx >= 10)) {
      fprintf(outfile,"Wrong numbers of predictor variables in training file %s\n",trainfile);
      goto loop1 ; 
    }
   
    /* compute the response mean */
    response_mean = response_sum / (double)trn_samp_num ;
    testfile = fopen(testname,"r");
    if(testfile == NULL) {
      fprintf(outfile,"Can't open file %s \n",testname);
      goto loop1 ;
    }
    tst_samp_num = 0;
    mse_sum = 0.0;
    tst_mean = 0.0;
    tst_min = 9999999999.0 ;
    tst_max = -9999999999.0 ;

    do{
      flag = fscanf(testfile,"%[^\n]\n",line);
      if(flag == EOF) 
	break;
      tst_samp_num++;
      str = line;
      pstr = &str ;
      for(i=1 ; i<=predx ; i++) {
	x = pardub(pstr);
	if(x < tstx_min[i]) tstx_min[i] =x;                                                    
	if(x > tstx_max[i]) tstx_max[i] = x;
      }

      /* get the response variable from test file input line */
      y = pardub(pstr);
      diff = response_mean - y ;
      mse_sum += diff * diff ;
      tst_mean += y ;
      if(y < tst_min) tst_min = y ;
      if(y > tst_max) tst_max = y ;
    }while(1);

    fclose(testfile);
    response_mean_variance = mse_sum/(tst_samp_num+1) ;
    response_mean_rms = sqrt(response_mean_variance);
    tst_mean=tst_mean/(tst_samp_num);
    tst_diff = tst_max-tst_min ;

    /* find scaling factor for each predictor variable */
    for (i=1 ; i<=predx ; i++) {
      if(trnx_min[i]<tstx_min[i]) 
	xmin[i] = trnx_min[i];
      else 
	xmin[i] = tstx_min[i];
      if(trnx_max[i] > tstx_max[i]) 
	xmax[i] = trnx_max[i];
      else 
	xmax[i] = tstx_max[i];
      scalex[i] = xmax[i] - xmin[i] ;
      
    }
    /************* rescale training file so all response values are 
  between .1 and .9 *****************************************/
    
    if(trn_min < tst_min) 
      response_min = trn_min;
    else 
      response_min = tst_min;

    if(trn_max > tst_max) 
      response_max = trn_max;
    else 
      response_max = tst_max ;
    scale = response_max - response_min ;

    trainfile = fopen(trainname,"r");
    temptrainfile = fopen(temptrainname,"w");
    tem2trainfile = fopen(tem2trainname,"w");

    do{
      flag= fscanf(trainfile,"%[^\n]\n",line);
      if(flag==EOF) 
	break;
      str = line;
      pstr = &str;

      for( n=1 ; n<=predx ; n++) {
	x = -1.0 +( 2.0*(pardub(pstr) - xmin[n]))/scalex[n]; 
	fprintf(temptrainfile,"%14.12f  ",x);
	fprintf(tem2trainfile,"%14.12f  ",x);
      }
      
      ytemp = pardub(pstr);
      y = .1 +( (ytemp-response_min)*.8)/scale;
      fprintf(temptrainfile,"%14.12f\n",y);
      fprintf(tem2trainfile,"%14.12f\n",ytemp);
    }while(1);
      
    fclose(trainfile);
    fclose(temptrainfile);
    fclose(tem2trainfile);
    testfile = fopen(testname,"r");
    tem2testfile = fopen(tem2testname,"w");
    temptestfile = fopen(temptestname,"w");
    
    do{
      flag= fscanf(testfile,"%[^\n]\n",line);
      if(flag==EOF) break;
      str = line;
      pstr = &str;

      for( n=1 ; n<=predx ; n++) {
	x = -1.0 +( 2.0*(pardub(pstr) - xmin[n]))/scalex[n]; 
	fprintf(tem2testfile,"%14.12f  ",x);
	fprintf(temptestfile,"%14.12f  ",x);
      }

      ytemp = pardub(pstr);
      y = .1 +( (ytemp-response_min)*.8)/scale;
      fprintf(temptestfile,"%14.12f\n",y);
      fprintf(tem2testfile,"%14.12f\n",ytemp);
    }while(1);
    fclose(testfile);
    fclose(temptestfile);
    fclose(tem2testfile);
   
    for(i=1; i<=80 ; i++) 
      fprintf(outfile,"_");
    fprintf(outfile,"\n\nTraining file %s opened with %d samples and %2d predictor variables (mean=%f)\n",trainname,trn_samp_num,predx,response_mean);
    fprintf(outfile,"Test file %s opened with %d samples (mean=%f)\n",testname,tst_samp_num,tst_mean);
    fprintf(outfile,"Zeroth order model (y=%f) has RMS error=%f on the test set.\n", response_mean,response_mean_rms);
    for(i=1; i<=80 ; i++) 
      fprintf(outfile,"_");
    fprintf(outfile,"\n");    
    fclose(outfile);

    /* minmaxout is the output file that shows worst case error */
    maxerr = fopen(maxerr_name,"a");
    for(i=1; i<=80 ; i++) 
      fprintf(maxerr,"_");
    fprintf(maxerr,"\n\nTraining file %s has response MIN = %f\n",trainname,trn_min);
    fprintf(maxerr,"Training file %s has response MAX = %f\n",trainname,trn_max);
    fprintf(maxerr,"Test file %s has response MIN = %f\n",testname,tst_min);
    fprintf(maxerr,"Test file %s has response MAX = %f\n",testname,tst_max);
    fprintf(maxerr,"The test response MIN-MAX difference is %f\n",tst_diff);
    for(i=1; i<=80 ; i++) 
      fprintf(maxerr,"_");
    fprintf(maxerr,"\n");
    fclose(maxerr);

    /************ INNER CONTROL FILE LOOP BEGINS HERE ********************/
    fseek(operfile, 0 , 0 );   /* reset operation file ptr to zero */
    do{
    loop2: /* if error in control line return here */
      flag = fscanf(operfile,"%[^\n]\n",line);
      if(flag == EOF) 
	break; /* control file now empty */
      if(strlen(line) < 4) {
	fprintf(outfile,"Too few characters in command line\n");
	goto finish;
      }

      strncpy(command,line,4);
      command[4] = '\0' ;
      command_num = lookup_cmd(command);
      if(command_num == 0){
	fprintf(outfile,"Illegal command or syntax in operation file\n");
	goto finish;
      }

      strcpy(parameter_line, line+4) ;
      para_ptr = parameter_line ;
      switch (command_num){
      case 1 : 
	ann1(); 
	break;
      case 2 : 
	ctm1(); 
	break;
      case 3 : 
	mars1(); 
	break;
      case 4 : 
	ppr1(); 
	break;
      case 5 : 
	knn1(); 
	break;
      }
    }while(1);
  }while(1);
 finish:
  
  strcpy(s,"rm ");
  strcat(s,temptrainname);
  system(s);

  strcpy(s,"rm ");
  strcat(s,temptestname);
  system(s);

  strcpy(s,"rm ");
  strcat(s,tem2trainname);
  system(s);

  strcpy(s,"rm ");
  strcat(s,tem2testname);
  system(s);

  fclose(operfile);
  fclose(namefile);
  return 1 ;
}

/*_________________________________________________________________________*/
/*                                                                         */
/*  PARSDUBL - ParseDouble routine to parse a double from a string         */
/*                                                                         */
/*_________________________________________________________________________*/
double pardub ( char **str )
{
  double num ;
  while (! (isdigit ( **str )  
	    ||  (**str == '-')  
	    ||  (**str == '.') 
	    || (**str == '+') )) {
    if (**str)
      ++(*str) ;
    else
      return 0. ;
  }
  num = atof ( *str ) ;
  while ((isdigit ( **str )
	  ||(**str=='-')
	  ||(**str=='.')
	  ||(**str=='e')
	  ||(**str=='E') 
	  || (**str == '+') ))
    ++(*str) ;
  
  return num ;
}

/*_________________________________________________________________________*/
/*                                                                         */
/*  CNTCOL - routine to count the number of numbers in a line of chars     */
/*                                                                         */
/*_________________________________________________________________________*/
int cntcol ( char **str )
{
  int i = 0 ;
  do{
    while (! (isdigit ( **str )  
	      ||  (**str == '-')  
	      ||  (**str == '.') 
	      || (**str == '+') )) {
      if (**str)
	++(*str) ;
      else
	return i ;
    }
    i++;
    while (isdigit ( **str )
	   ||(**str=='-')
	   ||(**str=='.')
	   ||(**str=='e')
	   ||(**str=='E')
	   ||(**str=='+') )
      ++(*str) ;
  }while(1);
}

void star_bar_plot(FILE *pfile, double y , double yscale ) {
#define COLUMNS 80
  int jj,ypos ;
  int columns = COLUMNS ;
  char matrix[COLUMNS+1] ;
  for(jj=1 ; jj<=columns ; jj++){
    if( jj%5 != 0 ) matrix[jj] = '.' ;
    else matrix[jj] = '|' ;
  }

  ypos =1 + (int)( (double)(columns-1) * y  / yscale ) ;
  if(ypos > columns) ypos = columns;
  if(ypos < 1) ypos = 1 ;
  ypos = columns-ypos+1 ;

  for(jj=1 ; jj<=ypos ; jj++) 
    matrix[jj] = '*' ;

  for(jj=1 ; jj<=columns ; jj++) 
    fprintf(pfile,"%c",matrix[jj]);
  fprintf(pfile,"\n");
  return ;
}

int lookup_cmd(char *cmd){
  if(strcmp(cmd,"ANN1") == 0) 
    return 1 ;
  if(strcmp(cmd,"CTM1") == 0) 
    return 2 ;
  if(strcmp(cmd,"CTMR") == 0) 
    return 2 ;
  if(strcmp(cmd,"MRS1") == 0) 
    return 3 ;
  if(strcmp(cmd,"MARS") == 0) 
    return 3 ;
  if(strcmp(cmd,"PPR1") == 0) 
    return 4 ;
  if(strcmp(cmd,"PRPR") == 0) 
    return 4 ;
  if(strcmp(cmd,"KNN1") == 0) 
    return 5 ;
  if(strcmp(cmd,"K-NN") == 0) 
    return 5 ;
  return 0;
}

int one_param(int *a){
  int num;
  while( ! ( isdigit(*para_ptr)) ) {
    if(*para_ptr) 
      ++(para_ptr);
    else 
      return 0 ;
  }
  num = atoi( para_ptr );
  while ( isdigit(*para_ptr) ) 
    ++(para_ptr) ;

  *a = num ;
  return 1 ;
}

int two_param(int *a ,int *b){
  int num;
  while( ! ( isdigit(*para_ptr)) ){
    if(*para_ptr) 
      ++(para_ptr);
    else 
      return 0 ;
  }
  num = atoi( para_ptr );
  while ( isdigit(*para_ptr) ) 
    ++(para_ptr) ;

  *a = num ;
  if( *para_ptr != ',') {
    printf("Illegal syntax in operaton file paramater list\n");
    exit(0);
  }
  ++(para_ptr);

  num = atoi( para_ptr );
  while ( isdigit(*para_ptr) ) 
    ++(para_ptr) ;

  *b = num ;
  return 1 ;
}

void ppr1(){
  char ppconname[] = "pp.config";
  char ppr_result_file[] = "ppr.res";
  FILE *ppconfile,*testfile, *resultfile, *outfile ,*maxerr;
  int m_max , m_min ;
  int flag , k ,status;
  char line[241];
  char *str ;
  char **pstr;
  double mse_sum,rms,diff,y,y_est,max_diff,fvu,max_ratio,mse,rmsr ;

  while( one_param(  &m_min ) ){
    m_max = m_min+5 ;
    ppconfile = fopen(ppconname,"w");
    fprintf(ppconfile,"%s\n",trainname);
    fprintf(ppconfile,"%d\n",trn_samp_num);
    fprintf(ppconfile,"%1d\n",predx);
    fprintf(ppconfile,"%s\n",testname);
    fprintf(ppconfile,"%d\n",tst_samp_num);
    fprintf(ppconfile,"%d\n",m_max);
    fprintf(ppconfile,"%d\n",m_min);
    fprintf(ppconfile,"%s\n",ppr_result_file);
    fclose(ppconfile);
    status = system("./xpp");    
    if (status!=0) {
      outfile = fopen(out_file_name,"a");
      fprintf(outfile,"PRPR  terms = %d failed with status = %x\n",m_min,status);
      fclose(outfile);
      continue; 
    }                  
    
    testfile = fopen(testname,"r");
    resultfile = fopen(ppr_result_file,"r");
    if(resultfile == NULL) {
      printf("Can't open %s\n",ppr_result_file);
      return ;
    }
    mse_sum = 0;
    max_diff = 0.0;
    for(k=1 ;k<=tst_samp_num ;k++) {
      flag = fscanf(resultfile,"%[^\n]\n",line);
      if(flag == EOF) break;
      str = line;
      pstr = &str;
      y = pardub(pstr);
      y_est = pardub(pstr);
      diff = y_est - y ;
      mse_sum += diff * diff ;
      if(diff > max_diff) max_diff = diff ;
    }
    
    rms = sqrt(mse_sum/tst_samp_num);
    mse = mse_sum/tst_samp_num ;
    fvu = mse/response_mean_variance ;
    rmsr = rms/response_mean_rms ;
    outfile = fopen(out_file_name,"a");
    fprintf(outfile,"PRPR terms = %d NRMS error ==> %f\n",m_min,rmsr);
    star_bar_plot(outfile,rms,response_mean_rms);
    fclose(testfile);
    fclose(resultfile);
    fclose(outfile);
    maxerr =fopen(maxerr_name,"a");
    max_ratio = max_diff/tst_diff ;
    fprintf(maxerr,"PRPR terms = %d NMAX error ==> %f\n",m_min,max_ratio);
    star_bar_plot(maxerr,max_diff,tst_diff);
    fclose(maxerr);
  }
  return;
}

void mars1() {
  char marsconname[] = "mars.con";
  char mars_result_file[] = "mars.res";
  char mars_message_file[] = "mars.mes";
  char mars_rms_file[] = "mars.rms" ;
  FILE *marsconfile,*testfile, *resultfile, *outfile ,*maxerr;
  int basismax ,degrees;
  int flag , k, n ,status ;
  char line[241];
  char *str ;
  char **pstr;
  double mse_sum,rms,diff,y,y_est,max_diff,fvu,max_ratio,mse,rmsr ;
  int spline = SPLINE ;

  while( two_param( &basismax , &degrees) ) {
    marsconfile = fopen(marsconname,"w");
    if(marsconfile == NULL) {
      printf("Can't open %s\n",marsconname);
      return ;
    }
    fprintf(marsconfile,"%s\n",trainname);
    fprintf(marsconfile,"%d\n",trn_samp_num);
    fprintf(marsconfile,"%d\n",predx);
    fprintf(marsconfile,"%s\n",testname);
    fprintf(marsconfile,"%d\n",tst_samp_num);
    fprintf(marsconfile,"%d\n",basismax);
    fprintf(marsconfile,"%d\n",degrees);
    fprintf(marsconfile,"%d\n",predx);
    fprintf(marsconfile,"%d\n",spline);
    fprintf(marsconfile,"%s\n",mars_message_file);
    fprintf(marsconfile,"%s\n",mars_result_file);
    fprintf(marsconfile,"%s\n",mars_rms_file);
    fclose(marsconfile);
    status = system("./xmars <mars.con");
    if (status!=0) {
      outfile = fopen(out_file_name,"a");
      fprintf(outfile,"MARS with %d max. basis functions  and %d d.o.f. failed with status = %x\n",basismax,degrees,status);
      fclose(outfile);
      continue; 
    }                  
    testfile = fopen(testname,"r");
    resultfile = fopen(mars_result_file,"r");
    if(resultfile == NULL) {
      printf("Can't open %s\n",mars_result_file);
      return ;
    }
    max_diff= 0.0 ;
    mse_sum = 0;
    for(k=1 ;k<=tst_samp_num ;k++) {
      flag= fscanf(testfile,"%[^\n]\n",line);
      if(flag==EOF) break;
      str = line;
      pstr = &str;
      for( n=1 ; n<=predx ; n++) {
	pardub(pstr);
      }
      y = pardub(pstr);
      flag= fscanf(resultfile,"%[^\n]\n",line);
      if(flag==EOF) break;
      str = line;
      pstr = &str;
      for( n=1 ; n<=predx ; n++) {
	pardub(pstr);
      }
      y_est = pardub(pstr);
      diff = y_est - y ;
      mse_sum += diff * diff ;
      if(diff > max_diff) max_diff = diff ;
    }
    rms = sqrt(mse_sum/tst_samp_num);
    mse = mse_sum/tst_samp_num ;
    fvu = mse/response_mean_variance ;
    rmsr = rms/response_mean_rms ;
    outfile = fopen(out_file_name,"a");
    fprintf(outfile,"MARS with %d max. basis functions and %d d.o.f. NRMS error ==>> %f\n",basismax,degrees,rmsr);
    star_bar_plot(outfile,rms,response_mean_rms);
    fclose(testfile);
    fclose(resultfile);
    fclose(outfile);
    maxerr =fopen(maxerr_name,"a");
    max_ratio = max_diff/tst_diff ;
    fprintf(maxerr,"MARS with %d max basis functions and %d d.o.f. NMAX error ==> %f\n",basismax,degrees,max_ratio);
    star_bar_plot(maxerr,max_diff,tst_diff);
    fclose(maxerr);
  }
  return ;
}

void ann1(){
  char annconname[] = "ann1.con";
  char ann_result_file[] = "ann_temp.res";
  char unscaled_ann_result_file[] = "ann1.res";
  FILE *unscaled_resultfile ;
  FILE *annconfile,*trainfile,*testfile, *resultfile, *outfile ,*maxerr;
  int neurons ;
  int flag, i , k ,n ,status;
  char line[241];
  char s[30] ;
  char *str ;
  char **pstr;
  double mse_sum,rms,diff,y,y_est ,max_diff,x,fvu,max_ratio,mse,rmsr ;

  while ( one_param( &neurons ) ){
    annconfile = fopen(annconname,"w");
    if(annconfile == NULL){
      printf("Can't open %s\n",annconname);
      return ;
    }
    /* must remove any previous contents of the ann result file
       because the NEURAL program only appends results to an
       existing file instead of clearing it first */
    resultfile = fopen(ann_result_file,"w");
    fclose(resultfile);
    strcpy(s,"rm ");
    strcat(s,ann_result_file);
    system(s);
    fprintf(annconfile,"NETWORK MODEL : LAYER\n");
    fprintf(annconfile,"LAYER INIT : %s\n",ANNINITIALIZATION);
    fprintf(annconfile,"OUTPUT MODEL : GENERAL\n");
    fprintf(annconfile,"QUIT RETRIES : %d\n",RETRIES);
    fprintf(annconfile,"N INPUTS : %d\n",predx);
    fprintf(annconfile,"N OUTPUTS : 1\n");
    fprintf(annconfile,"N HIDDEN1 : %d\n",neurons);
    fprintf(annconfile,"N HIDDEN2 : 0\n");
    fprintf(annconfile,"TRAIN : %s\n",temptrainname);
    fprintf(annconfile,"LEARN:\n");
    fprintf(annconfile,"OUTPUT FILE : %s\n",ann_result_file);
    fprintf(annconfile,"EXECUTE : %s\n",temptestname);
    fprintf(annconfile,"CLEAR WEIGHTS :\n");
    fprintf(annconfile,"CLEAR TRAINING :\n");
    fprintf(annconfile,"QUIT :\n");
    fclose(annconfile);
    status = system("./neural <ann1.con");
    if (status!=0) {
      outfile = fopen(out_file_name,"a");
      fprintf(outfile,"ANN1 with %d hidden neurons failed with status = %x\n",neurons,status);
      fclose(outfile);
      continue; 
    }                  
    testfile = fopen(testname,"r");
    resultfile = fopen(ann_result_file,"r");
    if(resultfile == NULL) {
      printf("Can't open %s\n",ann_result_file);
      return ;
    }
    unscaled_resultfile = fopen(unscaled_ann_result_file,"w");
    max_diff =0.0;
    mse_sum = 0;
    for(k=1 ;k<=tst_samp_num ;k++) { 
      flag= fscanf(testfile,"%[^\n]\n",line);
      if(flag==EOF) break;
      str = line;
      pstr = &str;
      for( n=1 ; n<=predx ; n++) {
	x = pardub(pstr);
	fprintf(unscaled_resultfile,"%f ",x);
      }
      y = pardub(pstr);
      flag= fscanf(resultfile,"%[^\n]\n",line);
      if(flag ==EOF) break;
      str = line;
      pstr = &str;
      y_est = pardub(pstr);

      /********** unscale estimate ***************/
      y_est =( ((y_est-.1)*scale)/.8) + response_min ;
      fprintf(unscaled_resultfile,"%f\n",y_est);
      diff = y_est - y ;
      mse_sum += diff * diff ;
      if(diff > max_diff) max_diff = diff;
    }
    rms = sqrt(mse_sum/tst_samp_num);
    mse = mse_sum/tst_samp_num ;
    fvu = (mse_sum/tst_samp_num)/response_mean_variance ;
    rmsr = rms/response_mean_rms ;
    fclose(testfile);
    fclose(resultfile);
    fclose(unscaled_resultfile);
    strcpy(s,"rm ");
    strcat(s,ann_result_file);
    system(s);
    outfile = fopen(out_file_name,"a");
    fprintf(outfile,"ANN1 with %d hidden neurons NRMS error ==>> %f\n",neurons,rmsr);
    star_bar_plot(outfile,rms,response_mean_rms);
    fclose(outfile);
    maxerr =fopen(maxerr_name,"a");
    max_ratio = max_diff/tst_diff ;
    fprintf(maxerr,"ANN1 with %d hidden neurons NMAX error ==> %f\n",neurons,max_ratio);
    star_bar_plot(maxerr,max_diff,tst_diff);
    fclose(maxerr);
  }
  return ;
}

void ctm1(){
  char ctmconname[] = "ctm.con";
  char ctm_result_file[] = "fit1";
  FILE *ctmconfile,*testfile, *resultfile, *outfile ,*maxerr;
  int mapdim , smoothness ;
  int flag , k, n ,status ;
  char line[241];
  char *str ;
  char **pstr;
  double mse_sum,rms,diff,y,y_est,sigmax ,max_diff,fvu,max_ratio,mse,rmsr ;
  int iterations = ITERATIONS ;
  while( two_param( &mapdim , &smoothness ) ){
    ctmconfile = fopen(ctmconname,"w");
    if(ctmconfile == NULL){
      printf("Can't open %s\n",ctmconname);
      return ;
    }
    fprintf(ctmconfile,"%s\n",trainname);
    fprintf(ctmconfile,"%s\n",testname);
    fprintf(ctmconfile,"%d\n",mapdim);
    fprintf(ctmconfile,"%d\n",smoothness);
    fprintf(ctmconfile,"0\n");
    fprintf(ctmconfile,"1\n");
    fprintf(ctmconfile,"done\n");
    fclose(ctmconfile);
    status = system("./bctm <ctm.con");
    if (status!=0) {
      outfile = fopen(out_file_name,"a");
      fprintf(outfile,"CTMR with %d map dimensions and %d smoothness failed with status = %x\n",mapdim,smoothness,status);
      fclose(outfile);
      continue; 
    }                  
    testfile = fopen(testname,"r");
    resultfile = fopen(ctm_result_file,"r");
    if(resultfile == NULL) {
      printf("Can't open %s\n",ctm_result_file);
      return ;
    }
    max_diff = 0.0;
    mse_sum = 0;
    for(k=1 ;k<=tst_samp_num ;k++){
      flag= fscanf(testfile,"%[^\n]\n",line);
      if(flag==EOF) break;
      str = line;
      pstr = &str;
      for( n=1 ; n<=predx ; n++){
	pardub(pstr);
      }
      y = pardub(pstr);
      flag= fscanf(resultfile,"%[^\n]\n",line);
      if(flag==EOF) break;
      str = line;
      pstr = &str;
      for( n=1 ; n<=predx ; n++){
	pardub(pstr);
      }
      y_est = pardub(pstr);
      diff = y_est - y ;
      mse_sum += diff * diff ;
      if(diff > max_diff) max_diff = diff ;
    }
    rms = sqrt(mse_sum/tst_samp_num);
    mse = mse_sum/tst_samp_num ;
    fvu = mse/response_mean_variance ;
    rmsr = rms/response_mean_rms ;
    outfile = fopen(out_file_name,"a");
    fprintf(outfile,"CTMR with %d map dimensions and %d smoothness level NRMS error ==>> %f\n",mapdim,smoothness,rmsr);
    star_bar_plot(outfile,rms,response_mean_rms);
    fclose(testfile);
    fclose(resultfile);
    fclose(outfile);
    maxerr =fopen(maxerr_name,"a");
    max_ratio = max_diff/tst_diff;
    fprintf(maxerr,"CTMR with %d map dimensions and %d smoothness level NMAX error ==> %f\n",mapdim,smoothness,max_ratio);
    star_bar_plot(maxerr,max_diff,tst_diff);
    fclose(maxerr);
  }
  return ;
}

void knn1() {

#define MAX_SAMP  10001
#define MAX_IN  10
#define MAX_K 100
  int samp_num;
  double trn[MAX_SAMP][MAX_IN+2];
  char knn_result_name[16] = "knn.res" ;
  int i,j,k,flag,test_num,valid_num ;
  char *str;
  char **pstr;
  char line[241];
  FILE *trainfile;
  FILE *testfile,*unscaled_testfile ;
  FILE *outfile ,*maxerr;
  FILE *resultfile;
  double mse_sum,mse_best,y,y_est,diff,rms,max_diff,fvu,max_ratio,mse,rmsr ;
  double x[MAX_IN+1];
  double x_near[MAX_K+1];
  double y_near[MAX_K+1];
  double distance,xdif,xtemp,ytemp,yy;
  double ysum =0.0;

  resultfile = fopen(knn_result_name,"w");
  trainfile =fopen(tem2trainname,"r");
  samp_num = 0;
  do{
    flag = fscanf(trainfile,"%[^\n]\n",line);
    if(flag == EOF) break;
    samp_num++;
    str = line;
    pstr = &str;
    for( i=1 ; i<=predx+1 ; i++){
      trn[samp_num][i] =pardub(pstr);
    }
  }while(1);

  fclose(trainfile);
  while( one_param(&k) ) {
    testfile = fopen(tem2testname,"r");
    unscaled_testfile = fopen(testname,"r");
    mse_sum = 0.0;
    test_num = 0;
    do{
      flag = fscanf(testfile,"%[^\n]\n",line);
      if(flag==EOF) break;
      str = line;
      pstr = &str;
      for( i=1 ; i<=predx ; i++){
	x[i] = pardub(pstr);
      }
      y = pardub(pstr);
      /******** compute the nearest neighbor *****************/
      for(j=1 ; j<=MAX_K ; j++) x_near[j] = 999999999999.0 ;
      ysum = 0.0;
      for(i=1; i<= samp_num ; i++){
        distance = 0.0;
	for(j=1; j<=predx ; j++){
	  xdif = (x[j]-trn[i][j]);
	  distance += xdif*xdif;
	}
	distance = sqrt(distance);
	yy = trn[i][predx+1];
	for(j=1 ; j<=k ; j++){
	  if(distance < x_near[j] ) {
	    xtemp = x_near[j];
	    ytemp = y_near[j];
	    x_near[j] = distance;
	    y_near[j] = yy;
	    distance = xtemp;
	    yy = ytemp;
	  }
	}
      }
      for(j=1; j<=k ; j++){
	ysum += y_near[j];
      }
      y_est =  ysum/ (double) k ;

      /*****************************************************/       
      diff = y_est - y ;
      mse_sum += diff * diff ;
      test_num++;
      if(diff > max_diff) 
	max_diff = diff;
      fscanf(unscaled_testfile,"%[^\n]\n",line);
      str = line ;
      pstr = &str ;
      for(i=1 ; i<=predx ; i++)
	fprintf(resultfile,"%f ",pardub(pstr) );
            
      fprintf(resultfile,"%11.8f\n",y_est);
    }while(1);
    
    rms = sqrt(mse_sum/tst_samp_num);
    mse = mse_sum/tst_samp_num ;
    fvu =mse/response_mean_variance ;
    rmsr = rms/response_mean_rms ;
    outfile = fopen(out_file_name,"a");
    fprintf(outfile,"K-NN with %d neighbors NRMS error ===>> %f\n",k,rmsr);
    star_bar_plot(outfile,rms,response_mean_rms);    
    fclose(testfile);
    fclose(unscaled_testfile);
    fclose(outfile);
    /* this causes a crash for some reason. HR */
    //fclose(resultfile);
    maxerr =fopen(maxerr_name,"a");
    max_ratio = max_diff/tst_diff;
    fprintf(maxerr,"K-NN with %d neighbors NMAX error ==> %f\n",k,max_ratio);
    star_bar_plot(maxerr,max_diff,tst_diff);
    fclose(maxerr);
  }
  return ;
}







