/*-----------------------------------------------------------------------
qpbsvmlib.c: Library for solving QP task required for learning SVM 
 without bias term. 
   
  min 0.5*x'*H*x + f'*x

  subject to  C >= x(i) >= 0 for all i
  
 H [dim x dim] is symmetric positive semi-definite matrix.
 f [dim x 1] is an arbitrary vector.

 The precision of found solution is given by parameters 
 tmax, tolabs, tolrel which define the stopping conditions:

    t >= tmax                   ->  exit_flag = 0  Number of iterations.
    UB-LB <= tolabs             ->  exit_flag = 1  Abs. tolerance.
    UB-LB <= UB*tolrel          ->  exit_flag = 2  Relative tolerance.

 UB ... Upper bound on the optimal solution.
 LB ... Lower bound on the optimal solution.
 t  ... Number of iterations.
 History ... Value of LB and UB wrt. number of iterations.

 1. Generalized Gauss-Seidel methods
 exitflag = qpbsvm_sca( &get_col, diag_H, f, UB, dim, tmax, 
               tolabs, tolrel, x, Nabla, &t, &History, verb )

 2. Greedy variant - Udpate variable yielding the best improvement.
 exitflag = qpbsvm_scas( &get_col, diag_H, f, UB, dim, tmax, 
               tolabs, tolrel, x, Nabla, &t, &History, verb )

 3. Updates variable which most violates the KKT conditions
 exitflag = qpbsvm_scamv( &get_col, diag_H, f, UB, dim, tmax, 
               tolabs, tolrel, tolKKT, x, Nabla, &t, &History, verb )

Modifications:
 08-dec-2006, VF
 21-nov-2006, VF
 03-nov-2006, VF
 01-nov-2006, VF

-------------------------------------------------------------------- */

#include "mex.h"
#include "matrix.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define HISTORY_BUF 1000000

#define ABS(A) ((A >= 0) ? A : -(A))
#define MIN(A,B) ((A < B) ? (A) : (B))
#define MAX(A,B) ((A >= B) ? (A) : (B))
#define INDEX(ROW,COL,DIM) ((COL*DIM)+ROW)

