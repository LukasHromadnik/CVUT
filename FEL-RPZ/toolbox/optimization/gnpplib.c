/*-----------------------------------------------------------------------
gnpplib.c: Library of solvers for Generalized Nearest Point Problem (GNPP).

  Generalized Nearest Point Problem (GNPP) to solve is
 
   min 0.5*alpha'*H*alpha + c'*alpha

   subject to  sum(alpha(find(y==1))) = 1,  
               sum(alpha(find(y==2))) = 1,
               alpha(i) >= 0
 
 H [dim x dim] is symmetric positive definite matrix.
 c [dim x 1] is an arbitrary vector.
 y [dim x 1] is an vector its entries aqual to 1 and/or 2.

 The precision of the found solution is given by
 the parameters tmax, tolabs and tolrel which
 define the stopping conditions:
 
 UB-LB <= tolabs      ->  exit_flag = 1   Abs. tolerance.
 UB-LB <= UB*tolrel   ->  exit_flag = 2   Relative tolerance.
 LB > th              ->  exit_flag = 3   Threshoold on lower bound.
 t >= tmax            ->  exit_flag = 0   Number of iterations.

 UB ... Upper bound on the optimal solution.
 LB ... Lower bound on the optimal solution.
 t  ... Number of iterations.
 History ... Value of LB and UB wrt. number of iterations.


 The following algorithms are implemented:
 ..............................................

 - GNPP solver based on MDM algorithm.
   exitflag = gnpp_mdm( &get_col, diag_H, vector_c, vector_y,
       dim, tmax, tolabs, tolrel, th, &alpha, &t, &aHa11, &aHa22, &History );

 - GNPP solver based on improved MDM algorithm (u fixed v optimized).
   exitflag = gnpp_mdm( &get_col, diag_H, vector_c, vector_y,
       dim, tmax, tolabs, tolrel, th, &alpha, &t, &aHa11, &aHa22, &History );

  For more info refer to V.Franc: Optimization Algorithms for Kernel 
  Methods. Research report. CTU-CMP-2005-22. CTU FEL Prague. 2005.
  ftp://cmp.felk.cvut.cz/pub/cmp/articles/franc/Franc-PhD.pdf .

 Modifications:
 09-sep-2005, VF
 27-nov-2004, VF, addopted from qpcsolver.c

-------------------------------------------------------------------- */

#include "mex.h"
#include "matrix.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define HISTORY_BUF 1000000

#define MINUS_INF INT_MIN
#define PLUS_INF  INT_MAX

#define ABS(A) ((A >= 0) ? A : -A)
#define MIN(A,B) ((A < B) ? A : B)
#define INDEX(ROW,COL,DIM) ((COL*DIM)+ROW)

/* --------------------------------------------------------------
 QPC solver based on MDM algorithm.

 Usage: exitflag = gnpp_mdm( &get_col, diag_H, vector_c, vector_y,
       dim, tmax, tolabs, tolrel, th, &alpha, &t, &aHa11, &aHa22, &History );
-------------------------------------------------------------- */
int gnpp_mdm(const void* (*get_col)(long,long),
            double *diag_H,
            double *vector_c,
            double *vector_y,
            long dim, 
            long tmax,
            double tolabs,
            double tolrel,
            double th,
            double *alpha,
            long  *ptr_t, 
            double *ptr_aHa11,
            double *ptr_aHa22,
            double **ptr_History,
            long verb)
{
  double LB;
  double UB;
  double aHa11, aHa12, aHa22, ac1, ac2;
  double tmp;
  double Huu, Huv, Hvv;
  double min_beta1, max_beta1, min_beta2, max_beta2, beta;
  double lambda;
  double delta1, delta2;
  double *History;
  double *Ha1;
  double *Ha2;
  double *tmp_ptr;
  double *col_u, *col_v;
  double *col_v1, *col_v2;
  long u1, u2;
  long v1, v2;
  long i;
  long t;
  long History_size;
  int exitflag;

  /* ------------------------------------------------------------ */
  /* Initialization                                               */
  /* ------------------------------------------------------------ */

  Ha1 = mxCalloc(dim, sizeof(double));
  if( Ha1 == NULL ) mexErrMsgTxt("Not enough memory.");
  Ha2 = mxCalloc(dim, sizeof(double));
  if( Ha2 == NULL ) mexErrMsgTxt("Not enough memory.");

  History_size = (tmax < HISTORY_BUF ) ? tmax+1 : HISTORY_BUF;
  History = mxCalloc(History_size*2,sizeof(double));
  if( History == NULL ) mexErrMsgTxt("Not enough memory.");

  /* inx1 = firts of find( y ==1 ), inx2 = firts of find( y ==2 ) */
  v1 = -1; v2 = -1; i = 0;
  while( (v1 == -1 || v2 == -1) && i < dim ) {
    if( v1 == -1 && vector_y[i] == 1 ) { v1 = i; }
    if( v2 == -1 && vector_y[i] == 2 ) { v2 = i; } 
    i++;
  }

  col_v1 = (double*)get_col(v1,-1);
  col_v2 = (double*)get_col(v2,v1);
  
  aHa12 = col_v1[v2];
  aHa11 = diag_H[v1];
  aHa22 = diag_H[v2];
  ac1 = vector_c[v1];
  ac2 = vector_c[v2];

  min_beta1 = PLUS_INF; min_beta2 = PLUS_INF;
  for( i = 0; i < dim; i++ ) 
  {
    alpha[i] = 0;
    Ha1[i] = col_v1[i];
    Ha2[i] = col_v2[i];

    beta = Ha1[i] + Ha2[i] + vector_c[i];

    if( vector_y[i] == 1 && min_beta1 > beta ) {
      u1 = i;
      min_beta1 = beta;
    }

    if( vector_y[i] == 2 && min_beta2 > beta ) {
      u2 = i;
      min_beta2 = beta;
    }
  }

  alpha[v1] = 1;
  alpha[v2] = 1;

  UB = 0.5*(aHa11 + 2*aHa12 + aHa22) + ac1 + ac2;
  LB = min_beta1 + min_beta2 - 0.5*(aHa11 + 2*aHa12 + aHa22);

  delta1 = Ha1[v1] + Ha2[v1] + vector_c[v1] - min_beta1;
  delta2 = Ha1[v2] + Ha2[v2] + vector_c[v2] - min_beta2;

  t = 0;
  History[INDEX(0,0,2)] = LB;
  History[INDEX(1,0,2)] = UB;

  if( verb ) {
    mexPrintf("Init: UB=%f, LB=%f, UB-LB=%f, (UB-LB)/|UB|=%f \n",
      UB, LB, UB-LB,(UB-LB)/UB);
  }  

  /* Stopping conditions */
  if( UB-LB <= tolabs ) exitflag = 1;
  else if(UB-LB <= ABS(UB)*tolrel ) exitflag = 2;
  else if(LB > th) exitflag = 3;
  else exitflag = -1;

  /* ------------------------------------------------------------ */
  /* Main optimization loop                                       */
  /* ------------------------------------------------------------ */

  while( exitflag == -1 ) 
  {
    t++;     

    if( delta1 > delta2 ) 
    {
      col_u = (double*)get_col(u1,-1);
      col_v = (double*)get_col(v1,u1);

      Huu = diag_H[u1];
      Hvv = diag_H[v1];
      Huv = col_u[v1];

      lambda = delta1/(alpha[v1]*(Huu - 2*Huv + Hvv ));
      lambda = MIN(1,lambda);

      tmp = lambda*alpha[v1];

      aHa11 = aHa11 + 2*tmp*(Ha1[u1]-Ha1[v1])+tmp*tmp*( Huu - 2*Huv + Hvv );
      aHa12 = aHa12 + tmp*(Ha2[u1]-Ha2[v1]);
      ac1 = ac1 + tmp*(vector_c[u1]-vector_c[v1]);

      alpha[u1] = alpha[u1] + tmp;
      alpha[v1] = alpha[v1] - tmp;

      min_beta1 = PLUS_INF; min_beta2 = PLUS_INF;
      max_beta1 = MINUS_INF; max_beta2 = MINUS_INF; 
      for( i = 0; i < dim; i ++ )
      {
         Ha1[i] = Ha1[i] + tmp*(col_u[i] - col_v[i]);

         beta = Ha1[i] + Ha2[i] + vector_c[i];
         if( vector_y[i] == 1 ) 
           {
             if( min_beta1 > beta ) { u1 = i; min_beta1 = beta; }
             if( max_beta1 < beta && alpha[i] > 0 ) { v1 = i; max_beta1 = beta; }
           }
         else
           {
             if( min_beta2 > beta ) { u2 = i; min_beta2 = beta; }
             if( max_beta2 < beta && alpha[i] > 0) { v2 = i; max_beta2 = beta; }
           }
      }
    }
    else
    {
      col_u = (double*)get_col(u2,-1);
      col_v = (double*)get_col(v2,u2);

      Huu = diag_H[u2];
      Hvv = diag_H[v2];
      Huv = col_u[v2];
  
      lambda = delta2/(alpha[v2]*( Huu - 2*Huv + Hvv ));
      lambda = MIN(1,lambda);

      tmp = lambda*alpha[v2];
      aHa22 = aHa22 + 2*tmp*( Ha2[u2]-Ha2[v2]) + tmp*tmp*( Huu - 2*Huv + Hvv);
      aHa12 = aHa12 + tmp*(Ha1[u2]-Ha1[v2]);
      ac2 = ac2 + tmp*( vector_c[u2]-vector_c[v2] );

      alpha[u2] = alpha[u2] + tmp;
      alpha[v2] = alpha[v2] - tmp;

      min_beta1 = PLUS_INF; min_beta2 = PLUS_INF;
      max_beta1 = MINUS_INF; max_beta2 = MINUS_INF; 
      for(i = 0; i < dim; i++ ) 
      {  
         Ha2[i] = Ha2[i] + tmp*( col_u[i] - col_v[i] );

         beta = Ha1[i] + Ha2[i] + vector_c[i];

         if( vector_y[i] == 1 ) 
         {
           if( min_beta1 > beta ) { u1 = i; min_beta1 = beta; }
           if( max_beta1 < beta && alpha[i] > 0 ) { v1 = i; max_beta1 = beta; }
         }
         else
         {
           if( min_beta2 > beta ) { u2 = i; min_beta2 = beta; }
           if( max_beta2 < beta && alpha[i] > 0) { v2 = i; max_beta2 = beta; }
         }
      }
    }

    UB = 0.5*(aHa11 + 2*aHa12 + aHa22) + ac1 + ac2;
    LB = min_beta1 + min_beta2 - 0.5*(aHa11 + 2*aHa12 + aHa22);
  
    delta1 = Ha1[v1] + Ha2[v1] + vector_c[v1] - min_beta1;
    delta2 = Ha1[v2] + Ha2[v2] + vector_c[v2] - min_beta2;

    /* Stopping conditions */
    if( UB-LB <= tolabs ) exitflag = 1; 
    else if( UB-LB <= ABS(UB)*tolrel ) exitflag = 2;
    else if(LB > th) exitflag = 3;
    else if(t >= tmax) exitflag = 0; 

    if( verb && (t % verb) == 0) {
     mexPrintf("%d: UB=%f,LB=%f,UB-LB=%f,(UB-LB)/|UB|=%f\n",
        t, UB, LB, UB-LB,(UB-LB)/UB); 
    }  

    /* Store selected values */
    if( t < History_size ) {
      History[INDEX(0,t,2)] = LB;
      History[INDEX(1,t,2)] = UB;
    }
    else {
      tmp_ptr = mxCalloc((History_size+HISTORY_BUF)*2,sizeof(double));
      if( tmp_ptr == NULL ) mexErrMsgTxt("Not enough memory.");
      for( i = 0; i < History_size; i++ ) {
        tmp_ptr[INDEX(0,i,2)] = History[INDEX(0,i,2)];
        tmp_ptr[INDEX(1,i,2)] = History[INDEX(1,i,2)];
      }
      tmp_ptr[INDEX(0,t,2)] = LB;
      tmp_ptr[INDEX(1,t,2)] = UB;
      
      History_size += HISTORY_BUF;
      mxFree( History );
      History = tmp_ptr;
    }
  }

  /* print info about last iteration*/
  if(verb && (t % verb) ) {
    mexPrintf("Exit: UB=%f, LB=%f, UB-LB=%f, (UB-LB)/|UB|=%f \n",
      UB, LB, UB-LB,(UB-LB)/UB);
  }  

  /*------------------------------------------------------- */
  /* Set outputs                                            */
  /*------------------------------------------------------- */
  (*ptr_t) = t;
  (*ptr_aHa11) = aHa11;
  (*ptr_aHa22) = aHa22;
  (*ptr_History) = History;

  /* Free memory */
  mxFree( Ha1 );
  mxFree( Ha2 );
  
  return( exitflag ); 
}