/* --------------------------------------------------------------

Usage: exitflag = qpbsvm_sca( &get_col, diag_H, f, UB, dim, tmax, 
               tolabs, tolrel, tolKKT, x, Nabla, &t, &History, verb )

-------------------------------------------------------------- */
int qpbsvm_sca(const void* (*get_col)(long,long),
            double *diag_H,
            double *f,
            double UB,
            long   dim,
            long   tmax,
            double tolabs,
            double tolrel,
            double tolKKT,
            double *x,
	        double *Nabla,
            long   *ptr_t,
            double **ptr_History,
            long   verb)
{
  double *History;
  double *col_H;
  double *tmp_ptr;
  double x_old;
  double delta_x;
  double xHx;
  double Q_P;
  double Q_D;
  double xf;
  double xi_sum;
  long History_size;
  long t;
  long i, j;
  int exitflag;
  int KKTsatisf;

  /* ------------------------------------------------------------ */
  /* Initialization                                               */
  /* ------------------------------------------------------------ */

  t = 0;

  History_size = (tmax < HISTORY_BUF ) ? tmax+1 : HISTORY_BUF;
  History = mxCalloc(History_size*2,sizeof(double));
  if( History == NULL ) mexErrMsgTxt("Not enough memory.");

  /* compute Q_P and Q_D */
  xHx = 0;
  xf = 0;
  xi_sum = 0;
  for(i = 0; i < dim; i++ ) {
    xHx += x[i]*(Nabla[i] - f[i]);
    xf += x[i]*f[i];
    xi_sum += MAX(0,-Nabla[i]);
  }

  Q_P = 0.5*xHx + xf;
  Q_D = -0.5*xHx - UB*xi_sum;
  History[INDEX(0,t,2)] = Q_P;
  History[INDEX(1,t,2)] = Q_D;

  if( verb > 0 ) {
    mexPrintf("%d: Q_P=%f, Q_D=%f, Q_P-Q_D=%f, (Q_P-Q_D)/|Q_P|=%f \n",
     t, Q_P, Q_D, Q_P-Q_D,(Q_P-Q_D)/ABS(Q_P));
  }

  exitflag = -1;
  while( exitflag == -1 ) 
  {
    t++;     
 
    for(i = 0; i < dim; i++ ) {
      if( diag_H[i] > 0 ) {
        /* variable update */
        x_old = x[i];
        x[i] = MIN(UB,MAX(0, x[i] - Nabla[i]/diag_H[i]));
        
        /* update Nabla */
        delta_x = x[i] - x_old;
        if( delta_x != 0 ) {
          col_H = (double*)get_col(i,-1);
          for(j = 0; j < dim; j++ ) {
            Nabla[j] += col_H[j]*delta_x;
          }
        }   

      }
    }

    /* compute Q_P and Q_D */
    xHx = 0;
    xf = 0;
    xi_sum = 0;
    KKTsatisf = 1;
    for(i = 0; i < dim; i++ ) {
      xHx += x[i]*(Nabla[i] - f[i]);
      xf += x[i]*f[i];
      xi_sum += MAX(0,-Nabla[i]);

      if((x[i] > 0 && x[i] < UB && ABS(Nabla[i]) > tolKKT) || 
         (x[i] == 0 && Nabla[i] < -tolKKT) ||
         (x[i] == UB && Nabla[i] > tolKKT)) KKTsatisf = 0;
    }

    Q_P = 0.5*xHx + xf;
    Q_D = -0.5*xHx - UB*xi_sum;

    /* stopping conditions */
    if(t >= tmax) exitflag = 0;
    else if(Q_P-Q_D <= tolabs) exitflag = 1;
    else if(Q_P-Q_D <= ABS(Q_P)*tolrel) exitflag = 2;
    else if(KKTsatisf == 1) exitflag = 3;

    if( verb > 0 && (t % verb == 0 || t==1)) {
      mexPrintf("%d: Q_P=%f, Q_D=%f, Q_P-Q_D=%f, (Q_P-Q_D)/|Q_P|=%f \n",
        t, Q_P, Q_D, Q_P-Q_D,(Q_P-Q_D)/ABS(Q_P));
    }

    /* Store UB LB to History buffer */
    if( t < History_size ) {
      History[INDEX(0,t,2)] = Q_P;
      History[INDEX(1,t,2)] = Q_D;
    }
    else {
      tmp_ptr = mxCalloc((History_size+HISTORY_BUF)*2,sizeof(double));
      if( tmp_ptr == NULL ) mexErrMsgTxt("Not enough memory.");
      for( i = 0; i < History_size; i++ ) {
        tmp_ptr[INDEX(0,i,2)] = History[INDEX(0,i,2)];
        tmp_ptr[INDEX(1,i,2)] = History[INDEX(1,i,2)];
      }
      tmp_ptr[INDEX(0,t,2)] = Q_P;
      tmp_ptr[INDEX(1,t,2)] = Q_D;
      
      History_size += HISTORY_BUF;
      mxFree( History );
      History = tmp_ptr;
    }
  }

  (*ptr_t) = t;
  (*ptr_History) = History;

  return( exitflag ); 
}


/* --------------------------------------------------------------

Usage: exitflag = qpbsvm_scas( &get_col, diag_H, f, UB, dim, tmax, 
               tolabs, tolrel, tolKKT, x, Nabla, &t, &History, verb )

-------------------------------------------------------------- */
int qpbsvm_scas(const void* (*get_col)(long,long),
            double *diag_H,
            double *f,
            double UB,
            long   dim,
            long   tmax,
            double tolabs,
            double tolrel,
            double tolKKT,
            double *x,
	        double *Nabla,
            long   *ptr_t,
            double **ptr_History,
            long   verb)
{
  double *History;
  double *col_H;
  double *tmp_ptr;
  double x_old;
  double x_new;
  double delta_x;
  double max_x;
  double xHx;
  double Q_P;
  double Q_D;
  double xf;
  double xi_sum;
  double max_update;
  double curr_update;
  long History_size;
  long t;
  long i, j;
  long max_i;
  int exitflag;
  int KKTsatisf;

  /* ------------------------------------------------------------ */
  /* Initialization                                               */
  /* ------------------------------------------------------------ */

  t = 0;

  History_size = (tmax < HISTORY_BUF ) ? tmax+1 : HISTORY_BUF;
  History = mxCalloc(History_size*2,sizeof(double));
  if( History == NULL ) mexErrMsgTxt("Not enough memory.");

  /* compute Q_P and Q_D */
  xHx = 0;
  xf = 0;
  xi_sum = 0;
  for(i = 0; i < dim; i++ ) {
    xHx += x[i]*(Nabla[i] - f[i]);
    xf += x[i]*f[i];
    xi_sum += MAX(0,-Nabla[i]);
  }

  Q_P = 0.5*xHx + xf;
  Q_D = -0.5*xHx - UB*xi_sum;
  History[INDEX(0,t,2)] = Q_P;
  History[INDEX(1,t,2)] = Q_D;

  if( verb > 0 ) {
    mexPrintf("%d: Q_P=%f, Q_D=%f, Q_P-Q_D=%f, (Q_P-Q_D)/|Q_P|=%f \n",
     t, Q_P, Q_D, Q_P-Q_D,(Q_P-Q_D)/ABS(Q_P));
  }

  exitflag = -1;
  while( exitflag == -1 ) 
  {
    t++;     

    max_update = -mxGetInf();
    for(i = 0; i < dim; i++ ) {
      if( diag_H[i] > 0 ) { 
        /* variable update */
        x_old = x[i];
        x_new = MIN(UB,MAX(0, x[i] - Nabla[i]/diag_H[i]));
  
        curr_update = -0.5*diag_H[i]*(x_new*x_new-x_old*x_old) - 
          (Nabla[i] - diag_H[i]*x_old)*(x_new - x_old);

        if( curr_update > max_update ) {
          max_i = i;
          max_update = curr_update;
          max_x = x_new;
        }
      } 
    }                                                                                            

    x_old = x[max_i];
    x[max_i] = max_x;

    /* update Nabla */
    delta_x = max_x - x_old;
    if( delta_x != 0 ) {
      col_H = (double*)get_col(max_i,-1);
      for(j = 0; j < dim; j++ ) {
        Nabla[j] += col_H[j]*delta_x;
      }
    }   

    /* compute Q_P and Q_D */
    xHx = 0;
    xf = 0;
    xi_sum = 0;
    KKTsatisf = 1;
    for(i = 0; i < dim; i++ ) {
      xHx += x[i]*(Nabla[i] - f[i]);
      xf += x[i]*f[i];
      xi_sum += MAX(0,-Nabla[i]);

      if((x[i] > 0 && x[i] < UB && ABS(Nabla[i]) > tolKKT) || 
         (x[i] == 0 && Nabla[i] < -tolKKT) ||
         (x[i] == UB && Nabla[i] > tolKKT)) KKTsatisf = 0;
    }

    Q_P = 0.5*xHx + xf;
    Q_D = -0.5*xHx - UB*xi_sum;

    /* stopping conditions */
    if(t >= tmax) exitflag = 0;
    else if(Q_P-Q_D <= tolabs) exitflag = 1;
    else if(Q_P-Q_D <= ABS(Q_P)*tolrel) exitflag = 2;
    else if(KKTsatisf == 1) exitflag = 3;

    if( verb > 0 && (t % verb == 0 || t==1)) {
      mexPrintf("%d: Q_P=%f, Q_D=%f, Q_P-Q_D=%f, (Q_P-Q_D)/|Q_P|=%f \n",
        t, Q_P, Q_D, Q_P-Q_D,(Q_P-Q_D)/ABS(Q_P));
    }

    /* Store UB LB to History buffer */
    if( t < History_size ) {
      History[INDEX(0,t,2)] = Q_P;
      History[INDEX(1,t,2)] = Q_D;
    }
    else {
      tmp_ptr = mxCalloc((History_size+HISTORY_BUF)*2,sizeof(double));
      if( tmp_ptr == NULL ) mexErrMsgTxt("Not enough memory.");
      for( i = 0; i < History_size; i++ ) {
        tmp_ptr[INDEX(0,i,2)] = History[INDEX(0,i,2)];
        tmp_ptr[INDEX(1,i,2)] = History[INDEX(1,i,2)];
      }
      tmp_ptr[INDEX(0,t,2)] = Q_P;
      tmp_ptr[INDEX(1,t,2)] = Q_D;
      
      History_size += HISTORY_BUF;
      mxFree( History );
      History = tmp_ptr;
    }
  }

  (*ptr_t) = t;
  (*ptr_History) = History;

  return( exitflag ); 
}