/* --------------------------------------------------------------
 QPC solver based on improved MDM algorithm (u fixed v optimized)

 Usage: exitflag = gnpp_imdm( &get_col, diag_H, vector_c, vector_y,
       dim, tmax, tolabs, tolrel, th, &alpha, &t, &aHa11, &aHa22, &History );
-------------------------------------------------------------- */
int gnpp_imdm(const void* (*get_col)(long,long),
            double *diag_H,
            double *vector_c,
            double *vector_y,
            long dim, 
            long tmax,
            double tolabs,
            double tolrel,
            double th,
            double *alpha,
            long  *ptr_t, 
            double *ptr_aHa11,
            double *ptr_aHa22,
            double **ptr_History,
            long verb)
{
  double LB;
  double UB;
  double aHa11, aHa12, aHa22, ac1, ac2;
  double tmp;
  double Huu, Huv, Hvv;
  double min_beta1, max_beta1, min_beta2, max_beta2, beta;
  double lambda;
  double delta1, delta2;
  double improv, max_improv;
  double *History;
  double *Ha1;
  double *Ha2;
  double *tmp_ptr;
  double *col_u, *col_v;
  double *col_v1, *col_v2;
  long u1, u2;
  long v1, v2;
  long i;
  long t;
  long History_size;
  int exitflag;
  int which_case;

  /* ------------------------------------------------------------ */
  /* Initialization                                               */
  /* ------------------------------------------------------------ */

  Ha1 = mxCalloc(dim, sizeof(double));
  if( Ha1 == NULL ) mexErrMsgTxt("Not enough memory.");
  Ha2 = mxCalloc(dim, sizeof(double));
  if( Ha2 == NULL ) mexErrMsgTxt("Not enough memory.");

  History_size = (tmax < HISTORY_BUF ) ? tmax+1 : HISTORY_BUF;
  History = mxCalloc(History_size*2,sizeof(double));
  if( History == NULL ) mexErrMsgTxt("Not enough memory.");

  /* inx1 = firts of find( y ==1 ), inx2 = firts of find( y ==2 ) */
  v1 = -1; v2 = -1; i = 0;
  while( (v1 == -1 || v2 == -1) && i < dim ) {
    if( v1 == -1 && vector_y[i] == 1 ) { v1 = i; }
    if( v2 == -1 && vector_y[i] == 2 ) { v2 = i; } 
    i++;
  }

  col_v1 = (double*)get_col(v1,-1);
  col_v2 = (double*)get_col(v2,v1);
  
  aHa12 = col_v1[v2];
  aHa11 = diag_H[v1];
  aHa22 = diag_H[v2];
  ac1 = vector_c[v1];
  ac2 = vector_c[v2];

  min_beta1 = PLUS_INF; min_beta2 = PLUS_INF;
  for( i = 0; i < dim; i++ ) 
  {
    alpha[i] = 0;
    Ha1[i] = col_v1[i];
    Ha2[i] = col_v2[i];

    beta = Ha1[i] + Ha2[i] + vector_c[i];

    if( vector_y[i] == 1 && min_beta1 > beta ) {
      u1 = i;
      min_beta1 = beta;
    }

    if( vector_y[i] == 2 && min_beta2 > beta ) {
      u2 = i;
      min_beta2 = beta;
    }
  }

  alpha[v1] = 1;
  alpha[v2] = 1;

  UB = 0.5*(aHa11 + 2*aHa12 + aHa22) + ac1 + ac2;
  LB = min_beta1 + min_beta2 - 0.5*(aHa11 + 2*aHa12 + aHa22);

  delta1 = Ha1[v1] + Ha2[v1] + vector_c[v1] - min_beta1;
  delta2 = Ha1[v2] + Ha2[v2] + vector_c[v2] - min_beta2;

  t = 0;
  History[INDEX(0,0,2)] = LB;
  History[INDEX(1,0,2)] = UB;

  if( verb ) {
    mexPrintf("Init: UB=%f, LB=%f, UB-LB=%f, (UB-LB)/|UB|=%f \n",
      UB, LB, UB-LB,(UB-LB)/UB);
  }  

  if( delta1 > delta2 ) 
  {
     which_case = 1;
     col_u = (double*)get_col(u1,v1);
     col_v = col_v1;
  }
  else
  {
     which_case = 2;
     col_u = (double*)get_col(u2,v2);
     col_v = col_v2;
  }

  /* Stopping conditions */
  if( UB-LB <= tolabs ) exitflag = 1;
  else if(UB-LB <= ABS(UB)*tolrel ) exitflag = 2;
  else if(LB > th) exitflag = 3;
  else exitflag = -1;

  /* ------------------------------------------------------------ */
  /* Main optimization loop                                       */
  /* ------------------------------------------------------------ */

  while( exitflag == -1 ) 
  {
    t++;     

    if( which_case == 1 )
    {
      Huu = diag_H[u1];
      Hvv = diag_H[v1];
      Huv = col_u[v1];

      lambda = delta1/(alpha[v1]*(Huu - 2*Huv + Hvv ));
      lambda = MIN(1,lambda);

      tmp = lambda*alpha[v1];

      aHa11 = aHa11 + 2*tmp*(Ha1[u1]-Ha1[v1])+tmp*tmp*( Huu - 2*Huv + Hvv );
      aHa12 = aHa12 + tmp*(Ha2[u1]-Ha2[v1]);
      ac1 = ac1 + tmp*(vector_c[u1]-vector_c[v1]);

      alpha[u1] = alpha[u1] + tmp;
      alpha[v1] = alpha[v1] - tmp;

      min_beta1 = PLUS_INF; min_beta2 = PLUS_INF;
      max_beta1 = MINUS_INF; max_beta2 = MINUS_INF; 
      for( i = 0; i < dim; i ++ )
      {
         Ha1[i] = Ha1[i] + tmp*(col_u[i] - col_v[i]);

         beta = Ha1[i] + Ha2[i] + vector_c[i];
         if( vector_y[i] == 1 ) 
           {
             if( min_beta1 > beta ) { u1 = i; min_beta1 = beta; }
             if( max_beta1 < beta && alpha[i] > 0 ) { v1 = i; max_beta1 = beta; }
           }
         else
           {
             if( min_beta2 > beta ) { u2 = i; min_beta2 = beta; }
             if( max_beta2 < beta && alpha[i] > 0) { v2 = i; max_beta2 = beta; }
           }
      }
    }
    else
    {
      Huu = diag_H[u2];
      Hvv = diag_H[v2];
      Huv = col_u[v2];
  
      lambda = delta2/(alpha[v2]*( Huu - 2*Huv + Hvv ));
      lambda = MIN(1,lambda);

      tmp = lambda*alpha[v2];
      aHa22 = aHa22 + 2*tmp*( Ha2[u2]-Ha2[v2]) + tmp*tmp*( Huu - 2*Huv + Hvv);
      aHa12 = aHa12 + tmp*(Ha1[u2]-Ha1[v2]);
      ac2 = ac2 + tmp*( vector_c[u2]-vector_c[v2] );

      alpha[u2] = alpha[u2] + tmp;
      alpha[v2] = alpha[v2] - tmp;

      min_beta1 = PLUS_INF; min_beta2 = PLUS_INF;
      max_beta1 = MINUS_INF; max_beta2 = MINUS_INF; 
      for(i = 0; i < dim; i++ ) 
      {  
         Ha2[i] = Ha2[i] + tmp*( col_u[i] - col_v[i] );

         beta = Ha1[i] + Ha2[i] + vector_c[i];

         if( vector_y[i] == 1 ) 
         {
           if( min_beta1 > beta ) { u1 = i; min_beta1 = beta; }
           if( max_beta1 < beta && alpha[i] > 0 ) { v1 = i; max_beta1 = beta; }
         }
         else
         {
           if( min_beta2 > beta ) { u2 = i; min_beta2 = beta; }
           if( max_beta2 < beta && alpha[i] > 0) { v2 = i; max_beta2 = beta; }
         }
      }
    }

    UB = 0.5*(aHa11 + 2*aHa12 + aHa22) + ac1 + ac2;
    LB = min_beta1 + min_beta2 - 0.5*(aHa11 + 2*aHa12 + aHa22);
  
    delta1 = Ha1[v1] + Ha2[v1] + vector_c[v1] - min_beta1;
    delta2 = Ha1[v2] + Ha2[v2] + vector_c[v2] - min_beta2;

    if( delta1 > delta2 ) 
    {
       col_u = (double*)get_col(u1,-1);

      /* search for optimal v while u is fixed */
      for( max_improv =  MINUS_INF, i = 0; i < dim; i++ ) {

        if( vector_y[i] == 1 && alpha[i] != 0 ) {

          beta = Ha1[i] + Ha2[i] + vector_c[i];

          if( beta >= min_beta1 ) {

            tmp = diag_H[u1] - 2*col_u[i] + diag_H[i];
            if( tmp != 0 ) {
              improv = (0.5*(beta-min_beta1)*(beta-min_beta1))/tmp;

              if( improv > max_improv ) {
                max_improv = improv;
                v1 = i;
              }
            }
          }
        }
      }
      col_v = (double*)get_col(v1,u1);
      delta1 = Ha1[v1] + Ha2[v1] + vector_c[v1] - min_beta1;
      which_case = 1;
      
    }
    else
    {
       col_u = (double*)get_col(u2,-1);

      /* search for optimal v while u is fixed */
      for( max_improv =  MINUS_INF, i = 0; i < dim; i++ ) {

        if( vector_y[i] == 2 && alpha[i] != 0 ) {

          beta = Ha1[i] + Ha2[i] + vector_c[i];

          if( beta >= min_beta2 ) {

            tmp = diag_H[u2] - 2*col_u[i] + diag_H[i];
            if( tmp != 0 ) {
              improv = (0.5*(beta-min_beta2)*(beta-min_beta2))/tmp;

              if( improv > max_improv ) {
                max_improv = improv;
                v2 = i;
              }
            }
          }
        }
      }

      col_v = (double*)get_col(v2,u2);
      delta2 = Ha1[v2] + Ha2[v2] + vector_c[v2] - min_beta2;
      which_case = 2;
    }
    

    /* Stopping conditions */
    if( UB-LB <= tolabs ) exitflag = 1; 
    else if( UB-LB <= ABS(UB)*tolrel ) exitflag = 2;
    else if(LB > th) exitflag = 3;
    else if(t >= tmax) exitflag = 0; 

    if( verb && (t % verb) == 0) {
     mexPrintf("%d: UB=%f,LB=%f,UB-LB=%f,(UB-LB)/|UB|=%f\n",
        t, UB, LB, UB-LB,(UB-LB)/UB); 
    }  

    /* Store selected values */
    if( t < History_size ) {
      History[INDEX(0,t,2)] = LB;
      History[INDEX(1,t,2)] = UB;
    }
    else {
      tmp_ptr = mxCalloc((History_size+HISTORY_BUF)*2,sizeof(double));
      if( tmp_ptr == NULL ) mexErrMsgTxt("Not enough memory.");
      for( i = 0; i < History_size; i++ ) {
        tmp_ptr[INDEX(0,i,2)] = History[INDEX(0,i,2)];
        tmp_ptr[INDEX(1,i,2)] = History[INDEX(1,i,2)];
      }
      tmp_ptr[INDEX(0,t,2)] = LB;
      tmp_ptr[INDEX(1,t,2)] = UB;
      
      History_size += HISTORY_BUF;
      mxFree( History );
      History = tmp_ptr;
    }
  }

  /* print info about last iteration*/
  if(verb && (t % verb) ) {
    mexPrintf("Exit: UB=%f, LB=%f, UB-LB=%f, (UB-LB)/|UB|=%f \n",
      UB, LB, UB-LB,(UB-LB)/UB);
  }  

  /*------------------------------------------------------- */
  /* Set outputs                                            */
  /*------------------------------------------------------- */
  (*ptr_t) = t;
  (*ptr_aHa11) = aHa11;
  (*ptr_aHa22) = aHa22;
  (*ptr_History) = History;

  /* Free memory */
  mxFree( Ha1 );
  mxFree( Ha2 );
  
  return( exitflag ); 
}