/* --------------------------------------------------------------

Usage: exitflag = qpbsvm_scamv( &get_col, diag_H, f, UB, dim, tmax, 
               tolabs, tolrel, tolKKT, x, Nabla, &t, &History, verb )

-------------------------------------------------------------- */
int qpbsvm_scamv(const void* (*get_col)(long,long),
            double *diag_H,
            double *f,
            double UB,
            long   dim,
            long   tmax,
            double tolabs,
            double tolrel,
            double tolKKT,
            double *x,
	        double *Nabla,
            long   *ptr_t,
            double **ptr_History,
            long   verb)
{
  double *History;
  double *col_H;
  double delta_x;
  double x_new;
  double max_viol;
  double fval;
  long t;
  long i;
  long u;
  int exitflag;

  /* ------------------------------------------------------------ */
  /* Initialization                                               */
  /* ------------------------------------------------------------ */

  t = 0;
  exitflag = -1;
  while( exitflag == -1 && t <= tmax) 
  {
    t++;     

    max_viol = 0;
    for(i = 0; i < dim; i++ ) 
    {      
      if( x[i] == 0 )
      {
        if( max_viol < -Nabla[i]) { u = i; max_viol = -Nabla[i]; }
      }
      else if( x[i] > 0 && x[i] < UB )
      {
        if( max_viol < ABS(Nabla[i]) ) { u = i; max_viol = ABS(Nabla[i]); } 
      }
      else if( max_viol < Nabla[i]) { u = i; max_viol = Nabla[i]; }
    }

/*    mexPrintf("%d: max_viol=%f, u=%d\n", t, max_viol, u);*/

    if( max_viol <= tolKKT ) 
    {
      exitflag = 1;
    } 
    else
    {
      /* update */
      x_new = MIN(UB,MAX(0, x[u] - Nabla[u]/diag_H[u]));

      delta_x = x_new - x[u];
      x[u] = x_new;

      col_H = (double*)get_col(u,-1);
      for(i = 0; i < dim; i++ ) {
        Nabla[i] += col_H[i]*delta_x;
      }
    }
  }

  History = mxCalloc((t+1)*2,sizeof(double));
  if( History == NULL ) mexErrMsgTxt("Not enough memory.");

  fval = 0;
  for(fval = 0, i = 0; i < dim; i++ ) {
    fval += 0.5*x[i]*(Nabla[i]+f[i]);
  }

  History[INDEX(0,t,2)] = fval;
  History[INDEX(1,t,2)] = 0;

  (*ptr_t) = t;
  (*ptr_History) = History;



  return( exitflag ); 
}
