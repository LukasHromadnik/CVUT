c
c Multivariate Adaptive Regression Splines (MARS modeling, version 3.5).
c
c
c Coded and copywrite (c) by Jerome H. Friedman (6/16/91).
c
c
c
c                         A Micro User's Guide
c                                  to
c                               MARS 3.5
c
c                          Jerome H. Friedman
c                          Stanford University
c
c    MARS 3.5 is a collection of subroutines that implement the multivariate
c adaptive regression spline strategy for data fitting and function
c approximation described in Friedman (1991a, 1991b). It is a generalization of
c MARS 1.0 described in Friedman (1988) and MARS 2.5 described in Friedman
c (1991a). It implements as a special case a palindromically invariant version
c of the TURBO fitting technique for smoothing and additive modeling described
c in Friedman and Silverman (1989).
c
c    These subroutines represent a set of tools that can be invoked from a user
c coded program to perform various analyses. The user routine is responsible for
c reading the data into memory and passing it to the MARS subroutines, along
c with the various parameter settings, as arguments. This set of subroutines 
c can also form the basis for incorporating this methodology into a
c statistical language or package.
c
c    The user interface subroutines are:
c
c MARS: computes the mars model from the data and provides summary
c     information for interpreting it.
c
c MISS: sets up mars to handle missing values.
c
c PLOT: constructs graphical output, useful for interpreting the
c     continuous part of the mars model, in a format that can be plotted
c     with a local graphics package.
c
c CATPRT: prints tables useful for interpreting the purely categorical parts
c     of the mars model.
c
c SLICE: produces user selected lower dimensional representations of the
c    mars model to aid in interpretation.
c
c FMOD: computes response estimates for given predictor variable vectors
c    given the mars model.
c
c   It should be noted that this is a new methodology with which there is,
c as of this time, little collective experience.
c
c
c References:
c
c [1] Friedman, J. H. (1988). Fitting functions to noisy data in high
c     dimensions. Proc., Twentyth Symposium on the Interface, Wegman, Gantz,
c     and Miller, eds. American Statistical Association, Alexandria, VA. 3-43.
c
c [2] Friedman, J. H. (1991a). Multivariate adaptive regression splines
c     (with discussion).  Annals of Statistics, 19, 1-141 (March).
c
c [3] Friedman, J. H. (1991b). Estimating functions of mixed ordinal and
c     categorical variables using adaptive splines. Department of Statistics,
c     Stanford University, Tech. Report LCS108.
c
c [4] Friedman, J. H. and Silverman, B. W. (1989). Flexible parsimonious
c     smoothing and additive modeling (with discussion). TECHNOMETRICS, 31,
c     3-39 (Feburary).
c
c
c
c User interface subroutines:
c
c       All arguments in the calling sequence of user called MARS
c       subroutines must be of the same type in the calling program
c       as indicated in the subroutine's documentation below. All
c       calling sequence arrays must be dimensioned  in the calling
c       program as indicated in the documentation below. This includes
c       all workspace arrays. The leading dimensions of multidimensional
c       arrays must match, whereas the last dimension and singley 
c       dimensioned arrays can be as large or larger than that indicated.
c
c       More detailed explanations for some of the quanities below can
c       be found in the indicated sections of references [2] and/or [3]
c       above.
c
c
c
c call mars (n,p,x,y,w,nk,mi,lx,fm,im,sp,dp,mm):
c
c input:
c n = number of observations.
c p = number of predictor variables per observation (integer).
c x(n,p) = predictor variable data matrix.
c y(n) = response value for each observation.
c w(n) = weight (mass) for each observation.
c nk = maximum number of basis functions.(Ref[2] Sec. 3.6, Ref[3] Sec. 2.3)
c mi = maximum number of variables per basis function (interaction level).
c      mi=1 => additive modeling (main effects only);
c      mi>1 => up to mi-variable interactions allowed.
c lx(p) = predictor variable flags; lx(i) corresponds to the ith variable:
c    lx(i) =  0 : exclude variable from model.
c             1 : ordinal variable - no restriction.
c             2 : ordinal variable that can only enter additively;
c                 no interactions with other variables.
c             3 : ordinal variable that can enter only linearly.
c            -1 : categorical variable - no restriction.
c            -2 : categorical variable that can only enter additively;
c                 no interactions with other variables.
c
c output:
c fm(3+nk*(5*mi+nmcv+6)+2*p+ntcv), im(21+nk*(3*mi+8)) = mars model.
c    (nmcv = maximum number of distinct values for any categorical variable;
c     ntcv = total number of distinct values over all categorical variables.)
c
c    note: upon return im(1) and im(2) contain the lengths of the fm and im
c          arrays (respectively) actually used by the program.
c
c workspace:
c sp(n*(max(nk+1,2)+3)+max(3*n+5*nk+p,2*p,4*n)+2*p+4*nk) : real.
c dp(max(n*nk,(nk+1)*(nk+1))+max((nk+2)*(nmcv+3),4*nk)) : double precision.
c mm(n*p+2*max(mi,nmcv)) : integer.
c
c
c defaults:
c    the following quanities are set to default values in mars. their values
c    can be changed by executing any of the following statements before the
c    call to mars, with the new value as the argument.
c
c call logit(il):
c il = ordinary/logistic regression flag.
c    il=0 => ordinary least-squares regression.
c    il=1 => logistic regression.
c    (default: il=0). If logistic regression is selected (il=1) then
c    the response variable is assumed to take on only the values 0/1 and
c    the mars model is for the log-odds: f(x) = log (Pr(Y=1:x)/Pr(Y=0:x)).
c    (Ref[2] Sec. 4.5)
c
c call setdf(df):
c df = number of degrees-of-freedom charged for (unrestricted)
c      knot optimization. (default: df=3.0)
c      (Ref[2] Sec. 3.6, Ref[3] Sec. 2.3)
c
c call xvalid(ix):
c ix = control parameter for sample reuse technique used to automatically
c    estimate smoothing parameter df (see above) from the data.
c    ix = 0 => no effect (default). value used for df is set by user if
c            setdf(df) is called (see above), otherwise default value 
c            (df=3.0) is used.
c    ix > 0 => ix - fold cross-validation.
c    ix < 0 => single validation pass using every (-ix)th (randomly selected)
c            observation as an independent test set.
c    if ix.ne.0 then call setdf(df) (see above) has no effect. if ix > 0,
c    computation increases roughly by a factor of ix over that for ix = 0.
c    for ix < 0 computation increases approximately by a factor of two.
c    (Ref[3] Sec. 2.3)
c 
c call stseed(is):
c is = seed for internal random number generator used to group observation
c      subsets for validation (ix.ne.0). (default: is=987654321).
c
c call print(it):
c it = fortran file number for printed output. (it.le.0 => no printed output.)
c      note that this controls printed output for all user called mars
c      routines. (default: it=6).
c
c call setfv(fv):
c fv = (fractional) incremental penalty for increasing the number of variables
c      in the mars model. sometimes useful with highly collinear designs as it
c      may produce nearly equivalent models with fewer predictor variables,
c      aiding in interpretation. (fv .ge. 0)
c    fv=0.0  => no penalty (default).
c    fv=0.05 => moderate penalty.
c    fv=0.1  => heavy penality.
c    the best value depends on the specific situation and some user
c    experimentation using different values is usually required. this option
c    should be used with some care. (Ref[2] Sec. 5.3)
c
c call setic(ic):
c ic = flag restricting categorical - ordinal interactions.
c    ic=0 => no effect (default).
c    ic=1 => interactions between categorical and ordinal variables prohibited.
c    ic=2 => maximum number of ordinal variables participating in any
c            interaction is restricted to two. categorical interactions are
c            unrestricted.
c    the restrictions associated with a value of ic are imposed in addition
c    to those that are controlled by the mi and lx flags (see above).
c
c call setint(i,j,k):
c i,j = predictor variable numbers.
c   k = interaction flag:
c      k.eq.0 => interactions between variables i and j are prohibited.
c      k.ne.0 => interactions between variables i and j are permitted
c                if allowed by the mi, lx, and ic parameter values (see above).
c                (default)
c   i.eq.0 .or. j.eq.0 => reset to defaults for all predictor variables.
c
c   this call can be executed repeatedly to invoke (or remove) multiple
c   constraints.
c
c call nest (i,j,nv,vals);
c nests predictor variable i within categorical predictor variable j.
c links the existance of a value for var(i) to a subset of values for var(j).
c variable j must be categorical (lx(j) < 0, see above). (Ref[3] Sec. 3.3)
c
c i = variable to be nested (ordinal or categorical).
c j = variable to which i is nested (categorical).
c nv = size of corresponding subset of values for variable j.
c vals(nv) = specific values of variable j for which values of variable i
c            are defined to exist.
c
c setting nv = 0 removes the nesting associated with i and j previously set.
c setting i = 0 and/or j = 0 removes all nesting (default).
c
c this call can be executed repeatedly to invoke (or remove) nestings.
c (recursive nesting is not implemented. j may not itself be nested to
c another variable.)
c
c note: variable nesting does NOT override the interaction constraints set by
c       calling setic or setint (see above). the mi and lx flags (see above)
c       do not limit interactions on variables to which others are nested.
c       they control nested and other nonnested variables as described above
c       except that interactions with variables to which others are nested
c       are ignored in applying the constraints.
c
c
c call setms(ms): 
c ms = minimum span (minimum number of observations between each knot).
c      ms .le. 0 => default value (depending on n and p) is used.
c      (default: ms=0). (Ref[2] Sec. 3.8)
c
c
c the following three routines (miss, mkmiss, xmiss) are used to enable
c mars to deal with various aspects of missing predictor values in the
c training data and/or future data to be predicted. for problems with
c no such missing values, these three routines are of no concern.
c
c
c call miss (n,p,x,lx,xm,flg,pn,xn,lxn,xs,xp);
c
c called  (optionally) before mars (see above) to indicate the presence of
c missing values in the predictor variable data matrix.
c
c sets up mars to accomodate missing values.
c produces as output transformations of some of the original mars input
c quanities defining the problem. the new transformed quanities define
c the same problem - but with missing values - and replace the corresponding
c original quanities in the call to mars (see above) and plot and slice
c (see below). in particular, a new predictor data matrix is created
c with extra (dummy) variables - one for each original variable with
c missing values - to indicate observations for which the corresponding
c original variable is not missing. each such original variable is
c automatically nested (see above) to this (corresponding) dummy variable.
c also produces as output a slicing vector to be used as input to
c slice (see below) to produce the mars model corresponding to no missing
c values on any of the original predictor variables, for interpretation.
c (Ref[3] Sec. 3.4)
c
c input:
c n,p,x,lx = original intended input to mars (see above).
c xm(p) = vector giving missing value flag for each original variable.
c         x(i,j) = xm(j) => x(i,j) is missing.
c flg = input to slice (see below).
c
c output:
c pn,xn,lxn = corresponding transformed quanities to be used as input to mars.
c pn = number of predictor variables in transformed data matrix (integer,
c      .le. 2*p)
c xn(n,pn) = transformed data matrix.
c lxn(pn) = predictor variable flags for transformed data matrix.
c xs(pn) = input for slice (see below). slicing vector that produces a slice
c          of the augmented predictor variable space to produce the nonmissing
c          mars model.
c xp(2*p+1) = input for xmiss (see below).
c
c notes:
c    (1) the value of the output quanity pn is less than or equal to 2*p.
c        the other output quanities (arrays) should be dimensioned
c        xn(n,2*p), lxn(2*p), xs(2*p) in the calling program to be safe.
c    (2) if there are no missing values then the output quanities 
c        pn,xn,lxn will be identical to p,x,lx respectively, and
c        xs(j)=flg, j=1,p.
c    (3) the corresponding quanities for input and output can be the same in
c        the calling program. however, they should have the corresponding
c        dimension equal to 2*p, and they will be altered.
c    (4) dimensions of all relevant workspace arrays in mars and other
c        user callable routines must be large enough to accomodate the
c        the increased number of variables pn (.le. 2*p) produced.
c
c
c call mkmiss (n,p,x,y,w,xm,pm,nnx,nn,xnn,yn,wn,sc);
c
c called (optionally) before miss (see above) to generate additional data
c with missing values by resampling from original (input) data.
c
c used to train mars for missing predictor values when they are under
c represented in the training data. takes as input original data and
c produces as output a new (larger) data set containing both the original
c data and additional data resampled from it with predictor variable values
c flagged as missing. (Ref[3] Sec. 3.4)
c
c input:
c n,p,x,y,w = original data set (see mars above).
c xm(p) = same as in miss (see above).
c pm(p) = vector of fractions of missing values in output sample:
c    pm(j) = fraction of the total output data with jth predictor variable
c            value missing.
c nnx = maximum sample size of new (output) data.
c
c output:
c nn = sample size of generated output data set (input to miss, mars, and
c      other user called routines, in place of n).
c xnn(nn,p) = new generated predictor data matrix to be used as input to
c             miss (see above) in place of x.
c yn(nn),wn(nn) = new generated data to be used as input to mars and other
c                 user called routines in place of y and w.
c
c workspace:
c sc(p*nnx): real.
c
c notes:
c    (1) the output arrays should be dimensioned xnn(nnx,p),yn(nnx),wn(nnx)
c        in the calling program to be safe.
c    (2) if much larger fractions of missing values are requested than
c        exist in the orginal training data then the size of the output
c        (resampled) data set can become very large (nn = big).
c    (3) if the size of the output (resampled) data set reaches the
c        specified maximum (nnx) then the actual fractions of missing
c        values for each variable will be less than those specified in the
c        input vector pm(p).
c    (4) dimensions of all relevant workspace arrays in mars and other
c        user callable routines must be large enough to accomodate the
c        the increased number of observations nn (.le. nnx) produced.
c
c
c call xmiss (n,x,xm,xp,xn);
c
c must be called before fmod (see below) if miss (see above) was called
c before mars (see above) to handle missing values. produces a new set
c of covariate vectors, from the original set intended for fmod, to replace
c the original set in the call to fmod. (Ref[3] Sec. 3.4)
c
c input:
c n = number of covariate vectors (see fmod below).
c x(n,p) = original covariate vectors intended for fmod.
c xm, xp = same as in miss (see above).
c
c output:
c xn(n,pn) = new set of covariate vectors to be used as input to fmod.
c
c notes:
c    (1) the value of the quanity pn is less than or equal to 2*p (p = the
c        number of original predictor variables). the output quanity (array)
c        should be dimensioned xn(n,2*p) in the calling program to be safe.
c    (2) the corresponding quanities x and xn can be the same in
c        the calling program. however, it should have the second
c        dimension equal to 2*p, and it will be altered.
c
c
c
c
c The following subroutines can be called only after mars.
c
c
c
c call plot (m,x,fm,im,ngc,ngs,icx,nc,crv,ns,srf,sp,mm):
c
c computes plots of all purely additive and all purely bivariate ordinal
c contributions to the mars model,in a form suitable for displaying
c with a computer graphics package. If there are no interactions between
c categorical and ordinal variables present in the mars model, this
c subroutine returns plots of the (compound) anova functions (Ref[2] Sec. 3.5,
c eqns (25) and (27)) for those ordinal variables involved in at most two
c (ordinal) variable interactions. If categorical-ordinal interactions are
c present, it returns the corresponding plots for each ordinal contribution
c in the categorical-ordinal decomposition (Ref[3] Sec. 3.2).  since the
c locations of the plotted functions are arbitrary, they are all translated
c to have zero minimum value.
c
c input:
c   m = model flag:
c     = 1 => plot piecewise-linear mars model.
c     = 2 => plot piecewise-cubic mars model. (Ref[2] Sec. 3.7)
c   x,fm,im = same as in mars (see above).
c   ngc = number of raster points for computing curve estimates.
c   ngs = number of raster points on each axis for computing surface estimates.
c   icx = convex hull flag:
c       = 0 => plot surface estimates over entire range of argument limits.
c       > 0 => plot surface estimates only inside the convex hull of the
c             (bivariate) point set.
c
c output:
c    nc = number of curves (purely additive ordinal anova functions).
c    crv(ngc,2,nc) = additive ordinal anova functions in anova
c                    decomposition order.
c       crv(.,1,m) = ordered abscissa values for mth anova function.
c       crv(.,2,m) = corresponding ordinate values.
c    ns = number of surfaces (purely bivariate ordinal anova functions).
c    srf(ngs,ngs,ns) = bivariate (plus associated univariate) ordinal
c                      anova functions in anova decomposition order.
c       srf(.,.,m) = total contribution (bivariate + univariate) of ordinal
c                  variables associated with mth bivariate anova function of the
c                  mars model, in a square raster format (ngs x ngs) over the
c                  ranges of the two variables. the first and second indicies
c                  correspond to the first and second variables respectively.
c
c workspace:
c    sp(max(4*ngs*ngs,ngc,2*n)) : real.
c    mm(max(2*(mi+1),nmcv)) : integer.
c
c
c
c call catprt (m,fm,im,sp,mm):
c
c prints all univariate and bivariate contributions to the purely categorical
c part of the mars model that are not involved in higher order (categorical) 
c interactions. These are the (compound) anova functions (Ref[2] Sec. 3.5,
c eqns (25) and (27)) for the purely categorical part of the categorical-
c ordinal decomposition (Ref[3] Sec. 3.2, eqns (32b) and (38b)). since
c the locations of the printed functions are arbitrary they are all
c translated to have zero minimum value. the functions are printed as tables
c of integers in the range [0,99] or [0,9]. the function value for each
c entry is the product of the corresponding integer and the scale factor
c printed above the table.
c
c input:
c m = model flag:
c   = 1 => piecewise-linear model.
c   = 2 => piecewise-cubic  model. (Ref[2] Sec. 3.7)
c fm,im = same as in mars (see above).
c
c workspace:
c sp(nmcv*nmcv) : real.
c mm(2*nk+nmcv) : integer.
c
c
c
c call slice (flg,xs,x,fm,im,fmn,imn,sp,mm):
c
c computes the mars model within a lower dimensional subspace defined by an
c axis oriented slice of the predictor variable space. the slice is selected
c by assigning specific values to a subset of the predictor variables. the
c returned model is a function of the variables complement to the selected
c subset, and represents the mars model conditioned on the specified values
c for the selected variables. this new lower dimensional sliced model can
c be input to plot and/or catprt for interpretation in the same manner as the
c original mars model. (Ref[2] Sec. 4.7, Ref[3] Sec. 2.4)
c
c input:
c flg = flag for indicating that a predictor variable is not in the subset
c    defining the slice. its value should be outside the range of values for
c    all predictor variables.
c xs(p) = vector defining the slice:
c    xs(i).eq.flg => do not condition on ith variable.
c    xs(i).ne.flg => condition on ith variable at value stored in xs(i).
c x = same as in mars (see above).
c fm,im = arrays defining mars model (output from mars - see above).
c
c output:
c fmn,imn = corresponding arrays defining the sliced model
c             (dimensioned same as in mars - see above).
c
c workspace:
c sp(2*nk+2*p+max(nk,3*p)) : real.
c mm(2*p) : integer.
c
c
c
c call fmod (m,n,x,fm,im,f,sp):
c
c calculates mars model response estimates for sets of covariate vectors.
c
c input:
c m = model flag:
c   = 1 => piecewise-linear mars model.
c   = 2 => piecewise-cubic mars model. (Ref[2] Sec. 3.7)
c n = number of covariate vectors.
c x(n,p) = covariate vectors.
c fm,im = same as in mars (see above).
c
c output:
c f(n) = value of the mars model estimate for each covariate vector.
c
c workspace:
c sp(n,2) : real.
c
c
c
c call cvinfo (dfs,pse,nbf):
c 
c returns results of sample reuse procedure for estimating optimal smoothing
c parameter df (see above). can only be called if xvalid(ix) was called
c before mars with ix.ne.0 (see above). (Ref[2] Sec 3.6, Ref[3] Sec. 2.3)
c
c output:
c dfs = optimal smoothing parameter estimate.
c pse = estimate of corresponding predictive-squared-error.
c nbf = estimate of associated number of (nonconstant) basis functions.
c
c 
c
      subroutine mars (n,p,x,y,w,nk,mi,lx,fm,im,sp,dp,mm)                   1
      integer p,lx(1),im(1),mm(1)                                           2
      real x(1),y(1),w(1),fm(1),sp(1)                                       3
      double precision dp(1)                                                4
      im(3)=n                                                               5
      im(4)=p                                                               6
      im(5)=nk                                                              7
      im(6)=mi                                                              8
      im(7)=16                                                              9
      im(8)=im(7)+5*nk                                                     10
      im(9)=im(8)+2*nk*mi                                                  11
      im(10)=im(9)+3*(nk+2)                                                12
      im(2)=im(10)+nk*mi-1                                                 13
      im(11)=1                                                             14
      im(12)=2                                                             15
      im(13)=im(12)+5*nk                                                   16
      im(14)=im(13)+1                                                      17
      im(15)=im(14)+nk*(5*mi+1)                                            18
      call mars1(n,p,x,y,w,nk,mi,lx,fm(im(11)),fm(im(12)),fm(im(15)),im(   19
     1im(7)),  im(im(8)),im(im(9)),im(im(10)),fm(im(13)),fm(im(14)),sp,d   20
     1p,mm)                                                                21
      im(1)=im(15)+lcm(p,nk,fm(im(12)),fm(im(15)))-1                       22
      return                                                               23
      end                                                                  24
      subroutine plot (m,x,fm,im,ngc,ngs,icx,nc,crv,ns,srf,sp,mm)          25
      integer im(1),mm(1)                                                  26
      real x(1),fm(1),crv(1),srf(1),sp(1)                                  27
      if(m .ne. 1) go to 1                                                 28
      call plotl(im(3),im(4),x,im(5),im(im(7)),im(im(8)),im(im(9)),im(im   29
     1(10)),  fm(im(12)),fm(im(15)),ngc,ngs,icx,nc,crv,ns,srf,sp,mm)       30
      return                                                               31
    1 call plotc(im(3),im(4),x,im(5),im(im(7)),im(im(8)),im(im(9)),im(im   32
     1(10)),  fm(im(14)),fm(im(15)),ngc,ngs,icx,nc,crv,ns,srf,sp,mm)       33
      return                                                               34
      end                                                                  35
      subroutine catprt (m,fm,im,sp,mm)                                    36
      integer im(1),mm(1)                                                  37
      real fm(1),sp(1)                                                     38
      call ctprt1(m,im(5),im(im(7)),im(im(8)),fm(im(12)),fm(im(15)),fm(i   39
     1m(14)),sp,mm)                                                        40
      return                                                               41
      end                                                                  42
      subroutine slice (flg,xs,x,fm,im,fmn,imn,sp,mm)                      43
      integer im(1),imn(1),mm(1)                                           44
      real xs(1),x(1),fm(1),fmn(1),sp(1)                                   45
      do 1 i=1,15                                                          46
      imn(i)=im(i)                                                         47
    1 continue                                                             48
      i=im(15)                                                             49
      go to 3                                                              50
    2 i=i+1                                                                51
    3 if((i).gt.(im(1))) go to 4                                           52
      fmn(i)=fm(i)                                                         53
      go to 2                                                              54
    4 call slice1(flg,xs,im(3),im(4),x,im(5),fm(im(11)),fm(im(12)),fm(im   55
     1(15)),  im(im(7)),im(im(8)),im(im(9)),im(im(10)),fm(im(13)),fm(im(   56
     114)),  fmn(im(11)),fmn(im(12)),imn(im(7)),imn(im(8)),imn(im(9)),im   57
     1n(im(10)),  fmn(im(13)),fmn(im(14)),sp,mm)                           58
      return                                                               59
      end                                                                  60
      subroutine fmod (m,n,x,fm,im,f,sp)                                   61
      integer im(1)                                                        62
      real x(1),fm(1),f(1),sp(1)                                           63
      if(m .ne. 1) go to 1                                                 64
      call fmrs(n,x,im(5),fm(im(11)),fm(im(12)),fm(im(15)),f)              65
      return                                                               66
    1 call cmrs(n,x,fm(im(15)),im(im(7)),im(im(8)),im(im(9)),im(im(10)),   67
     1  fm(im(13)),fm(im(14)),f,sp)                                        68
      return                                                               69
      end                                                                  70
      subroutine print(it)                                                 71
      call printm(it)                                                      72
      call printg(it)                                                      73
      call printc(it)                                                      74
      call prtslc(it)                                                      75
      return                                                               76
      end                                                                  77
      subroutine setint(i,j,k)                                             78
      parameter(mlist=1000)                                                79
      integer m(2,mlist)                                                   80
      save m                                                               81
      data il /0/                                                          82
      if((i .ne. 0) .and. (j .ne. 0)) go to 1                              83
      il=0                                                                 84
      return                                                               85
    1 if(i.eq.j) return                                                    86
      m1=min0(i,j)                                                         87
      m2=max0(i,j)                                                         88
      if(k .ne. 0) go to 6                                                 89
      l=1                                                                  90
      go to 3                                                              91
    2 l=l+1                                                                92
    3 if((l).gt.(il)) go to 4                                              93
      if(m1.eq.m(1,l).and.m2.eq.m(2,l)) return                             94
      go to 2                                                              95
    4 il=il+1                                                              96
      if(il .le. mlist) go to 5                                            97
      write(6,  '('' increase parameter mlist in subroutine setint to gr   98
     1eater than'',           i5,/,'' and recompile.'')') il               99
      stop                                                                100
    5 m(1,il)=m1                                                          101
      m(2,il)=m2                                                          102
      return                                                              103
    6 ig=0                                                                104
      l=1                                                                 105
      go to 8                                                             106
    7 l=l+1                                                               107
    8 if((l).gt.(il)) go to 10                                            108
      if(m1 .ne. m(1,l) .or. m2 .ne. m(2,l)) go to 7                      109
      ig=1                                                                110
   10 if(ig.eq.0) return                                                  111
      il=il-1                                                             112
      ll=l                                                                113
      go to 12                                                            114
   11 ll=ll+1                                                             115
   12 if((ll).gt.(il)) go to 13                                           116
      m(1,ll)=m(1,ll+1)                                                   117
      m(2,ll)=m(2,ll+1)                                                   118
      go to 11                                                            119
   13 return                                                              120
      entry intlst(it)                                                    121
      if(it.le.0) return                                                  122
      if(il.eq.0) return                                                  123
      write(it,'(/,'' interactions prohibited between:'')')               124
      do 14 l=1,il                                                        125
      write(it,'(''    var('',i3,'')  and  var('',i3,'')'')') m(1,l),m(2  126
     1,l)                                                                 127
   14 continue                                                            128
      return                                                              129
      entry intalw(i,j,k)                                                 130
      k=1                                                                 131
      m1=min0(i,j)                                                        132
      m2=max0(i,j)                                                        133
      l=1                                                                 134
      go to 16                                                            135
   15 l=l+1                                                               136
   16 if((l).gt.(il)) go to 18                                            137
      if(m1 .ne. m(1,l) .or. m2 .ne. m(2,l)) go to 15                     138
      k=0                                                                 139
   18 return                                                              140
      end                                                                 141
      subroutine mars1 (n,p,x,y,w,nk,mi,lx,az,tb,cm,kp,kv,lp,lv,bz,tc,sp  142
     1,dp,mm)                                                             143
      integer p,kp(5,1),kv(2,1),lp(3,1),lv(1),mm(n,1),lx(p)               144
      real x(n,p),y(n),w(n),tb(5,nk),cm(1),tc(1),sp(1)                    145
      double precision dp(1)                                              146
      data ms,df,il,fv,it,ic,ix /0,3.0,0,0.0,6,0,0/                       147
      if(it.gt.0) write(it,11)                                            148
      if(it.gt.0) write(it,10) n,p,nk,ms,mi,df,il,fv,ic                   149
      if(it.gt.0) write(it,12)                                            150
      if(it.gt.0) write(it,'('' var: '',5('' '',20i3,/))') (i,i=1,p)      151
      if(it.gt.0) write(it,'('' flag:'',5('' '',20i3,/))') (lx(i),i=1,p)  152
      call intlst(it)                                                     153
      call nstlst(it)                                                     154
      i1=max0(n*(nk+1),2*n)+1                                             155
      im=i1+n+max0(3*n+5*nk,2*p,4*n,2*n+5*nk+p)                           156
      is=im+p                                                             157
      i2=max0(n*nk,(nk+1)*(nk+1))+1                                       158
      call rspnpr(it,il,n,y,w,mm)                                         159
      do 2 j=1,p                                                          160
      do 1 i=1,n                                                          161
      mm(i,j)=i                                                           162
    1 continue                                                            163
      call psort(x(1,j),mm(1,j),1,n)                                      164
    2 continue                                                            165
      call ordpr(it,n,p,x,lx,mm)                                          166
      call atoscl (n,p,w,x,lx,mm,sp(im),sp(is),cm,x)                      167
      call catpr(it,n,p,x,cm,mm(1,p+1))                                   168
      call oknest(it,p,lx,cm)                                             169
      if(ix.ne.0) call cvmars (ix,n,p,x,y,w,nk,ms,df,fv,mi,lx,it,sp(im),  170
     1sp(is),tb,cm,sp,dp,dp(i2),mm, sp(is+p),sp(is+p+2*n))                171
      call marsgo  (n,p,x,y,w,nk,ms,df,fv,mi,lx,it,sp(im),sp(is),az,tb,c  172
     1m,sp,dp,dp(i2),mm)                                                  173
      if(il .le. 0) go to 6                                               174
      call logitl(n,x,y,w,nk,il,az,tb,cm,sp,dp)                           175
      if(it .le. 0) go to 6                                               176
      sw=0.0                                                              177
      wn=sw                                                               178
      do 3 i=1,n                                                          179
      sw=sw+w(i)                                                          180
      wn=wn+w(i)**2                                                       181
    3 continue                                                            182
      wn=sw**2/wn                                                         183
      ef=1.0                                                              184
      do 4 k=1,nk                                                         185
      if(tb(1,k).ne.0.0) ef=ef+tb(5,k)                                    186
    4 continue                                                            187
      ef=1.0/(1.0-ef/wn)**2                                               188
      s=0.0                                                               189
      t=s                                                                 190
      call fmrs(n,x,nk,az,tb,cm,sp)                                       191
      do 5 i=1,n                                                          192
      yh=1.0/(1.0+exp(-sp(i)))                                            193
      gcv=ef*(y(i)-yh)**2                                                 194
      s=s+w(i)*gcv                                                        195
      t=t+w(i)*yh*(1.0-yh)                                                196
    5 continue                                                            197
      s=s/sw                                                              198
      t=t/sw                                                              199
      write(it,13) s,t                                                    200
    6 if(it .le. 0) go to 7                                               201
      if(il.eq.0) call anova (n,x,y,w,nk,it,tb,cm,lp,lv,sp,dp)            202
      if(il.gt.0) call anoval(n,x,y,w,nk,il,it,az,tb,cm,lp,lv,sp,dp)      203
    7 call ccoll (nk,tb,cm,kp,kv,lp,lv,mm)                                204
      call cubic (n,p,x,y,w,nk,it,tb,cm,kp,kv,lp,lv,bz,tc,sp,sp(i1),sp(i  205
     11+2*p),mm,dp)                                                       206
      if(il .le. 0) go to 9                                               207
      call logitc(n,x,y,w,nk,il,cm,kp,kv,lp,lv,bz,tc,sp,sp(i1+4*n),dp)    208
      if(it .le. 0) go to 9                                               209
      call cmrs(n,x,cm,kp,kv,lp,lv,bz,tc,sp,sp(n+1))                      210
      s=0.0                                                               211
      t=s                                                                 212
      do 8 i=1,n                                                          213
      yh=1.0/(1.0+exp(-sp(i)))                                            214
      gcv=ef*(y(i)-yh)**2                                                 215
      s=s+w(i)*gcv                                                        216
      t=t+w(i)*yh*(1.0-yh)                                                217
    8 continue                                                            218
      s=s/sw                                                              219
      t=t/sw                                                              220
      write(it,14) s,t                                                    221
    9 if(it.gt.0) call varimp (n,p,x,y,w,nk,il,it,az,tb,cm,sp,sp(p+1),dp  222
     1)                                                                   223
      call orgpl(sp(im),sp(is),nk,tb,cm)                                  224
      call orgpc(sp(im),sp(is),lp,lv,tc)                                  225
      call sclato(n,p,x,sp(im),sp(is),cm,x)                               226
      return                                                              227
      entry setms(mal)                                                    228
      ms=mal                                                              229
      return                                                              230
      entry setdf(val)                                                    231
      df=val                                                              232
      return                                                              233
      entry printm(mal)                                                   234
      it=mal                                                              235
      return                                                              236
      entry logit(mal)                                                    237
      il=mal                                                              238
      return                                                              239
      entry setfv(val)                                                    240
      fv=val                                                              241
      return                                                              242
      entry setic(mal)                                                    243
      ic=mal                                                              244
      call stelg(ic)                                                      245
      return                                                              246
      entry xvalid(mal)                                                   247
      ix=mal                                                              248
      call xvmrgo(ix)                                                     249
      return                                                              250
   10 format(/' input parameters (see doc.):',/,  '    n     p    nk      251
     1ms    mi     df    il    fv     ic',/,  ' ',i5,i5,i6,i6,i6,f8.3,i5  252
     1,f7.3,i6)                                                           253
   11 format(//,' MARS modeling, version 3.5 (6/16/91)',/)                254
   12 format(/' predictor variable flags:')                               255
   13 format(/' piecewise-linear logistic gcv =',g12.4,'   ave var =',g1  256
     12.4)                                                                257
   14 format(/' piecewise-cubic logistic gcv =',g12.4,'   ave var =',g12  258
     1.4)                                                                 259
      end                                                                 260
      subroutine plotc (n,p,x,nk,kp,kv,lp,lv,tc,cm,ngc,ngs,icx,nc,crv,ns  261
     1,srf,sp,mm)                                                         262
      integer p,kp(5,1),kv(2,1),lp(3,1),lv(1),mm(1)                       263
      real x(n,p),tb(5,nk),tc(1),cm(1),crv(ngc,2,1),srf(ngs,ngs,1),sp(1)  264
     1,zl(2),zu(2)                                                        265
      data big,it /1.e30,6/                                               266
      if(it.gt.0) write(it,'(/'' mars graphics (piecewise-cubic):'',/)')  267
      jnt=2                                                               268
      go to 1                                                             269
      entry plotl (n,p,x,nk,kp,kv,lp,lv,tb,cm,ngc,ngs,icx,nc,crv,ns,srf,  270
     1sp,mm)                                                              271
      if(it.gt.0) write(it,'(/'' mars graphics (piecewise-linear):'',/)'  272
     1)                                                                   273
      jnt=1                                                               274
    1 ngsq=ngs**2                                                         275
      iz=2*ngsq                                                           276
      d=1.0/(ngs-1)                                                       277
      dc=1.0/(ngc-1)                                                      278
      ll=1                                                                279
      nc=0                                                                280
      ns=nc                                                               281
    2 if(kp(1,ll).lt.0) go to 36                                          282
      if(kp(3,ll) .gt. 0) go to 3                                         283
      ll=ll+1                                                             284
      go to 2                                                             285
    3 nf=kp(3,ll)                                                         286
      k4=kp(4,ll)-1                                                       287
      k1=kp(1,ll)                                                         288
      k2=kp(2,ll)                                                         289
      if(it .le. 0) go to 7                                               290
      if(k1 .ne. 0) go to 4                                               291
      write(it,'('' pure ordinal contribution:'')')                       292
      go to 7                                                             293
    4 write(it,'('' categorical - ordinal interaction:'')')               294
      do 6 i=1,k1                                                         295
      jj=kv(1,k2+i-1)                                                     296
      j=iabs(jj)                                                          297
      k=kv(2,k2+i-1)                                                      298
      ncx=int(cm(2*j+1)+.1)-int(cm(2*j)+.1)+1                             299
      do 5 l=1,ncx                                                        300
      mm(l)=cm(k+l)+.1                                                    301
      if(jj.lt.0) mm(l)=mod(mm(l)+1,2)                                    302
    5 continue                                                            303
      write(it,'('' x('',i3,'') ='',70i1/80i1)') j,(mm(l),l=1,ncx)        304
    6 continue                                                            305
    7 do 35 k=1,nf                                                        306
      l=lp(1,k+k4)                                                        307
      if(l.gt.2) go to 35                                                 308
      ko=lp(2,k+k4)                                                       309
      if(l .ne. 1) go to 17                                               310
      j=0                                                                 311
      jv=lv(ko)                                                           312
      do 9 m=k,nf                                                         313
      l1=lp(1,m+k4)                                                       314
      if(l1.eq.1) go to 9                                                 315
      l2=lp(2,m+k4)-1                                                     316
      do 8 i=1,l1                                                         317
      if(jv.eq.lv(l2+i)) j=1                                              318
    8 continue                                                            319
      if(j.eq.1) go to 10                                                 320
    9 continue                                                            321
   10 if(j.eq.1) go to 35                                                 322
      nc=nc+1                                                             323
      zl(1)=big                                                           324
      zu(1)=-big                                                          325
      do 11 i=1,n                                                         326
      r=x(i,jv)                                                           327
      zl(1)=amin1(zl(1),r)                                                328
      zu(1)=amax1(zu(1),r)                                                329
   11 continue                                                            330
      dl=(zu(1)-zl(1))*dc                                                 331
      do 12 i=1,ngc                                                       332
      crv(i,1,nc)=zl(1)+dl*(i-1)                                          333
   12 continue                                                            334
      if(jnt .ne. 1) go to 13                                             335
      call fun(l,jv,ngc,crv(1,1,nc),nk,tb,cm,k1,kv(1,k2),crv(1,2,nc),mm)  336
      go to 14                                                            337
   13 call cfun (l,jv,ngc,crv(1,1,nc),nf,lp(1,k4+1),lv,tc(kp(5,ll)),  cr  338
     1v(1,2,nc),sp,mm)                                                    339
   14 dl=big                                                              340
      do 15 i=1,ngc                                                       341
      dl=amin1(dl,crv(i,2,nc))                                            342
   15 continue                                                            343
      fx=0.0                                                              344
      do 16 i=1,ngc                                                       345
      crv(i,2,nc)=crv(i,2,nc)-dl                                          346
      fx=amax1(fx,crv(i,2,nc))                                            347
   16 continue                                                            348
      if(it.gt.0) write(it,39) nc,jv,fx                                   349
      go to 35                                                            350
   17 j=0                                                                 351
      mm(1)=lv(ko)                                                        352
      mm(2)=lv(ko+1)                                                      353
      do 19 m=k,nf                                                        354
      l1=lp(1,m+k4)                                                       355
      if(l1.le.2) go to 19                                                356
      l2=lp(2,m+k4)-1                                                     357
      do 18 i=1,l1                                                        358
      if(mm(1).eq.lv(l2+i).or.mm(2).eq.lv(l2+i)) j=1                      359
   18 continue                                                            360
      if(j.eq.1) go to 20                                                 361
   19 continue                                                            362
   20 if(j.eq.1) go to 35                                                 363
      ns=ns+1                                                             364
      zl(1)=big                                                           365
      zl(2)=zl(1)                                                         366
      zu(1)=-big                                                          367
      zu(2)=zu(1)                                                         368
      do 22 j=1,2                                                         369
      do 21 i=1,n                                                         370
      r=x(i,mm(j))                                                        371
      zl(j)=amin1(zl(j),r)                                                372
      zu(j)=amax1(zu(j),r)                                                373
   21 continue                                                            374
   22 continue                                                            375
      do 23 j=1,2                                                         376
      dl=(zu(j)-zl(j))/(ngs-3)                                            377
      zu(j)=zu(j)+dl                                                      378
      zl(j)=zl(j)-dl                                                      379
   23 continue                                                            380
      ne=0                                                                381
      d1=d*(zu(1)-zl(1))                                                  382
      d2=d*(zu(2)-zl(2))                                                  383
      do 25 j=1,ngs                                                       384
      do 24 i=1,ngs                                                       385
      ne=ne+1                                                             386
      sp(iz+ne)=zl(1)+d1*(i-1)                                            387
      sp(iz+ngsq+ne)=zl(2)+d2*(j-1)                                       388
   24 continue                                                            389
   25 continue                                                            390
      dl=big                                                              391
      if(jnt .ne. 1) go to 26                                             392
      call pair(mm,ngsq,sp(iz+1),nk,tb,cm,k1,kv(1,k2),  srf(1,1,ns),sp,m  393
     1m(3))                                                               394
      go to 27                                                            395
   26 call cpair(mm,ngsq,sp(iz+1),nf,lp(1,k4+1),lv,  tc(kp(5,ll)),srf(1,  396
     11,ns),sp)                                                           397
   27 if(icx .le. 0) go to 29                                             398
      call cvxhul(n,x(1,mm(1)),x(1,mm(2)),big,nh,sp)                      399
      if(it .le. 0 .or. 3*nh .lt. iz) go to 28                            400
      nxs=sqrt(float(3*nh)*0.5)+1.1                                       401
      write(it,38) nxs                                                    402
   28 call hulset(ngsq,sp(iz+1),big,nh,sp,srf(1,1,ns))                    403
   29 do 31 j=1,ngs                                                       404
      do 30 i=1,ngs                                                       405
      if(i.eq.1.or.j.eq.1.or.i.eq.ngs.or.j.eq.ngs.or.srf(i,j,ns).ge.big)  406
     1 go to 30                                                           407
      dl=amin1(dl,srf(i,j,ns))                                            408
   30 continue                                                            409
   31 continue                                                            410
      fx=0.0                                                              411
      do 34 j=1,ngs                                                       412
      do 33 i=1,ngs                                                       413
      if((i .ne. 1) .and. ((j .ne. 1) .and. ((i .ne. ngs) .and. ((j .ne.  414
     1 ngs) .and. (srf(i,j,ns) .lt. big))))) go to 32                     415
      srf(i,j,ns)=0.0                                                     416
      go to 33                                                            417
   32 srf(i,j,ns)=srf(i,j,ns)-dl                                          418
      fx=amax1(fx,srf(i,j,ns))                                            419
   33 continue                                                            420
   34 continue                                                            421
      if(it.gt.0) write(it,40) ns,mm(1),mm(2),fx                          422
   35 continue                                                            423
      ll=ll+1                                                             424
      go to 2                                                             425
   36 if(it.gt.0) write(it,37) nc,ns                                      426
      return                                                              427
      entry printg(nal)                                                   428
      it=nal                                                              429
      return                                                              430
   37 format(/,' ',i3,' curves and',i3,' surfaces.'/)                     431
   38 format(' plot: convex hull too large. increase ngs to',i6)          432
   39 format('   crv',i3,':  x(',i2,').  max =',g12.4)                    433
   40 format('   srf',i3,':  x(',i2,'), x(',i2,').  max =',g12.4)         434
      end                                                                 435
      subroutine ctprt1 (m,nk,kp,kv,tb,cm,tc,sc,js)                       436
      integer kp(5,1),kv(2,1),js(1)                                       437
      real cm(1),tc(1),sc(1)                                              438
      data big,it /9.9e30,6/                                              439
      if(it.le.0) return                                                  440
      nc=ncat(kp)                                                         441
      if(nc.eq.0) return                                                  442
      write(it,'(/,'' there are'',i3,'' purely categorical basis functio  443
     1ns.'')') nc                                                         444
      write(it,'('' purely additive and bivariate contributions follow''  445
     1)')                                                                 446
      if(m .ne. 1) go to 1                                                447
      write(it,'('' (piecewise-linear fit):'')')                          448
      go to 2                                                             449
    1 write(it,'('' (piecewise-cubic fit):'')')                           450
    2 call catv(1,kp,kv,nv,js)                                            451
      do 8 jj=1,nv                                                        452
      j=js(jj)                                                            453
      xm=big                                                              454
      xx=-big                                                             455
      nl=int(cm(2*j+1)+.1)-int(cm(2*j)+.1)+1                              456
      do 3 i=1,nl                                                         457
      sc(i)=cvlv(m,1,j,i,nk,kp,kv,tb,cm,tc)                               458
      xm=amin1(xm,sc(i))                                                  459
      xx=amax1(xx,sc(i))                                                  460
    3 continue                                                            461
      px=99.0                                                             462
      if(nl.gt.26) px=9.0                                                 463
      rx=xx-xm                                                            464
      rxp=rx/px                                                           465
      write(it,'(/,'' f( x('',i3,'') ) : scale ='',g12.4)') j,rxp         466
      if(rxp.le.0.0) go to 8                                              467
      do 4 i=1,nl                                                         468
      js(i+nv)=(sc(i)-xm)/rxp+.5                                          469
    4 continue                                                            470
      if(nl .gt. 26) go to 5                                              471
      write(it,28) (i,i=1,nl)                                             472
      write(it,28) (js(i+nv),i=1,nl)                                      473
      go to 8                                                             474
    5 if(nl .gt. 38) go to 6                                              475
      write(it,29) (i,i=1,nl)                                             476
      write(it,29) (js(i+nv),i=1,nl)                                      477
      go to 8                                                             478
    6 if(nl .gt. 78) go to 7                                              479
      write(it,30) (mod(i,10),i=1,nl)                                     480
      write(it,30) (js(i+nv),i=1,nl)                                      481
      go to 8                                                             482
    7 write(it,37) 78                                                     483
    8 continue                                                            484
      call catv(2,kp,kv,nv,js)                                            485
      do 27 jj=1,nv                                                       486
      j1=js(2*jj-1)                                                       487
      j2=js(2*jj)                                                         488
      xm=big                                                              489
      xx=-big                                                             490
      n1=int(cm(2*j1+1)+.1)-int(cm(2*j1)+.1)+1                            491
      n2=int(cm(2*j2+1)+.1)-int(cm(2*j2)+.1)+1                            492
      k=0                                                                 493
      do 10 i=1,n1                                                        494
      s1=cvlv(m,1,j1,i,nk,kp,kv,tb,cm,tc)                                 495
      js(2*nv+1)=i                                                        496
      do 9 j=1,n2                                                         497
      js(2*nv+2)=j                                                        498
      k=k+1                                                               499
      sc(k)=s1+cvlv(m,2,js(2*jj-1),js(2*nv+1),nk,kp,kv,tb,cm,tc)          500
    9 continue                                                            501
   10 continue                                                            502
      do 12 j=1,n2                                                        503
      s1=cvlv(m,1,j2,j,nk,kp,kv,tb,cm,tc)                                 504
      do 11 i=1,n1                                                        505
      k=j+n2*(i-1)                                                        506
      sc(k)=sc(k)+s1                                                      507
   11 continue                                                            508
   12 continue                                                            509
      k=0                                                                 510
      do 14 i=1,n1                                                        511
      do 13 j=1,n2                                                        512
      k=k+1                                                               513
      x=sc(k)                                                             514
      xx=amax1(xx,x)                                                      515
      xm=amin1(xm,x)                                                      516
   13 continue                                                            517
   14 continue                                                            518
      na=min0(n1,n2)                                                      519
      nb=max0(n1,n2)                                                      520
      if(na .ne. n1) go to 15                                             521
      ja=j1                                                               522
      jb=j2                                                               523
      go to 16                                                            524
   15 ja=j2                                                               525
      jb=j1                                                               526
   16 px=99.0                                                             527
      if(na.gt.25) px=9.0                                                 528
      rx=xx-xm                                                            529
      rxp=rx/px                                                           530
      write(it,'(/,'' f( x('',i3,''), x('',i3,'') ) : scale ='',g12.4)')  531
     1ja,jb,rxp                                                           532
      if(rxp.le.0.0) go to 27                                             533
      if(na .le. 75) go to 17                                             534
      write(it,37) 75                                                     535
      go to 27                                                            536
   17 if(na .gt. 25) go to 18                                             537
      write(it,34) (i,i=1,na)                                             538
      go to 20                                                            539
   18 if(na .gt. 37) go to 19                                             540
      write(it,35) (i,i=1,na)                                             541
      go to 20                                                            542
   19 write(it,36) (mod(i,10),i=1,na)                                     543
   20 do 26 j=1,nb                                                        544
      do 23 i=1,na                                                        545
      if(na .ne. n1) go to 21                                             546
      k=j+n2*(i-1)                                                        547
      go to 22                                                            548
   21 k=i+n2*(j-1)                                                        549
   22 js(i+2*nv)=(sc(k)-xm)/rxp+.5                                        550
   23 continue                                                            551
      if(na .gt. 25) go to 24                                             552
      write(it,31) j,(js(i+2*nv),i=1,na)                                  553
      go to 26                                                            554
   24 if(na .gt. 37) go to 25                                             555
      write(it,32) j,(js(i+2*nv),i=1,na)                                  556
      go to 26                                                            557
   25 write(it,33) j,(js(i+2*nv),i=1,na)                                  558
   26 continue                                                            559
   27 continue                                                            560
      return                                                              561
      entry printc(nal)                                                   562
      it=nal                                                              563
      return                                                              564
   28 format(' ',26i3)                                                    565
   29 format(' ',38i2)                                                    566
   30 format(' ',78i1)                                                    567
   31 format(' ',i3,' ',25i3)                                             568
   32 format(' ',i3,' ',37i2)                                             569
   33 format(' ',i3,' ',75i1)                                             570
   34 format('     ',25i3)                                                571
   35 format('     ',37i2)                                                572
   36 format('     ',75i1)                                                573
   37 format(' function not printed (more than',i3,' categorical values)  574
     1.')                                                                 575
      end                                                                 576
      subroutine slice1 (flg,xs,n,p,x,nk,az,tb,cm,kp,kv,lp,lv,bz,tc,azn,  577
     1tbn,kpn,kvn,  lpn,lvn,bzn,tcn,sp,mm)                                578
      integer p,kp(5,1),kv(2,1),lp(3,1),lv(1),kpn(5,1),kvn(2,1),lpn(3,1)  579
     1,lvn(1),mm(1)                                                       580
      real xs(p),x(n,p),tb(5,nk),cm(1),tc(1),tbn(5,nk),tcn(1),sp(1)       581
      data it,big /6,9.9e30/                                              582
      ni=0                                                                583
      do 1 m=1,nk                                                         584
      if(tb(1,m).ne.0.0) ni=ni+1                                          585
    1 continue                                                            586
      if(ni .ne. 0) go to 3                                               587
      kpn(1,1)=-1                                                         588
      lpn(1,1)=0                                                          589
      do 2 m=1,nk                                                         590
      tbn(1,m)=0.0                                                        591
    2 continue                                                            592
      azn=0.0                                                             593
      bzn=azn                                                             594
      if(it.gt.0) write(it,'('' slice: original mars model = constant.''  595
     1)')                                                                 596
      return                                                              597
    3 if(it .le. 0) go to 5                                               598
      write(it,'(/,'' sliced mars model: flag ='',g12.4)') flg            599
      write(it,'(/,'' slice:'')')                                         600
      do 4 j=1,p                                                          601
      if(xs(j).eq.flg) go to 4                                            602
      write(it,'('' x('',i3,'') ='',g12.4)') j,xs(j)                      603
    4 continue                                                            604
    5 i1=2*nk+1                                                           605
      i2=i1+2*p                                                           606
      i3=max0(i2+p,i1+nk)                                                 607
      do 7 j=1,p                                                          608
      xl=big                                                              609
      xr=-xl                                                              610
      do 6 i=1,n                                                          611
      xl=amin1(xl,x(i,j))                                                 612
      xr=amax1(xr,x(i,j))                                                 613
    6 continue                                                            614
      sp(j+i3-1)=xr-xl                                                    615
      sp(j+i3-1+p)=xl                                                     616
    7 continue                                                            617
      call reducq(flg,xs,nk,tb,cm,tc,kp,kv,lp,lv,sp(i3),sp,sp(i1),sp(i2)  618
     1)                                                                   619
      call reducl(flg,xs,nk,az,tb,cm,bz,sp,sp(i3),azn,tbn,bzn,sp(i1))     620
      ni=0                                                                621
      do 8 m=1,nk                                                         622
      if(tbn(1,m).ne.0.0) ni=ni+1                                         623
    8 continue                                                            624
      if(ni .ne. 0) go to 10                                              625
      kpn(1,1)=-1                                                         626
      lpn(1,1)=0                                                          627
      do 9 m=1,nk                                                         628
      tbn(1,m)=0.0                                                        629
    9 continue                                                            630
      azn=0.0                                                             631
      bzn=azn                                                             632
      if(it.gt.0) write(it,'('' sliced mars model = constant.'')')        633
      return                                                              634
   10 if(it.gt.0) call slova(nk,it,tbn,ni,lpn,lvn)                        635
      call ccoll(nk,tbn,cm,kpn,kvn,lpn,lvn,mm)                            636
      call qslice(p,nk,tbn,cm,sp,kpn,kvn,lpn,lvn,tcn,sp(i3),sp(i1),mm)    637
      return                                                              638
      entry prtslc(ig)                                                    639
      it=ig                                                               640
      return                                                              641
      end                                                                 642
      subroutine cmrs (n,x,cm,kp,kv,lp,lv,bz,tc,y,sc)                     643
      integer kp(5,1),kv(2,1),lp(3,1),lv(1)                               644
      real x(n,1),tc(1),cm(1),y(n),sc(n,2)                                645
      data ifg /0/                                                        646
      do 1 i=1,n                                                          647
      y(i)=bz                                                             648
    1 continue                                                            649
      ll=1                                                                650
      la=ll                                                               651
      l1=la                                                               652
    2 if(kp(1,ll).lt.0) go to 19                                          653
      do 3 i=1,n                                                          654
      sc(i,1)=1.0                                                         655
    3 continue                                                            656
      if(kp(1,ll) .le. 0) go to 11                                        657
      jl=kp(1,ll)                                                         658
      do 10 il=1,jl                                                       659
      k=kp(2,ll)+il-1                                                     660
      jj=kv(1,k)                                                          661
      j=iabs(jj)                                                          662
      kk=kv(2,k)                                                          663
      do 9 i=1,n                                                          664
      if(sc(i,1).eq.0.0) go to 9                                          665
      if(ifg .ne. 0) go to 4                                              666
      ic=icat(x(i,j),j,cm)                                                667
      go to 5                                                             668
    4 ic=x(i,j)+.1                                                        669
    5 if(ic .ne. 0) go to 6                                               670
      sc(i,1)=0.0                                                         671
      go to 7                                                             672
    6 sc(i,1)=cm(ic+kk)                                                   673
    7 if(jj .ge. 0) go to 9                                               674
      if(sc(i,1) .ne. 0.0) go to 8                                        675
      sc(i,1)=1.0                                                         676
      go to 9                                                             677
    8 sc(i,1)=0.0                                                         678
    9 continue                                                            679
   10 continue                                                            680
      go to 12                                                            681
   11 if(kp(3,ll) .gt. 0) go to 12                                        682
      ll=ll+1                                                             683
      go to 2                                                             684
   12 if(kp(3,ll) .ge. 0) go to 14                                        685
      k=-kp(3,ll)                                                         686
      ll=ll+1                                                             687
      do 13 i=1,n                                                         688
      if(sc(i,1).eq.0.0) go to 13                                         689
      y(i)=y(i)+tc(k)                                                     690
   13 continue                                                            691
      go to 2                                                             692
   14 kp3=kp(3,ll)                                                        693
      do 18 m=1,kp3                                                       694
      l=lp(1,l1)                                                          695
      nt=lp(3,l1)                                                         696
      lb=la+5*l*nt-1                                                      697
      do 17 j=1,nt                                                        698
      do 15 i=1,n                                                         699
      sc(i,2)=sc(i,1)                                                     700
   15 continue                                                            701
      call que(j,l,nt,lv(lp(2,l1)),n,x,tc(la),sc(1,2))                    702
      do 16 i=1,n                                                         703
      y(i)=y(i)+tc(lb+j)*sc(i,2)                                          704
   16 continue                                                            705
   17 continue                                                            706
      la=lb+nt+1                                                          707
      l1=l1+1                                                             708
   18 continue                                                            709
      ll=ll+1                                                             710
      go to 2                                                             711
   19 return                                                              712
      entry stcmrs(i1)                                                    713
      ifg=i1                                                              714
      return                                                              715
      end                                                                 716
      subroutine fmrs (n,x,nk,az,tb,cm,y)                                 717
      real x(n,1),tb(5,nk),cm(1),y(n)                                     718
      double precision s                                                  719
      data ifg /0/                                                        720
      do 13 i=1,n                                                         721
      s=az                                                                722
      do 12 m=1,nk                                                        723
      if(tb(1,m).eq.0.0) go to 12                                         724
      phi=1.0                                                             725
      ip=m                                                                726
    1 if(ip.le.0) go to 11                                                727
      t=tb(2,ip)                                                          728
      j=abs(t)+.1                                                         729
      if(cm(2*j) .le. 0.0) go to 8                                        730
      if(ifg .ne. 0) go to 2                                              731
      k=icat(x(i,j),j,cm)                                                 732
      go to 3                                                             733
    2 k=x(i,j)+.1                                                         734
    3 if(k .ne. 0) go to 4                                                735
      u=0.0                                                               736
      go to 5                                                             737
    4 u=cm(k+int(tb(3,ip)+.1))                                            738
    5 if(t .ge. 0.0) go to 9                                              739
      if(u .ne. 0.0) go to 6                                              740
      u=1.0                                                               741
      go to 9                                                             742
    6 u=0.0                                                               743
      go to 9                                                             744
    8 u=amax1(0.0,sign(1.0,t)*(x(i,j)-tb(3,ip)))                          745
    9 if(u .ne. 0.0) go to 10                                             746
      phi=0.0                                                             747
      go to 11                                                            748
   10 phi=phi*u                                                           749
      ip=tb(4,ip)+.1                                                      750
      go to 1                                                             751
   11 s=s+tb(1,m)*phi                                                     752
   12 continue                                                            753
      y(i)=s                                                              754
   13 continue                                                            755
      return                                                              756
      entry stfmrs(i1)                                                    757
      ifg=i1                                                              758
      return                                                              759
      end                                                                 760
      subroutine marsgo (n,p,x,y,w,nk,ms,df,fv,mi,lx,it,xm,xs,az,tb,cm,s  761
     1c,db,d,mm)                                                          762
      integer p,mm(n,p),lx(p)                                             763
      logical elg                                                         764
      real x(n,p),y(n),w(n),xm(p),xs(p),tb(5,nk),cm(1),sc(n,1),vcst(3),t  765
     1x(5)                                                                766
      double precision db(n,1),d(nk,1)                                    767
      double precision yb,yv,sw,s,t,u,v,we,sy,a,b,xb,xx,xd,ssq,alr        768
      double precision dx,wn,se,tt,txt,xt,st,su,yc,eps,rsq,dy,dv          769
      character*28 hol                                                    770
      data ix,alr,eps,big,fln,nmin,alf,vcst  /0,1.d-7,1.d-4,9.9e30,-1.0,  771
     15,.05,1.0,.666667,.333333/                                          772
      if(it.gt.0) write(it,89)                                            773
      mk=nk                                                               774
      df1=0.0                                                             775
      nep=0                                                               776
      do 1 j=1,p                                                          777
      if(lx(j).eq.0) go to 1                                              778
      if(x(mm(1,j),j).ge.x(mm(n,j),j)) go to 1                            779
      nep=nep+1                                                           780
      cst=vcst(iabs(lx(j)))                                               781
      if(mi.eq.1) cst=amin1(cst,vcst(2))                                  782
      df1=df1+cst                                                         783
    1 continue                                                            784
      if(nep .ne. 0) go to 2                                              785
      if(it.gt.0) write(it,'('' no predictor variables.'')')              786
      stop                                                                787
    2 if(nep.eq.1) df1=vcst(3)                                            788
      cfac=df1/nep                                                        789
      df1=df*cfac                                                         790
      mkp1=mk+1                                                           791
      mkp2=mk+2                                                           792
      sw=0.d0                                                             793
      wn=sw                                                               794
      yb=wn                                                               795
      s=yb                                                                796
      do 3 i=1,n                                                          797
      sw=sw+w(i)                                                          798
      wn=wn+w(i)**2                                                       799
      yb=yb+w(i)*y(i)                                                     800
    3 continue                                                            801
      yb=yb/sw                                                            802
      wn=sw**2/wn                                                         803
      do 4 i=1,n                                                          804
      s=s+w(i)*(y(i)-yb)**2                                               805
    4 continue                                                            806
      yv=s/sw                                                             807
      tcst=1.0                                                            808
      tcmx=wn-df1*vcst(1)-2.0                                             809
      if(cm(1) .le. 0.0) go to 7                                          810
      i=2                                                                 811
      go to 6                                                             812
    5 i=i+(2)                                                             813
    6 if((2)*((i)-(2*p)).gt.0) go to 7                                    814
      if(cm(i).gt.0.0) kcp0=cm(i+1)+.1                                    815
      go to 5                                                             816
    7 m=0                                                                 817
      mtot=m                                                              818
      txm=yv/(1.d0-1.d0/wn)**2                                            819
      rsq=yv*sw                                                           820
      kr=0                                                                821
      nopt=0                                                              822
      if(it.gt.0) write(it,90) m,txm,0.0,1.0                              823
      if(fln.lt.0.0) fln=1.0+4.0/wn                                       824
    8 if(m.ge.mk.or.tcst.ge.tcmx) go to 61                                825
      nopt=nopt+1                                                         826
      mm1=m                                                               827
      m=m+1                                                               828
      txi=big                                                             829
      kcp=kcp0                                                            830
      l=0                                                                 831
      go to 10                                                            832
    9 l=l+1                                                               833
   10 if((l).gt.(mm1)) go to 46                                           834
      if(nnord(l,tb).ge.mi) go to 9                                       835
      call blf0(l,0,n,x,w,cm,sc,nnt,sc(1,mkp1))                           836
      lbf=0                                                               837
      if(nnt.le.nmin) go to 9                                             838
      nep=0                                                               839
      do 11 jp=1,p                                                        840
      if(x(mm(1,jp),jp).ge.x(mm(n,jp),jp)) go to 11                       841
      if(jf(l,jp,tb).ne.0) go to 11                                       842
      call isfac(l,jp,mm1,tb,cm,ja)                                       843
      if(ja.lt.0) go to 11                                                844
      if(.not.elg(jp,l,lx,tb,cm)) go to 11                                845
      nep=nep+1                                                           846
   11 continue                                                            847
      if(nep.eq.0) go to 9                                                848
      call mnspan(ms,alf,nep,nnt,mn,me,mel)                               849
      if(nnt.le.max0(me,mel)) go to 9                                     850
      do 45 jp=1,p                                                        851
      if(x(mm(1,jp),jp).ge.x(mm(n,jp),jp)) go to 45                       852
      if(jf(l,jp,tb).ne.0) go to 45                                       853
      call isfac(l,jp,mm1,tb,cm,ja)                                       854
      if(ja.lt.0) go to 45                                                855
      if(.not.elg(jp,l,lx,tb,cm)) go to 45                                856
      if(ja .ne. 0) go to 13                                              857
      if(lbf .eq. 0) go to 14                                             858
      call blf0(l,0,n,x,w,cm,sc,nnt,sc(1,mkp1))                           859
      lbf=0                                                               860
      call mnspan(ms,alf,nep,nnt,mn,me,mel)                               861
      go to 14                                                            862
   13 call blf0(l,ja,n,x,w,cm,sc,nnt,sc(1,mkp1))                          863
      lbf=1                                                               864
      if(nnt.le.nmin) go to 45                                            865
      call mnspan(ms,alf,nep,nnt,mn,me,mel)                               866
      if(nnt.le.max0(me,mel)) go to 45                                    867
   14 fvr=1.0                                                             868
      if(jft(mm1,jp,tb).eq.0) fvr=1.0+fv                                  869
      ict=0                                                               870
      if(lx(jp) .ge. 0) go to 15                                          871
      ict=1                                                               872
      nc=int(cm(2*jp+1)+.1)-int(cm(2*jp)+.1)+1                            873
      call csp(jp,nc,m,n,x,y,w,nk,tb,cm,kcp,yb,d,kr,nnt,  sw,me,mkp2,nop  874
     1,sc(1,mkp1),db,d(1,3),mm(1,p+1))                                    875
      if(nop.eq.0) go to 45                                               876
      go to 39                                                            877
   15 tb(2,m)=jp                                                          878
      tb(3,m)=x(mm(1,jp),jp)                                              879
      tb(4,m)=l                                                           880
      k1=kr                                                               881
      ssq=rsq                                                             882
      call update(1,n,m,kr,x,y,w,sw,yb,tb,cm,sc,sc(1,mkp1),db,d,d(1,3))   883
      if(kr .le. k1) go to 16                                             884
      rsq=rsq-d(kr,1)**2                                                  885
      tb(1,m)=rsq/sw                                                      886
      go to 17                                                            887
   16 tb(1,m)=big                                                         888
   17 if((lx(jp) .ne. 3) .and. ((m .lt. mk) .and. (nnt .gt. me+mel))) go  889
     1 to 20                                                              890
      tb(1,m)=rsq/sw                                                      891
      if(fvr*tb(1,m) .gt. txi .or. newb(m,tb) .ne. 0) go to 19            892
      txi=fvr*tb(1,m)                                                     893
      tx(1)=tb(1,m)                                                       894
      do 18 i=2,4                                                         895
      tx(i)=tb(i,m)                                                       896
   18 continue                                                            897
      jas=ja                                                              898
   19 kr=k1                                                               899
      rsq=ssq                                                             900
      go to 45                                                            901
   20 mm1=m                                                               902
      m=m+1                                                               903
      tb(1,m)=big                                                         904
      xa=0.0                                                              905
      j=n                                                                 906
      nnl=nnt                                                             907
      nst=0                                                               908
      nnr=-1                                                              909
   21 j0=j                                                                910
   22 mj=mm(j,jp)                                                         911
      h=sc(mj,mkp1)                                                       912
      if(w(mj) .le. 0.0 .or. h .le. 0.0) go to 23                         913
      nst=nst+1                                                           914
      nnl=nnl-1                                                           915
      nnr=nnr+1                                                           916
   23 if(x(mm(j-1,jp),jp).lt.x(mm(j,jp),jp) .and.nst.ge.mn.and.nnl.ge.me  917
     1l.and.nnr.ge.me) go to 24                                           918
      j=j-1                                                               919
      if(j.le.1) go to 24                                                 920
      go to 22                                                            921
   24 if(j.le.1) go to 39                                                 922
      nst=0                                                               923
      xb=xa                                                               924
      xa=x(mm(j,jp),jp)                                                   925
      if(j0 .ne. n) go to 28                                              926
      v=0.d0                                                              927
      u=v                                                                 928
      t=u                                                                 929
      we=t                                                                930
      se=we                                                               931
      sy=se                                                               932
      dy=sy                                                               933
      i=1                                                                 934
      go to 26                                                            935
   25 i=i+1                                                               936
   26 if((i).gt.(kr)) go to 27                                            937
      d(i,2)=0.d0                                                         938
      d(i,3)=d(i,2)                                                       939
      go to 25                                                            940
   27 txt=x(mm(1,jp),jp)+x(mm(n,jp),jp)                                   941
      xt=0.5*txt                                                          942
      go to 31                                                            943
   28 dx=xb-xa                                                            944
      dy=dy+dx*sy                                                         945
      we=we+dx*se                                                         946
      v=v+dx*(2.d0*u-(xb+xa-txt)*t)                                       947
      i=1                                                                 948
      go to 30                                                            949
   29 i=i+1                                                               950
   30 if((i).gt.(kr)) go to 31                                            951
      d(i,2)=d(i,2)+dx*d(i,3)                                             952
      go to 29                                                            953
   31 do 34 k=j,j0                                                        954
      mj=mm(k,jp)                                                         955
      h=sc(mj,mkp1)                                                       956
      if(w(mj).le.0.0.or.h.le.0.0) go to 34                               957
      xx=x(mj,jp)                                                         958
      xd=xx-xa                                                            959
      su=w(mj)*h                                                          960
      st=su*xd                                                            961
      yc=y(mj)-yb                                                         962
      dy=dy+st*yc                                                         963
      sy=sy+su*yc                                                         964
      we=we+st                                                            965
      se=se+su                                                            966
      sj=w(mj)*h**2                                                       967
      v=v+sj*xd**2                                                        968
      t=t+sj                                                              969
      u=u+sj*(xx-xt)                                                      970
      i=1                                                                 971
      go to 33                                                            972
   32 i=i+1                                                               973
   33 if((i).gt.(kr)) go to 34                                            974
      tt=db(mj,i)                                                         975
      d(i,2)=d(i,2)+st*tt                                                 976
      d(i,3)=d(i,3)+su*tt                                                 977
      go to 32                                                            978
   34 continue                                                            979
      dv=v-we**2/sw                                                       980
      if(dv .le. 0.d0) go to 38                                           981
      a=0.d0                                                              982
      b=a                                                                 983
      i=1                                                                 984
      go to 36                                                            985
   35 i=i+1                                                               986
   36 if((i).gt.(kr)) go to 37                                            987
      s=d(i,2)                                                            988
      a=a+s*d(i,1)                                                        989
      b=b+s**2                                                            990
      go to 35                                                            991
   37 b=dv-b                                                              992
      if(b .le. eps*dv) go to 38                                          993
      b=-(dy-a)**2/b                                                      994
      if(b .ge. tb(1,m)) go to 38                                         995
      tb(1,m)=b                                                           996
      tb(3,m)=xa                                                          997
   38 j=j-1                                                               998
      if(j.le.1) go to 39                                                 999
      go to 21                                                           1000
   39 tb(2,m)=jp                                                         1001
      tb(4,m)=l                                                          1002
      tb(1,m)=(rsq+tb(1,m))/sw                                           1003
      if(ict .ne. 0 .or. tb(1,mm1) .gt. fln*tb(1,m)) go to 40            1004
      mp=mm1                                                             1005
      go to 41                                                           1006
   40 mp=m                                                               1007
   41 if(fvr*tb(1,mp) .ge. txi .or. newb(mp,tb) .ne. 0) go to 44         1008
      txi=fvr*tb(1,mp)                                                   1009
      tx(1)=tb(1,mp)                                                     1010
      do 42 i=2,4                                                        1011
      tx(i)=tb(i,mp)                                                     1012
   42 continue                                                           1013
      jas=ja                                                             1014
      if(ict .eq. 0) go to 44                                            1015
      do 43 i=1,nc                                                       1016
      cm(kcp0+i)=cm(kcp+i)                                               1017
   43 continue                                                           1018
      kcp=kcp0+nc                                                        1019
      tx(3)=kcp0                                                         1020
   44 if(ict .ne. 0) go to 45                                            1021
      m=mm1                                                              1022
      mm1=m-1                                                            1023
      kr=k1                                                              1024
      rsq=ssq                                                            1025
   45 continue                                                           1026
      go to 9                                                            1027
   46 jp=tx(2)+.1                                                        1028
      if(cm(2*jp) .le. 0.) go to 47                                      1029
      nc=int(cm(2*jp+1)+.1)-int(cm(2*jp)+.1)+1                           1030
      kcp0=kcp0+nc                                                       1031
   47 if(jas .le. 0) go to 53                                            1032
      call getnst(jas,cm,jn,kcp,cm(kcp0+1))                              1033
      tb(2,m)=jn                                                         1034
      tb(3,m)=kcp0                                                       1035
      kcp0=kcp0+kcp                                                      1036
      tb(4,m)=tx(4)                                                      1037
      k1=kr                                                              1038
      call blf(int(tx(4)+.1),n,sc,sc(1,mkp1))                            1039
      tx(4)=m                                                            1040
      call update(2,n,m,kr,x,y,w,sw,yb,tb,cm,sc,sc(1,mkp1),db,d,d(1,3))  1041
      if(kr.gt.k1) rsq=rsq-d(kr,1)**2                                    1042
      if(m .ge. mk) go to 51                                             1043
      m=m+1                                                              1044
      tb(2,m)=-tb(2,m-1)                                                 1045
      do 48 i=3,4                                                        1046
      tb(i,m)=tb(i,m-1)                                                  1047
   48 continue                                                           1048
      if(ibfext(m,tb,cm) .eq. 0) go to 49                                1049
      m=m-1                                                              1050
      go to 51                                                           1051
   49 do 50 i=1,n                                                        1052
      sc(i,m)=phi(m,i,n,x,tb,cm)                                         1053
   50 continue                                                           1054
   51 if(it .le. 0) go to 52                                             1055
      mp=m-1                                                             1056
      tcst=(nopt-1)*df1+kr+1.0                                           1057
      fjn=jn                                                             1058
      fkr=kr                                                             1059
      gcv=(rsq/sw)/(1.d0-tcst/wn)**2                                     1060
      call holl(jn,cm,tb(3,m),hol)                                       1061
      if(m.eq.mtot+1) write(it,92) m,gcv,fkr,tcst,fjn,hol,tb(4,m)        1062
      if(m.eq.mtot+2) write(it,91) m,mp,gcv,fkr,tcst,fjn,hol,tb(4,m)     1063
   52 mtot=m                                                             1064
      m=m+1                                                              1065
      if(m.gt.mk) go to 61                                               1066
   53 do 54 i=1,5                                                        1067
      tb(i,m)=tx(i)                                                      1068
   54 continue                                                           1069
      k1=kr                                                              1070
      call blf(int(tx(4)+.1),n,sc,sc(1,mkp1))                            1071
      call update(2,n,m,kr,x,y,w,sw,yb,tb,cm,sc,sc(1,mkp1),db,d,d(1,3))  1072
      if(kr.gt.k1) rsq=rsq-d(kr,1)**2                                    1073
      if(m .ge. mk .or. (cm(2*jp) .le. 0.0) .and. (tx(3) .le. x(mm(1,jp) 1074
     1,jp))) go to 58                                                    1075
      m=m+1                                                              1076
      do 55 i=1,4                                                        1077
      tb(i,m)=tx(i)                                                      1078
   55 continue                                                           1079
      tb(2,m)=-tb(2,m)                                                   1080
      if(cm(2*jp) .le. 0.0) go to 57                                     1081
      do 56 i=1,n                                                        1082
      sc(i,m)=phi(m,i,n,x,tb,cm)                                         1083
   56 continue                                                           1084
      go to 58                                                           1085
   57 k1=kr                                                              1086
      call update(2,n,m,kr,x,y,w,sw,yb,tb,cm,sc,sc(1,mkp1),db,d,d(1,3))  1087
      if(kr.gt.k1) rsq=rsq-d(kr,1)**2                                    1088
   58 tcst=nopt*df1+kr+1.0                                               1089
      if(it .le. 0) go to 60                                             1090
      mp=m-1                                                             1091
      jp=abs(tx(2))+.1                                                   1092
      fkr=kr                                                             1093
      gcv=(rsq/sw)/(1.d0-tcst/wn)**2                                     1094
      if(cm(2*jp) .le. 0.0) go to 59                                     1095
      call holl(jp,cm,tx(3),hol)                                         1096
      if(m.eq.mtot+1) write(it,92) m,gcv,fkr,tcst,tx(2),hol,tx(4)        1097
      if(m.eq.mtot+2) write(it,91) m,mp,gcv,fkr,tcst,tx(2),hol,tx(4)     1098
      go to 60                                                           1099
   59 xk=xm(jp)+xs(jp)*tx(3)                                             1100
      if(m.eq.mtot+1) write(it,85) m,gcv,fkr,tcst,tx(2),xk,tx(4)         1101
      if(m.eq.mtot+2) write(it,86) m,mp,gcv,fkr,tcst,tx(2),xk,tx(4)      1102
   60 mtot=m                                                             1103
      go to 8                                                            1104
   61 mk=min0(m,mk)                                                      1105
      m=mk+1                                                             1106
      k=m                                                                1107
      go to 63                                                           1108
   62 k=k+1                                                              1109
   63 if((k).gt.(nk)) go to 64                                           1110
      tb(1,k)=0.0                                                        1111
      go to 62                                                           1112
   64 call sscp(n,m,sc,y,w,yb,yv,sw,db,d)                                1113
      call lsf1(db,m,d,yb,alr,b,d(1,2),a,d(1,3))                         1114
      nli=0                                                              1115
      do 65 k=1,mk                                                       1116
      if(d(k,2).ne.0.d0) nli=nli+1                                       1117
   65 continue                                                           1118
      df1=df1*nopt+nli                                                   1119
      tcst=df1+1.0                                                       1120
      df1=df1/nli                                                        1121
      do 66 k=1,nk                                                       1122
      tb(5,k)=df1                                                        1123
   66 continue                                                           1124
      asm=(b/sw)/(1.d0-tcst/wn)**2                                       1125
      tcsts=tcst                                                         1126
      az=a                                                               1127
      do 67 k=1,mk                                                       1128
      tb(1,k)=0.0                                                        1129
      if(d(k,2).ne.0.d0) tb(1,k)=d(k,2)                                  1130
   67 continue                                                           1131
      if(ix .eq. 0) go to 73                                             1132
      sc(1,1)=(cfac*nopt)/nli                                            1133
      sc(2,1)=wn                                                         1134
      sc(3,1)=yv                                                         1135
      sc(4,1)=yb                                                         1136
      do 72 k=nli,nk                                                     1137
      call array(k+4,n,i,j)                                              1138
      sc(i,j)=b/sw                                                       1139
      k1=k*(nk+1)+3                                                      1140
      l=0                                                                1141
      go to 69                                                           1142
   68 l=l+1                                                              1143
   69 if((l).gt.(nk)) go to 72                                           1144
      k1=k1+1                                                            1145
      call array(k1,n,i,j)                                               1146
      if(l .ne. 0) go to 70                                              1147
      sc(i,j)=a                                                          1148
      go to 68                                                           1149
   70 if(l .le. mk) go to 71                                             1150
      sc(i,j)=0.0                                                        1151
      go to 68                                                           1152
   71 sc(i,j)=d(l,2)                                                     1153
      go to 68                                                           1154
   72 continue                                                           1155
      call array((nk+1)**2+4,n,i,j)                                      1156
      sc(i,j)=mk                                                         1157
      kl=nli                                                             1158
   73 do 80 ll=2,nli                                                     1159
      call bkstp(db,m,d,yb,alr,b,d(1,2),a,k,d(1,3))                      1160
      if(k.eq.0) go to 81                                                1161
      if(ix .eq. 0) go to 78                                             1162
      call array(kl+3,n,i,j)                                             1163
      sc(i,j)=b/sw                                                       1164
      kl=kl-1                                                            1165
      k1=kl*(nk+1)+3                                                     1166
      l=0                                                                1167
      go to 75                                                           1168
   74 l=l+1                                                              1169
   75 if((l).gt.(nk)) go to 78                                           1170
      k1=k1+1                                                            1171
      call array(k1,n,i,j)                                               1172
      if(l .ne. 0) go to 76                                              1173
      sc(i,j)=a                                                          1174
      go to 74                                                           1175
   76 if(l .le. mk) go to 77                                             1176
      sc(i,j)=0.0                                                        1177
      go to 74                                                           1178
   77 sc(i,j)=d(l,2)                                                     1179
      go to 74                                                           1180
   78 tcst=tcst-df1                                                      1181
      b=(b/sw)/(1.d0-tcst/wn)**2                                         1182
      if(b.ge.asm) go to 80                                              1183
      asm=b                                                              1184
      tcsts=tcst                                                         1185
      az=a                                                               1186
      do 79 i=1,mk                                                       1187
      tb(1,i)=0.0                                                        1188
      if(d(i,2).ne.0.d0) tb(1,i)=d(i,2)                                  1189
   79 continue                                                           1190
   80 continue                                                           1191
   81 if(txm .gt. asm) go to 83                                          1192
      asm=txm                                                            1193
      tcsts=1.0                                                          1194
      az=yb                                                              1195
      do 82 i=1,nk                                                       1196
      tb(1,i)=0.0                                                        1197
   82 continue                                                           1198
   83 if(it .le. 0) go to 84                                             1199
      write(it,87)                                                       1200
      call coefpr(it,mk,az,tb,cm,xs)                                     1201
      write(it,88) asm,tcsts                                             1202
   84 return                                                             1203
      entry setfln(val)                                                  1204
      fln=val                                                            1205
      return                                                             1206
      entry setalf(val)                                                  1207
      alf=val                                                            1208
      return                                                             1209
      entry setmin(nal)                                                  1210
      nmin=nal                                                           1211
      return                                                             1212
      entry setcta(val)                                                  1213
      vcst(2)=val                                                        1214
      return                                                             1215
      entry setctl(val)                                                  1216
      vcst(3)=val                                                        1217
      return                                                             1218
      entry xvmrgo(nal)                                                  1219
      ix=nal                                                             1220
      return                                                             1221
      entry setalr(val)                                                  1222
      alr=val                                                            1223
      return                                                             1224
   85 format('   ',i3,'    ',g12.4,2('   ',f5.1),'       ',f3.0,  '      1225
     1 ',g12.4,'          ',f3.0)                                        1226
   86 format(' ',i3,' ',i3,'  ',g12.4,2('   ',f5.1),'       ',f3.0,  '   1227
     1    ',g12.4,'          ',f3.0)                                     1228
   87 format(/,' final model after backward stepwise elimination:')      1229
   88 format(/,'   (piecewise linear) gcv = ',g12.4,'   #efprms = ',f5.1 1230
     1)                                                                  1231
   89 format(//,' forward stepwise knot placement:',//  '  basfn(s)    g 1232
     1cv      #indbsfns  #efprms',  '   variable      knot            pa 1233
     1rent')                                                             1234
   90 format('   ',i3,'    ',g12.4,2('   ',f5.1))                        1235
   91 format(' ',i3,' ',i3,'  ',g12.4,2('   ',f5.1),'       ',f3.0,a28,f 1236
     13.0)                                                               1237
   92 format('   ',i3,'    ',g12.4,2('   ',f5.1),'       ',f3.0,a28,f3.0 1238
     1)                                                                  1239
      end                                                                1240
      subroutine atoscl(n,p,w,x,lx,mm,xm,xs,cm,z)                        1241
      integer p,lx(p),mm(n,p)                                            1242
      real w(n),x(n,p),z(n,p),xm(p),xs(p),cm(1)                          1243
      double precision s,t,sw                                            1244
      sw=0.d0                                                            1245
      do 1 j=1,n                                                         1246
      sw=sw+w(j)                                                         1247
    1 continue                                                           1248
      ip=0                                                               1249
      nct=ip                                                             1250
      do 12 i=1,p                                                        1251
      if(lx(i) .ne. 0) go to 2                                           1252
      xm(i)=0.0                                                          1253
      xs(i)=xm(i)                                                        1254
      go to 12                                                           1255
    2 if(lx(i) .ge. 0) go to 8                                           1256
      nc=0                                                               1257
      xm(i)=ip                                                           1258
      j=1                                                                1259
      nct=nct+1                                                          1260
    3 j0=j                                                               1261
      if(j .ge. n) go to 5                                               1262
    4 if(x(mm(j+1,i),i).gt.x(mm(j,i),i)) go to 5                         1263
      j=j+1                                                              1264
      if(j.ge.n) go to 5                                                 1265
      go to 4                                                            1266
    5 ip=ip+1                                                            1267
      cm(ip)=x(mm(j,i),i)                                                1268
      nc=nc+1                                                            1269
      do 6 k=j0,j                                                        1270
      z(mm(k,i),i)=nc                                                    1271
    6 continue                                                           1272
      j=j+1                                                              1273
      if(j.gt.n) go to 7                                                 1274
      go to 3                                                            1275
    7 xs(i)=nc                                                           1276
      go to 12                                                           1277
    8 s=0.d0                                                             1278
      t=s                                                                1279
      do 9 j=1,n                                                         1280
      s=s+w(j)*x(j,i)                                                    1281
    9 continue                                                           1282
      s=s/sw                                                             1283
      xm(i)=s                                                            1284
      do 10 j=1,n                                                        1285
      z(j,i)=x(j,i)-s                                                    1286
      t=t+w(j)*z(j,i)**2                                                 1287
   10 continue                                                           1288
      xs(i)=1.0                                                          1289
      if(t.le.0.d0) go to 12                                             1290
      t=dsqrt(t/sw)                                                      1291
      xs(i)=t                                                            1292
      t=1.d0/t                                                           1293
      do 11 j=1,n                                                        1294
      z(j,i)=t*z(j,i)                                                    1295
   11 continue                                                           1296
   12 continue                                                           1297
      n2=2*p+1                                                           1298
      if(nct .ne. 0) go to 14                                            1299
      do 13 i=1,n2                                                       1300
      cm(i)=0.0                                                          1301
   13 continue                                                           1302
      return                                                             1303
   14 n2p1=n2+1                                                          1304
      i=ip                                                               1305
      go to 16                                                           1306
   15 i=i+(-1)                                                           1307
   16 if((-1)*((i)-(1)).gt.0) go to 17                                   1308
      cm(i+n2)=cm(i)                                                     1309
      go to 15                                                           1310
   17 j=0                                                                1311
      i=2                                                                1312
      go to 19                                                           1313
   18 i=i+(2)                                                            1314
   19 if((2)*((i)-(n2)).gt.0) go to 22                                   1315
      j=j+1                                                              1316
      if(lx(j) .ge. 0) go to 20                                          1317
      cm(i)=xm(j)+n2p1                                                   1318
      cm(i+1)=cm(i)+xs(j)-1.0                                            1319
      go to 18                                                           1320
   20 cm(i)=0.0                                                          1321
      cm(i+1)=cm(i)                                                      1322
      go to 18                                                           1323
   22 cm(1)=nct                                                          1324
      call stfmrs(1)                                                     1325
      call stcmrs(1)                                                     1326
      return                                                             1327
      end                                                                1328
      subroutine orgpl (xm,xs,nk,tb,cm)                                  1329
      real xm(1),xs(1),tb(5,nk),cm(1)                                    1330
      do 1 m=1,nk                                                        1331
      j=abs(tb(2,m))+.1                                                  1332
      if(cm(2*j).gt.0.0) go to 1                                         1333
      tb(3,m)=xm(j)+xs(j)*tb(3,m)                                        1334
    1 continue                                                           1335
      do 4 m=1,nk                                                        1336
      if(tb(1,m).eq.0.0) go to 4                                         1337
      scl=1.0                                                            1338
      ip=m                                                               1339
    2 if(ip.le.0) go to 3                                                1340
      j=abs(tb(2,ip))+.1                                                 1341
      if(cm(2*j).eq.0.0) scl=scl*xs(j)                                   1342
      ip=tb(4,ip)+.1                                                     1343
      go to 2                                                            1344
    3 tb(1,m)=tb(1,m)/scl                                                1345
    4 continue                                                           1346
      return                                                             1347
      end                                                                1348
      subroutine anova (n,x,y,w,nk,it,tb,cm,lp,lv,t,d)                   1349
      integer lp(3,1),lv(1)                                              1350
      real x(n,1),y(n),w(n),tb(5,nk),cm(1),t(n,nk)                       1351
      double precision d(nk,1),s,u,sw,yv,wn,yb                           1352
      if(it.le.0) return                                                 1353
      nkp1=nk+1                                                          1354
      nkp2=nkp1+1                                                        1355
      nkp3=nkp2+1                                                        1356
      lm=nkp3+nk                                                         1357
      sw=0.d0                                                            1358
      wn=sw                                                              1359
      s=wn                                                               1360
      u=s                                                                1361
      do 1 i=1,n                                                         1362
      sw=sw+w(i)                                                         1363
      wn=wn+w(i)**2                                                      1364
      s=s+w(i)*y(i)                                                      1365
    1 continue                                                           1366
      s=s/sw                                                             1367
      yb=s                                                               1368
      wn=sw**2/wn                                                        1369
      do 2 i=1,n                                                         1370
      u=u+w(i)*(y(i)-s)**2                                               1371
    2 continue                                                           1372
      yv=u/sw                                                            1373
      eft=1.0                                                            1374
      do 3 m=1,nk                                                        1375
      if(tb(1,m).ne.0.0) eft=eft+tb(5,m)                                 1376
    3 continue                                                           1377
      ni=0                                                               1378
      do 9 m=1,nk                                                        1379
      if(tb(1,m).eq.0.0) go to 9                                         1380
      ni=ni+1                                                            1381
      s=0.d0                                                             1382
      do 4 j=1,n                                                         1383
      t(j,ni)=phi(m,j,n,x,tb,cm)                                         1384
      s=s+w(j)*t(j,ni)                                                   1385
    4 continue                                                           1386
      s=s/sw                                                             1387
      do 5 j=1,n                                                         1388
      t(j,ni)=t(j,ni)-s                                                  1389
    5 continue                                                           1390
      do 7 i=1,ni                                                        1391
      s=0.d0                                                             1392
      do 6 j=1,n                                                         1393
      s=s+w(j)*t(j,i)*t(j,ni)                                            1394
    6 continue                                                           1395
      d(i,ni)=s                                                          1396
    7 continue                                                           1397
      s=0.d0                                                             1398
      do 8 j=1,n                                                         1399
      s=s+w(j)*t(j,ni)*(y(j)-yb)                                         1400
    8 continue                                                           1401
      d(ni,nkp1)=s                                                       1402
      d(ni,nkp2)=tb(1,m)                                                 1403
      lv(ni)=m                                                           1404
    9 continue                                                           1405
      if(ni .ne. 0) go to 10                                             1406
      write(it,26)                                                       1407
      return                                                             1408
   10 do 11 m=1,ni                                                       1409
      t(m,1)=lv(m)                                                       1410
   11 continue                                                           1411
      write(it,24) ni                                                    1412
      call coll(nk,tb,lp,lv,lp(1,nkp1))                                  1413
      m=1                                                                1414
   12 if(lp(1,m).eq.0) go to 13                                          1415
      m=m+1                                                              1416
      go to 12                                                           1417
   13 na=m-1                                                             1418
      m=1                                                                1419
      nim1=ni-1                                                          1420
      if(na .ne. 1) go to 14                                             1421
      k2=lp(2,m)                                                         1422
      i2=lp(1,m)+k2-1                                                    1423
      efm=eft-1.0                                                        1424
      u=yv/(1.d0-1.d0/wn)**2                                             1425
      s=sqrt(varf(nk,d,d(1,nkp2),sw,1,ni))                               1426
      write(it,25) m,s,u,lp(3,m),efm,(lv(i),i=k2,i2)                     1427
      return                                                             1428
   14 do 23 m=1,na                                                       1429
      k2=lp(2,m)                                                         1430
      l=lp(1,m)                                                          1431
      i2=l+k2-1                                                          1432
      ll=k2-1                                                            1433
      np=ni                                                              1434
      do 19 im=1,ni                                                      1435
      i=t(im,1)+.1                                                       1436
      if(nord(i,tb) .eq. l) go to 15                                     1437
      t(im,2)=0.0                                                        1438
      go to 19                                                           1439
   15 k=0                                                                1440
      do 16 j=1,l                                                        1441
      if(jf(i,lv(ll+j),tb).eq.1) go to 16                                1442
      k=1                                                                1443
      go to 17                                                           1444
   16 continue                                                           1445
   17 if(k .ne. 1) go to 18                                              1446
      t(im,2)=0.0                                                        1447
      go to 19                                                           1448
   18 t(im,2)=1.0                                                        1449
      np=np-1                                                            1450
   19 continue                                                           1451
   20 k=0                                                                1452
      do 21 i=1,nim1                                                     1453
      if(t(i,2) .le. t(i+1,2)) go to 21                                  1454
      k=1                                                                1455
      call exch(nk,ni,i,d,t,t(1,2))                                      1456
   21 continue                                                           1457
      if(k.eq.0) go to 22                                                1458
      go to 20                                                           1459
   22 call lsf(nk,np,nkp1,0.d0,d,d(1,lm),s,u,d(1,nkp3),1)                1460
      efm=efp(lp(1,m),lv(lp(2,m)),nk,tb)                                 1461
      u=(u/sw+yv)/(1.d0-(eft-efm)/wn)**2                                 1462
      s=sqrt(varf(nk,d,d(1,nkp2),sw,np+1,ni))                            1463
      write(it,25) m,s,u,lp(3,m),efm,(lv(i),i=k2,i2)                     1464
   23 continue                                                           1465
      return                                                             1466
   24 format(/' anova decomposition on',i3,' basis functions:',/  '  fun 1467
     1. std. dev.     -gcv    #bsfns  #efprms  variable(s)')             1468
   25 format(' ',i3,' ',2g12.4,'  ',i2,'      ',f4.1,'  ',20i4)          1469
   26 format(/' estimated optimal model = response mean.')               1470
      end                                                                1471
      subroutine anoval (n,x,y,w,nk,il,it,az,tb,cm,lp,lv,sc,d)           1472
      integer lp(3,1),lv(1)                                              1473
      real x(n,1),y(n),w(n),tb(5,nk),cm(1),sc(n,1)                       1474
      double precision d(nk,1),sw,yv,wn,yb                               1475
      if(it.le.0) return                                                 1476
      sw=0.d0                                                            1477
      yb=sw                                                              1478
      yv=yb                                                              1479
      wn=yv                                                              1480
      do 1 i=1,n                                                         1481
      sw=sw+w(i)                                                         1482
      wn=wn+w(i)**2                                                      1483
      yb=yb+w(i)*y(i)                                                    1484
    1 continue                                                           1485
      yb=yb/sw                                                           1486
      wn=sw**2/wn                                                        1487
      do 2 i=1,n                                                         1488
      yv=yv+w(i)*(y(i)-yb)**2                                            1489
    2 continue                                                           1490
      yv=yv/sw                                                           1491
      eft=1.0                                                            1492
      ni=0                                                               1493
      do 3 m=1,nk                                                        1494
      if(tb(1,m).eq.0.0) go to 3                                         1495
      ni=ni+1                                                            1496
      eft=eft+tb(5,m)                                                    1497
    3 continue                                                           1498
      if(ni .ne. 0) go to 4                                              1499
      write(it,14)                                                       1500
      return                                                             1501
    4 write(it,12) ni                                                    1502
      call coll(nk,tb,lp,lv,lp(1,nk+1))                                  1503
      m=1                                                                1504
    5 if(lp(1,m).eq.0) go to 6                                           1505
      m=m+1                                                              1506
      go to 5                                                            1507
    6 na=m-1                                                             1508
      m=1                                                                1509
      if(na .ne. 1) go to 7                                              1510
      k2=lp(2,m)                                                         1511
      i2=lp(1,m)+k2-1                                                    1512
      efm=eft-1.0                                                        1513
      u=yv/(1.d0-1.d0/wn)**2                                             1514
      write(it,13) m,u,lp(3,m),efm,(lv(i),i=k2,i2)                       1515
      return                                                             1516
    7 ip=nk+4                                                            1517
      do 11 m=1,na                                                       1518
      k2=lp(2,m)                                                         1519
      l=lp(1,m)                                                          1520
      i2=l+k2-1                                                          1521
      ll=k2-1                                                            1522
      call cptb(nk,tb,sc(1,ip))                                          1523
      do 10 i=1,nk                                                       1524
      if(tb(1,i).eq.0.0) go to 10                                        1525
      if(nord(i,tb).ne.l) go to 10                                       1526
      k=0                                                                1527
      do 8 j=1,l                                                         1528
      if(jf(i,lv(ll+j),tb).eq.1) go to 8                                 1529
      k=1                                                                1530
      go to 9                                                            1531
    8 continue                                                           1532
    9 if(k.eq.1) go to 10                                                1533
      call setz(i,sc(1,ip))                                              1534
   10 continue                                                           1535
      a0=az                                                              1536
      call vp(n,x,y,w,nk,il,yb,sw,a0,sc(1,ip),cm,u,sc,d)                 1537
      efm=efp(lp(1,m),lv(lp(2,m)),nk,tb)                                 1538
      u=u/(1.d0-(eft-efm)/wn)**2                                         1539
      write(it,13) m,u,lp(3,m),efm,(lv(i),i=k2,i2)                       1540
   11 continue                                                           1541
      return                                                             1542
   12 format(/' logit anova decomposition on',i3,' basis functions:',/   1543
     1'  fun.    -gcv    #bsfns  #efprms  variable(s)')                  1544
   13 format(' ',i3,' ',g12.4,'   ',i2,'     ',f4.1,'    ',20i4)         1545
   14 format(/' estimated optimal model = response mean.')               1546
      end                                                                1547
      subroutine cptb(nk,tb,ub)                                          1548
      real tb(5,nk),ub(5,1)                                              1549
      do 2 m=1,nk                                                        1550
      do 1 k=1,5                                                         1551
      ub(k,m)=tb(k,m)                                                    1552
    1 continue                                                           1553
    2 continue                                                           1554
      return                                                             1555
      entry setz(l,ub)                                                   1556
      ub(1,l)=0.0                                                        1557
      return                                                             1558
      end                                                                1559
      subroutine fun (l,jv,n,x,nk,tb,cm,jl,kv,t,js)                      1560
      integer jv(l),kv(2,jl),js(1)                                       1561
      real x(n,l),tb(5,nk),cm(1),t(n)                                    1562
      double precision s                                                 1563
      do 8 i=1,n                                                         1564
      s=0.d0                                                             1565
      do 7 m=1,nk                                                        1566
      if(icf(m,tb,cm,jl,kv,js).eq.0) go to 7                             1567
      if(nordc(1,m,tb,cm).ne.l) go to 7                                  1568
      k=0                                                                1569
      do 1 j=1,l                                                         1570
      if(jf(m,jv(j),tb).eq.1) go to 1                                    1571
      k=1                                                                1572
      go to 2                                                            1573
    1 continue                                                           1574
    2 if(k.eq.1) go to 7                                                 1575
      phi=1.0                                                            1576
      ip=m                                                               1577
    3 if(ip.le.0) go to 6                                                1578
      u=tb(2,ip)                                                         1579
      j=abs(u)+.1                                                        1580
      if(cm(2*j) .eq. 0.0) go to 4                                       1581
      ip=tb(4,ip)+.1                                                     1582
      go to 3                                                            1583
    4 do 5 k=1,l                                                         1584
      if(j.eq.jv(k)) j=k                                                 1585
    5 continue                                                           1586
      phi=phi*amax1(0.0,sign(1.0,u)*(x(i,j)-tb(3,ip)))                   1587
      ip=tb(4,ip)+.1                                                     1588
      go to 3                                                            1589
    6 s=s+tb(1,m)*phi                                                    1590
    7 continue                                                           1591
      t(i)=s                                                             1592
    8 continue                                                           1593
      return                                                             1594
      end                                                                1595
      subroutine cubic (n,p,x,y,w,nk,it,tb,cm,kp,kv,lp,lv,bz,tc,t,z,sc,j 1596
     1s,d)                                                               1597
      integer p,kp(5,1),kv(2,1),lp(3,1),lv(1),js(1)                      1598
      real x(n,p),y(n),w(n),tb(5,nk),cm(1),tc(1),t(n,nk),z(2,p),sc(n)    1599
      double precision d(nk,1),s,u,sw,yb,wn,yv                           1600
      data big /9.9e30/                                                  1601
      yb=0.d0                                                            1602
      sw=yb                                                              1603
      wn=sw                                                              1604
      yv=wn                                                              1605
      do 1 i=1,n                                                         1606
      sw=sw+w(i)                                                         1607
      wn=wn+w(i)**2                                                      1608
      yb=yb+w(i)*y(i)                                                    1609
    1 continue                                                           1610
      yb=yb/sw                                                           1611
      wn=sw**2/wn                                                        1612
      do 2 i=1,n                                                         1613
      yv=yv+w(i)*(y(i)-yb)**2                                            1614
    2 continue                                                           1615
      yv=yv/sw                                                           1616
      ni=0                                                               1617
      do 3 m=1,nk                                                        1618
      if(tb(1,m).ne.0.0) ni=ni+1                                         1619
    3 continue                                                           1620
      if(ni .ne. 0) go to 4                                              1621
      bz=yb                                                              1622
      u=yv/(1.0-1.0/wn)**2                                               1623
      if(it.gt.0) write(it,34) ni,u                                      1624
      return                                                             1625
    4 nkp1=nk+1                                                          1626
      nkp2=nk+2                                                          1627
      nkp3=nk+3                                                          1628
      lm=nkp3+nk                                                         1629
      do 6 i=1,p                                                         1630
      xl=big                                                             1631
      xr=-xl                                                             1632
      do 5 j=1,n                                                         1633
      xl=amin1(xl,x(j,i))                                                1634
      xr=amax1(xr,x(j,i))                                                1635
    5 continue                                                           1636
      z(1,i)=xl                                                          1637
      z(2,i)=xr                                                          1638
    6 continue                                                           1639
      ll=1                                                               1640
      la=ll                                                              1641
      l1=la                                                              1642
      lt=0                                                               1643
    7 if(kp(1,ll).lt.0) go to 20                                         1644
      do 8 i=1,n                                                         1645
      sc(i)=1.0                                                          1646
    8 continue                                                           1647
      if(kp(1,ll) .le. 0) go to 12                                       1648
      jl=kp(1,ll)                                                        1649
      do 11 il=1,jl                                                      1650
      k=kp(2,ll)+il-1                                                    1651
      jj=kv(1,k)                                                         1652
      j=iabs(jj)                                                         1653
      kk=kv(2,k)                                                         1654
      do 10 i=1,n                                                        1655
      if(sc(i).eq.0.0) go to 10                                          1656
      ic=x(i,j)+.1                                                       1657
      sc(i)=cm(ic+kk)                                                    1658
      if(jj .ge. 0) go to 10                                             1659
      if(sc(i) .ne. 0.0) go to 9                                         1660
      sc(i)=1.0                                                          1661
      go to 10                                                           1662
    9 sc(i)=0.0                                                          1663
   10 continue                                                           1664
   11 continue                                                           1665
      go to 13                                                           1666
   12 if(kp(3,ll) .gt. 0) go to 13                                       1667
      ll=ll+1                                                            1668
      go to 7                                                            1669
   13 if(kp(3,ll) .gt. 0) go to 15                                       1670
      lt=lt+1                                                            1671
      kp(5,ll)=0                                                         1672
      do 14 i=1,n                                                        1673
      t(i,lt)=sc(i)                                                      1674
   14 continue                                                           1675
      go to 19                                                           1676
   15 kp3=kp(3,ll)                                                       1677
      kp(5,ll)=la                                                        1678
      do 18 m=1,kp3                                                      1679
      l=lp(1,l1)                                                         1680
      nt=lp(3,l1)                                                        1681
      call knts(l,nt,lv(lp(2,l1)),kp(1,ll),kv(1,kp(2,ll)),  nk,tb,cm,tc( 1682
     1la),js)                                                            1683
      call side(l,nt,lv(lp(2,l1)),z,tc(la))                              1684
      do 17 jp=1,nt                                                      1685
      lt=lt+1                                                            1686
      do 16 i=1,n                                                        1687
      t(i,lt)=sc(i)                                                      1688
   16 continue                                                           1689
      call que(jp,l,nt,lv(lp(2,l1)),n,x,tc(la),t(1,lt))                  1690
   17 continue                                                           1691
      l1=l1+1                                                            1692
      la=la+nt*(5*l+1)                                                   1693
   18 continue                                                           1694
   19 ll=ll+1                                                            1695
      go to 7                                                            1696
   20 do 26 j=1,lt                                                       1697
      s=0.d0                                                             1698
      u=s                                                                1699
      do 21 i=1,n                                                        1700
      s=s+w(i)*t(i,j)                                                    1701
   21 continue                                                           1702
      s=s/sw                                                             1703
      d(j,nkp2)=s                                                        1704
      do 22 i=1,n                                                        1705
      t(i,j)=t(i,j)-s                                                    1706
   22 continue                                                           1707
      s=0.d0                                                             1708
      do 23 i=1,n                                                        1709
      s=s+w(i)*(y(i)-yb)*t(i,j)                                          1710
   23 continue                                                           1711
      d(j,nkp1)=s                                                        1712
      do 25 k=1,j                                                        1713
      s=0.d0                                                             1714
      do 24 i=1,n                                                        1715
      s=s+w(i)*t(i,k)*t(i,j)                                             1716
   24 continue                                                           1717
      d(k,j)=s                                                           1718
   25 continue                                                           1719
   26 continue                                                           1720
      call lsf(nk,lt,nkp1,yb,d,d(1,lm),s,u,d(1,nkp3),1)                  1721
      eft=1.0                                                            1722
      do 27 i=1,nk                                                       1723
      if(tb(1,i).ne.0.0) eft=eft+tb(5,i)                                 1724
   27 continue                                                           1725
      u=(u/sw+yv)/(1.0-eft/wn)**2                                        1726
      bz=s                                                               1727
      ll=1                                                               1728
      l1=ll                                                              1729
      le=la-1                                                            1730
      la=0                                                               1731
      lt=la                                                              1732
   28 if(kp(1,ll).lt.0) go to 33                                         1733
      if(kp(1,ll) .ne. 0 .or. kp(3,ll) .gt. 0) go to 29                  1734
      ll=ll+1                                                            1735
      go to 28                                                           1736
   29 if(kp(3,ll) .gt. 0) go to 30                                       1737
      le=le+1                                                            1738
      kp(3,ll)=-le                                                       1739
      lt=lt+1                                                            1740
      tc(le)=d(lt,lm)                                                    1741
      ll=ll+1                                                            1742
      go to 28                                                           1743
   30 kp3=kp(3,ll)                                                       1744
      do 32 m=1,kp3                                                      1745
      nt=lp(3,l1)                                                        1746
      la=la+5*lp(1,l1)*nt                                                1747
      do 31 i=1,nt                                                       1748
      lt=lt+1                                                            1749
      tc(i+la)=d(lt,lm)                                                  1750
   31 continue                                                           1751
      la=la+nt                                                           1752
      l1=l1+1                                                            1753
   32 continue                                                           1754
      ll=ll+1                                                            1755
      go to 28                                                           1756
   33 if(it.gt.0) write(it,34) lt,u                                      1757
      return                                                             1758
   34 format(/' piecewise cubic fit on',i3,' basis functions, gcv =',g12 1759
     1.4)                                                                1760
      end                                                                1761
      subroutine cfun (l,jv,n,x,nf,lp,lv,tc,t,sc,jw)                     1762
      integer jv(l),lp(3,1),lv(1),jw(l)                                  1763
      real x(n,l),tc(1),t(n),sc(n)                                       1764
      do 1 i=1,n                                                         1765
      t(i)=0.0                                                           1766
    1 continue                                                           1767
      la=1                                                               1768
      do 10 l1=1,nf                                                      1769
      if(lp(1,l1).ne.l) go to 9                                          1770
      l2=lp(2,l1)-1                                                      1771
      do 3 j=1,l                                                         1772
      m=0                                                                1773
      do 2 k=1,l                                                         1774
      if(jv(j).eq.lv(k+l2)) m=1                                          1775
    2 continue                                                           1776
      if(m.eq.0) go to 9                                                 1777
    3 continue                                                           1778
      nt=lp(3,l1)                                                        1779
      lb=la+5*l*nt-1                                                     1780
      do 8 j=1,nt                                                        1781
      do 5 k=1,l                                                         1782
      do 4 i=1,l                                                         1783
      if(lv(k+l2).eq.jv(i)) jw(k)=i                                      1784
    4 continue                                                           1785
    5 continue                                                           1786
      do 6 i=1,n                                                         1787
      sc(i)=1.0                                                          1788
    6 continue                                                           1789
      call que(j,l,nt,jw,n,x,tc(la),sc)                                  1790
      do 7 i=1,n                                                         1791
      t(i)=t(i)+tc(lb+j)*sc(i)                                           1792
    7 continue                                                           1793
    8 continue                                                           1794
      go to 11                                                           1795
    9 la=la+lp(3,l1)*(5*lp(1,l1)+1)                                      1796
   10 continue                                                           1797
   11 return                                                             1798
      end                                                                1799
      subroutine orgpc (xm,xs,lp,lv,tc)                                  1800
      integer lp(3,1),lv(1)                                              1801
      real xm(1),xs(1),tc(1)                                             1802
      la=1                                                               1803
      l1=la                                                              1804
    1 if(lp(1,l1).eq.0) go to 3                                          1805
      l=lp(1,l1)                                                         1806
      nt=lp(3,l1)                                                        1807
      lb=la+5*l*nt-1                                                     1808
      do 2 j=1,nt                                                        1809
      call scpc(xm,xs,j,l,nt,lv(lp(2,l1)),tc(la),tc(lb+j))               1810
    2 continue                                                           1811
      la=lb+nt+1                                                         1812
      l1=l1+1                                                            1813
      go to 1                                                            1814
    3 return                                                             1815
      end                                                                1816
      subroutine pair (jv,n,x,nk,tb,cm,jl,kv,f,sc,js)                    1817
      integer jv(2),kv(2,jl),js(1)                                       1818
      real x(n,1),tb(5,nk),cm(1),f(n),sc(n)                              1819
      call fun(2,jv,n,x,nk,tb,cm,jl,kv,f,js)                             1820
      do 2 k=1,2                                                         1821
      call fun(1,jv(k),n,x(1,k),nk,tb,cm,jl,kv,sc,js)                    1822
      do 1 i=1,n                                                         1823
      f(i)=f(i)+sc(i)                                                    1824
    1 continue                                                           1825
    2 continue                                                           1826
      return                                                             1827
      end                                                                1828
      subroutine cpair (jv,n,x,nf,lp,lv,tc,f,sc)                         1829
      integer jv(2),lp(3,1),lv(1),jw(2)                                  1830
      real x(n,1),tc(1),f(n),sc(n,2)                                     1831
      call cfun(2,jv,n,x,nf,lp,lv,tc,f,sc,jw)                            1832
      do 2 k=1,2                                                         1833
      call cfun(1,jv(k),n,x(1,k),nf,lp,lv,tc,sc,sc(1,2),jw)              1834
      do 1 i=1,n                                                         1835
      f(i)=f(i)+sc(i,1)                                                  1836
    1 continue                                                           1837
    2 continue                                                           1838
      return                                                             1839
      end                                                                1840
      subroutine logitl (n,x,y,w,nk,il,az,tb,cm,sc,d)                    1841
      integer kp(5,1),kv(2,1),lp(3,1),lv(1)                              1842
      real x(n,1),y(n),w(n),tb(5,nk),cm(1),sc(n,1),tc(1),ss(n)           1843
      double precision d(nk,1),a,b,s,sw,yb                               1844
      data niter,wm,thr /30,0.0001,0.0001/                               1845
      do 2 i=1,n                                                         1846
      k=0                                                                1847
      do 1 m=1,nk                                                        1848
      if(tb(1,m).eq.0.0) go to 1                                         1849
      k=k+1                                                              1850
      sc(i,k)=phi(m,i,n,x,tb,cm)                                         1851
    1 continue                                                           1852
    2 continue                                                           1853
      if(k .ne. 0) go to 3                                               1854
      az=alog(az/(1.0-az))                                               1855
      return                                                             1856
    3 mk=k                                                               1857
      a=az                                                               1858
      jnt=1                                                              1859
      go to 19                                                           1860
      entry logitc (n,x,y,w,nk,il,cm,kp,kv,lp,lv,bz,tc,sc,ss,d)          1861
      ll=1                                                               1862
      la=ll                                                              1863
      l1=la                                                              1864
      lt=0                                                               1865
    4 if(kp(1,ll).lt.0) go to 17                                         1866
      do 5 i=1,n                                                         1867
      ss(i)=1.0                                                          1868
    5 continue                                                           1869
      if(kp(1,ll) .le. 0) go to 9                                        1870
      jl=kp(1,ll)                                                        1871
      do 8 il=1,jl                                                       1872
      k=kp(2,ll)+il-1                                                    1873
      jj=kv(1,k)                                                         1874
      j=iabs(jj)                                                         1875
      kk=kv(2,k)                                                         1876
      do 7 i=1,n                                                         1877
      if(ss(i).eq.0.0) go to 7                                           1878
      ic=x(i,j)+.1                                                       1879
      ss(i)=cm(ic+kk)                                                    1880
      if(jj .ge. 0) go to 7                                              1881
      if(ss(i) .ne. 0.0) go to 6                                         1882
      ss(i)=1.0                                                          1883
      go to 7                                                            1884
    6 ss(i)=0.0                                                          1885
    7 continue                                                           1886
    8 continue                                                           1887
      go to 10                                                           1888
    9 if(kp(3,ll) .gt. 0) go to 10                                       1889
      ll=ll+1                                                            1890
      go to 4                                                            1891
   10 if(kp(3,ll) .gt. 0) go to 12                                       1892
      lt=lt+1                                                            1893
      do 11 i=1,n                                                        1894
      sc(i,lt)=ss(i)                                                     1895
   11 continue                                                           1896
      go to 16                                                           1897
   12 kp3=kp(3,ll)                                                       1898
      do 15 m=1,kp3                                                      1899
      l=lp(1,l1)                                                         1900
      nt=lp(3,l1)                                                        1901
      do 14 jp=1,nt                                                      1902
      lt=lt+1                                                            1903
      do 13 i=1,n                                                        1904
      sc(i,lt)=ss(i)                                                     1905
   13 continue                                                           1906
      call que(jp,l,nt,lv(lp(2,l1)),n,x,tc(la),sc(1,lt))                 1907
   14 continue                                                           1908
      l1=l1+1                                                            1909
      la=la+nt*(5*l+1)                                                   1910
   15 continue                                                           1911
   16 ll=ll+1                                                            1912
      go to 4                                                            1913
   17 if(lt .ne. 0) go to 18                                             1914
      bz=alog(bz/(1.0-bz))                                               1915
      return                                                             1916
   18 mk=lt                                                              1917
      a=bz                                                               1918
      jnt=2                                                              1919
   19 mkp1=mk+1                                                          1920
      mkp2=mk+2                                                          1921
      mkp3=mk+3                                                          1922
      mkp4=mk+4                                                          1923
      iter=0                                                             1924
      if(jnt .ne. 1) go to 21                                            1925
      k=0                                                                1926
      do 20 m=1,nk                                                       1927
      if(tb(1,m).eq.0.0) go to 20                                        1928
      k=k+1                                                              1929
      d(k,mkp3)=tb(1,m)                                                  1930
   20 continue                                                           1931
      go to 27                                                           1932
   21 ll=1                                                               1933
      l1=ll                                                              1934
      la=0                                                               1935
      lt=la                                                              1936
   22 if(kp(1,ll).lt.0) go to 27                                         1937
      if(kp(1,ll) .ne. 0 .or. kp(3,ll) .gt. 0) go to 23                  1938
      ll=ll+1                                                            1939
      go to 22                                                           1940
   23 if(kp(3,ll) .gt. 0) go to 24                                       1941
      lt=lt+1                                                            1942
      d(lt,mkp3)=tc(-kp(3,ll))                                           1943
      ll=ll+1                                                            1944
      go to 22                                                           1945
   24 kp3=kp(3,ll)                                                       1946
      do 26 m=1,kp3                                                      1947
      nt=lp(3,l1)                                                        1948
      la=la+5*lp(1,l1)*nt                                                1949
      do 25 i=1,nt                                                       1950
      lt=lt+1                                                            1951
      d(lt,mkp3)=tc(i+la)                                                1952
   25 continue                                                           1953
      la=la+nt                                                           1954
      l1=l1+1                                                            1955
   26 continue                                                           1956
      ll=ll+1                                                            1957
      go to 22                                                           1958
   27 iter=iter+1                                                        1959
      b=0.d0                                                             1960
      sw=b                                                               1961
      yb=sw                                                              1962
      do 29 i=1,n                                                        1963
      s=a                                                                1964
      do 28 m=1,mk                                                       1965
      s=s+d(m,mkp3)*sc(i,m)                                              1966
   28 continue                                                           1967
      sc(i,mkp3)=s                                                       1968
      pp=1.0/(1.0+exp(-sc(i,mkp3)))                                      1969
      ww=amax1(pp*(1.0-pp),wm)                                           1970
      sc(i,mkp3)=sc(i,mkp3)+(y(i)-pp)/ww                                 1971
      if(il.eq.2) ww=ww**2                                               1972
      ww=ww*w(i)                                                         1973
      sc(i,mkp2)=ww                                                      1974
      sw=sw+ww                                                           1975
      yb=yb+ww*sc(i,mkp3)                                                1976
      if(iter.gt.1) b=b+abs(pp-sc(i,mkp1))                               1977
      sc(i,mkp1)=pp                                                      1978
   29 continue                                                           1979
      if(iter.gt.niter.or.(iter.gt.1.and.b/n.lt.thr)) go to 37           1980
      yb=yb/sw                                                           1981
      do 36 m=1,mk                                                       1982
      b=0.d0                                                             1983
      do 30 i=1,n                                                        1984
      b=b+sc(i,mkp2)*sc(i,m)                                             1985
   30 continue                                                           1986
      b=b/sw                                                             1987
      mm1=m-1                                                            1988
      l=1                                                                1989
      go to 32                                                           1990
   31 l=l+1                                                              1991
   32 if((l).gt.(mm1)) go to 34                                          1992
      s=0.d0                                                             1993
      do 33 i=1,n                                                        1994
      s=s+sc(i,mkp2)*(sc(i,m)-b)*sc(i,l)                                 1995
   33 continue                                                           1996
      d(l,m)=s                                                           1997
      go to 31                                                           1998
   34 a=0.d0                                                             1999
      s=a                                                                2000
      do 35 i=1,n                                                        2001
      ww=sc(i,mkp2)                                                      2002
      pp=sc(i,m)-b                                                       2003
      s=s+ww*pp**2                                                       2004
      a=a+ww*pp*sc(i,mkp3)                                               2005
   35 continue                                                           2006
      d(m,m)=s                                                           2007
      d(m,mkp1)=a                                                        2008
      d(m,mkp2)=b                                                        2009
   36 continue                                                           2010
      call lsf(nk,mk,mkp1,yb,d,d(1,mkp3),a,s,d(1,mkp4),1)                2011
      go to 27                                                           2012
   37 if(jnt .ne. 1) go to 39                                            2013
      az=a                                                               2014
      k=0                                                                2015
      do 38 m=1,nk                                                       2016
      if(tb(1,m).eq.0.0) go to 38                                        2017
      k=k+1                                                              2018
      tb(1,m)=d(k,mkp3)                                                  2019
   38 continue                                                           2020
      go to 45                                                           2021
   39 bz=a                                                               2022
      ll=1                                                               2023
      l1=ll                                                              2024
      la=0                                                               2025
      lt=la                                                              2026
   40 if(kp(1,ll).lt.0) go to 45                                         2027
      if(kp(1,ll) .ne. 0 .or. kp(3,ll) .gt. 0) go to 41                  2028
      ll=ll+1                                                            2029
      go to 40                                                           2030
   41 if(kp(3,ll) .gt. 0) go to 42                                       2031
      lt=lt+1                                                            2032
      tc(-kp(3,ll))=d(lt,mkp3)                                           2033
      ll=ll+1                                                            2034
      go to 40                                                           2035
   42 kp3=kp(3,ll)                                                       2036
      do 44 m=1,kp3                                                      2037
      nt=lp(3,l1)                                                        2038
      la=la+5*lp(1,l1)*nt                                                2039
      do 43 i=1,nt                                                       2040
      lt=lt+1                                                            2041
      tc(i+la)=d(lt,mkp3)                                                2042
   43 continue                                                           2043
      la=la+nt                                                           2044
      l1=l1+1                                                            2045
   44 continue                                                           2046
      ll=ll+1                                                            2047
      go to 40                                                           2048
   45 return                                                             2049
      end                                                                2050
      subroutine varimp (n,p,x,y,w,nk,il,it,az,tb,cm,vip,sc,d)           2051
      integer p                                                          2052
      real x(n,p),y(n),w(n),tb(5,nk),cm(1),vip(p),sc(n,1)                2053
      double precision d(nk,1),sw,yb,yv,wn                               2054
      sw=0.d0                                                            2055
      yb=sw                                                              2056
      yv=yb                                                              2057
      wn=yv                                                              2058
      do 1 i=1,n                                                         2059
      sw=sw+w(i)                                                         2060
      wn=wn+w(i)**2                                                      2061
      yb=yb+w(i)*y(i)                                                    2062
    1 continue                                                           2063
      yb=yb/sw                                                           2064
      do 2 i=1,n                                                         2065
      yv=yv+w(i)*(y(i)-yb)**2                                            2066
    2 continue                                                           2067
      yv=yv/sw                                                           2068
      wn=sw**2/wn                                                        2069
      ip=nk+4                                                            2070
      call varz(0,nk,tb,sc(1,ip),cst,nd)                                 2071
      if(cst .ne. 1.0) go to 3                                           2072
      g0=0.0                                                             2073
      if(il.gt.0) g0=yv                                                  2074
      go to 4                                                            2075
    3 a0=az                                                              2076
      call vp(n,x,y,w,nk,il,yb,sw,a0,sc(1,ip),cm,g0,sc,d)                2077
    4 cst=1.d0/(1.d0-cst/wn)**2                                          2078
      if(il .ne. 0) go to 5                                              2079
      g0=(g0+yv)*cst                                                     2080
      go to 6                                                            2081
    5 g0=g0*cst                                                          2082
    6 do 12 j=1,p                                                        2083
      call varz(j,nk,tb,sc(1,ip),cst,nd)                                 2084
      if(nd .ne. 0) go to 7                                              2085
      vip(j)=g0                                                          2086
      go to 12                                                           2087
    7 if(cst .ne. 1.0) go to 8                                           2088
      g=0.0                                                              2089
      if(il.gt.0) g=yv                                                   2090
      go to 9                                                            2091
    8 a0=az                                                              2092
      call vp(n,x,y,w,nk,il,yb,sw,a0,sc(1,ip),cm,g,sc,d)                 2093
    9 cst=1.d0/(1.d0-cst/wn)**2                                          2094
      if(il .ne. 0) go to 10                                             2095
      g=(g+yv)*cst                                                       2096
      go to 11                                                           2097
   10 g=g*cst                                                            2098
   11 vip(j)=g                                                           2099
   12 continue                                                           2100
      if(it .le. 0) go to 13                                             2101
      write(it,17)                                                       2102
      call numprt(it,p,vip)                                              2103
   13 a0=0.0                                                             2104
      do 14 j=1,p                                                        2105
      vip(j)=sqrt(amax1(0.0,vip(j)-g0))                                  2106
      a0=amax1(a0,vip(j))                                                2107
   14 continue                                                           2108
      if(a0.le.0.0) return                                               2109
      do 15 j=1,p                                                        2110
      vip(j)=100.0*vip(j)/a0                                             2111
   15 continue                                                           2112
      if(it .le. 0) go to 16                                             2113
      write(it,18)                                                       2114
      call numprt(it,p,vip)                                              2115
   16 return                                                             2116
   17 format(/,' -gcv removing each variable:')                          2117
   18 format(/,' relative variable importance:')                         2118
      end                                                                2119
      subroutine numprt(it,n,a)                                          2120
      real a(1)                                                          2121
      i2=0                                                               2122
    1 if(i2.ge.n) go to 2                                                2123
      i1=i2+1                                                            2124
      i2=i2+6                                                            2125
      if(i2.gt.n) i2=n                                                   2126
      write(it,'(/,'' '',6(''    '',i4,''    ''))') (i,i=i1,i2)          2127
      write(it,'('' '',6g12.4)') (a(i),i=i1,i2)                          2128
      go to 1                                                            2129
    2 return                                                             2130
      end                                                                2131
      subroutine varz(j,nk,tb,ub,cst,nd)                                 2132
      real tb(5,nk),ub(5,nk)                                             2133
      do 2 m=1,nk                                                        2134
      do 1 k=1,5                                                         2135
      ub(k,m)=tb(k,m)                                                    2136
    1 continue                                                           2137
    2 continue                                                           2138
      nd=0                                                               2139
      if(j .le. 0) go to 4                                               2140
      do 3 m=1,nk                                                        2141
      if(ub(1,m).eq.0.0) go to 3                                         2142
      if(jf(m,j,ub) .eq. 0) go to 3                                      2143
      ub(1,m)=0.0                                                        2144
      nd=nd+1                                                            2145
    3 continue                                                           2146
    4 cst=1.0                                                            2147
      do 5 m=1,nk                                                        2148
      if(ub(1,m).ne.0.0) cst=cst+ub(5,m)                                 2149
    5 continue                                                           2150
      return                                                             2151
      end                                                                2152
      subroutine vp (n,x,y,w,nk,il,yb,sw,az,tb,cm,gof,sc,d)              2153
      real x(n,1),y(n),w(n),tb(5,nk),cm(1),sc(n,nk)                      2154
      double precision d(nk,1),s,t,yb,sw                                 2155
      if(il .ne. 0) go to 1                                              2156
      call lstsqr(n,x,y,w,nk,yb,sw,tb,cm,gof,sc,d)                       2157
      return                                                             2158
    1 call logitl(n,x,y,w,nk,il,az,tb,cm,sc,d)                           2159
      t=0.d0                                                             2160
      do 3 i=1,n                                                         2161
      s=az                                                               2162
      k=0                                                                2163
      do 2 m=1,nk                                                        2164
      if(tb(1,m).eq.0.0) go to 2                                         2165
      k=k+1                                                              2166
      s=s+tb(1,m)*sc(i,k)                                                2167
    2 continue                                                           2168
      a=s                                                                2169
      pp=1.0/(1.0+exp(-a))                                               2170
      t=t+w(i)*(y(i)-pp)**2                                              2171
    3 continue                                                           2172
      gof=t/sw                                                           2173
      return                                                             2174
      end                                                                2175
      subroutine lstsqr (n,x,y,w,nk,yb,sw,tb,cm,gof,sc,d)                2176
      real x(n,1),y(n),w(n),tb(5,nk),cm(1),sc(n,nk)                      2177
      double precision d(nk,1),a,b,s,yb,sw                               2178
      do 2 i=1,n                                                         2179
      k=0                                                                2180
      do 1 m=1,nk                                                        2181
      if(tb(1,m).eq.0.0) go to 1                                         2182
      k=k+1                                                              2183
      sc(i,k)=phi(m,i,n,x,tb,cm)                                         2184
    1 continue                                                           2185
    2 continue                                                           2186
      mk=k                                                               2187
      mkp1=mk+1                                                          2188
      mkp2=mk+2                                                          2189
      mkp3=mk+3                                                          2190
      mkp4=mk+4                                                          2191
      do 9 m=1,mk                                                        2192
      b=0.d0                                                             2193
      do 3 i=1,n                                                         2194
      b=b+w(i)*sc(i,m)                                                   2195
    3 continue                                                           2196
      b=b/sw                                                             2197
      mm1=m-1                                                            2198
      l=1                                                                2199
      go to 5                                                            2200
    4 l=l+1                                                              2201
    5 if((l).gt.(mm1)) go to 7                                           2202
      s=0.d0                                                             2203
      do 6 i=1,n                                                         2204
      s=s+w(i)*(sc(i,m)-b)*sc(i,l)                                       2205
    6 continue                                                           2206
      d(l,m)=s                                                           2207
      go to 4                                                            2208
    7 a=0.d0                                                             2209
      s=a                                                                2210
      do 8 i=1,n                                                         2211
      ww=w(i)                                                            2212
      pp=sc(i,m)-b                                                       2213
      s=s+ww*pp**2                                                       2214
      a=a+ww*pp*y(i)                                                     2215
    8 continue                                                           2216
      d(m,m)=s                                                           2217
      d(m,mkp1)=a                                                        2218
      d(m,mkp2)=b                                                        2219
    9 continue                                                           2220
      call lsf(nk,mk,mkp1,yb,d,d(1,mkp3),a,s,d(1,mkp4),1)                2221
      gof=s/sw                                                           2222
      return                                                             2223
      end                                                                2224
      function efp(l,jv,nk,tb)                                           2225
      integer jv(l)                                                      2226
      real tb(5,nk)                                                      2227
      efp=0.0                                                            2228
      do 3 m=1,nk                                                        2229
      if(tb(1,m).eq.0.0) go to 3                                         2230
      if(nord(m,tb).ne.l) go to 3                                        2231
      k=0                                                                2232
      do 1 j=1,l                                                         2233
      if(jf(m,jv(j),tb).eq.1) go to 1                                    2234
      k=1                                                                2235
      go to 2                                                            2236
    1 continue                                                           2237
    2 if(k.eq.1) go to 3                                                 2238
      efp=efp+tb(5,m)                                                    2239
    3 continue                                                           2240
      return                                                             2241
      end                                                                2242
      function elg(jv,l,lx,tb,cm)                                        2243
      real tb(5,1),cm(1)                                                 2244
      integer lx(1)                                                      2245
      logical elg                                                        2246
      data ic /0/                                                        2247
      elg=.false.                                                        2248
      kx=iabs(lx(jv))                                                    2249
      if(kx.eq.0) return                                                 2250
      if(l .ne. 0) go to 1                                               2251
      elg=.true.                                                         2252
      return                                                             2253
    1 if((kx .ne. 2) .and. (kx .ne. 3)) go to 2                          2254
      if(nnord(l,tb).gt.0) return                                        2255
    2 ip=l                                                               2256
    3 if(ip.le.0) go to 4                                                2257
      jl=abs(tb(2,ip))+.1                                                2258
      ip=tb(4,ip)+.1                                                     2259
      go to 3                                                            2260
    4 k=iabs(lx(jl))                                                     2261
      call isnstr(jl,jb)                                                 2262
      if((k.eq.2.or.k.eq.3).and.jb.eq.0) return                          2263
      if(ic .ne. 1) go to 5                                              2264
      if(lx(jv).lt.0.and.nordc(1,l,tb,cm).gt.0) return                   2265
      if(lx(jv).gt.0.and.nordc(2,l,tb,cm).gt.0) return                   2266
      go to 6                                                            2267
    5 if(ic .ne. 2) go to 6                                              2268
      if(lx(jv).gt.0.and.nordc(1,l,tb,cm).ge.2) return                   2269
    6 ip=l                                                               2270
    7 if(ip.le.0) go to 8                                                2271
      jl=abs(tb(2,ip))+.1                                                2272
      call intalw(jv,jl,k)                                               2273
      if(k.eq.0) return                                                  2274
      ip=tb(4,ip)+.1                                                     2275
      go to 7                                                            2276
    8 elg=.true.                                                         2277
      return                                                             2278
      entry stelg(i1)                                                    2279
      ic=i1                                                              2280
      return                                                             2281
      end                                                                2282
      function phi(m,i,n,x,tb,cm)                                        2283
      real tb(5,1),x(n,1),cm(1)                                          2284
      phi=1.0                                                            2285
      ip=m                                                               2286
    1 if(ip.le.0) go to 7                                                2287
      t=tb(2,ip)                                                         2288
      j=abs(t)+.1                                                        2289
      if(cm(2*j) .le. 0.0) go to 4                                       2290
      u=cm(int(x(i,j)+.1)+int(tb(3,ip)+.1))                              2291
      if(t .ge. 0.0) go to 5                                             2292
      if(u .ne. 0.0) go to 2                                             2293
      u=1.0                                                              2294
      go to 5                                                            2295
    2 u=0.0                                                              2296
      go to 5                                                            2297
    4 u=amax1(0.0,sign(1.0,t)*(x(i,j)-tb(3,ip)))                         2298
    5 if(u .gt. 0.0) go to 6                                             2299
      phi=0.0                                                            2300
      return                                                             2301
    6 phi=phi*u                                                          2302
      ip=tb(4,ip)+.1                                                     2303
      go to 1                                                            2304
    7 return                                                             2305
      end                                                                2306
      function nord(m,tb)                                                2307
      real tb(5,1)                                                       2308
      ip=m                                                               2309
      nord=0                                                             2310
    1 if(ip.le.0) go to 2                                                2311
      nord=nord+1                                                        2312
      ip=tb(4,ip)+.1                                                     2313
      go to 1                                                            2314
    2 return                                                             2315
      end                                                                2316
      function jf(m,j,tb)                                                2317
      real tb(5,1)                                                       2318
      ip=m                                                               2319
      jf=0                                                               2320
    1 if(ip.le.0) go to 2                                                2321
      jp=abs(tb(2,ip))+.1                                                2322
      if(jp.eq.j) jf=1                                                   2323
      ip=tb(4,ip)+.1                                                     2324
      go to 1                                                            2325
    2 return                                                             2326
      end                                                                2327
      subroutine lsf(nk,m,mkp1,yb,d,a,a0,gf,dp,k1)                       2328
      double precision a(m),d(nk,1),dp(nk,1),eps,yb,a0,gf,s,t,sl,dps     2329
      data big,eps /9.9e30,1.d-05/                                       2330
      mkp2=mkp1+1                                                        2331
      gf=big                                                             2332
      if(d(m,m).le.0.d0) return                                          2333
      sl=1.d0+eps                                                        2334
      do 2 k=k1,m                                                        2335
      do 1 i=1,k                                                         2336
      dp(i,k)=d(i,k)                                                     2337
    1 continue                                                           2338
      dp(k,k)=dp(k,k)*sl                                                 2339
    2 continue                                                           2340
      do 3 k=1,m                                                         2341
      a(k)=d(k,mkp1)                                                     2342
    3 continue                                                           2343
      info=k1                                                            2344
      call spofa(dp,nk,m,info)                                           2345
      if(info.ne.0) return                                               2346
      call sposl(dp,nk,m,a)                                              2347
      s=yb                                                               2348
      t=0.d0                                                             2349
      do 4 i=1,m                                                         2350
      s=s-a(i)*d(i,mkp2)                                                 2351
      t=t-a(i)*(d(i,mkp1)+eps*d(i,i)*a(i))                               2352
    4 continue                                                           2353
      a0=s                                                               2354
      gf=t                                                               2355
      return                                                             2356
      entry seteps(dps)                                                  2357
      eps=dps                                                            2358
      return                                                             2359
      end                                                                2360
      subroutine coll(nk,tb,lp,lv,jv)                                    2361
      integer lp(3,1),lv(1),jv(1)                                        2362
      real tb(5,nk)                                                      2363
      mo=0                                                               2364
      do 1 m=1,nk                                                        2365
      if(tb(1,m).ne.0.0) mo=max0(mo,nord(m,tb))                          2366
    1 continue                                                           2367
      if(mo .ne. 0) go to 2                                              2368
      lp(1,1)=0                                                          2369
      return                                                             2370
    2 l1=1                                                               2371
      l2=l1                                                              2372
      do 11 mt=1,mo                                                      2373
      l10=l1                                                             2374
      do 10 m=1,nk                                                       2375
      if(tb(1,m).eq.0.0.or.nord(m,tb).ne.mt) go to 10                    2376
      call jfv(m,tb,jv)                                                  2377
      jg=0                                                               2378
      l1m1=l1-1                                                          2379
      i=l10                                                              2380
      go to 4                                                            2381
    3 i=i+1                                                              2382
    4 if((i).gt.(l1m1)) go to 8                                          2383
      k=lp(2,i)-1                                                        2384
      ig=0                                                               2385
      do 5 j=1,mt                                                        2386
      if(jv(j).eq.lv(k+j)) go to 5                                       2387
      ig=1                                                               2388
      go to 6                                                            2389
    5 continue                                                           2390
    6 if(ig .ne. 0) go to 3                                              2391
      jg=1                                                               2392
      lp(3,i)=lp(3,i)+1                                                  2393
    8 if(jg .ne. 0) go to 10                                             2394
      lp(1,l1)=mt                                                        2395
      lp(2,l1)=l2                                                        2396
      lp(3,l1)=1                                                         2397
      k=l2-1                                                             2398
      do 9 i=1,mt                                                        2399
      lv(i+k)=jv(i)                                                      2400
    9 continue                                                           2401
      l1=l1+1                                                            2402
      l2=l2+mt                                                           2403
   10 continue                                                           2404
   11 continue                                                           2405
      lp(1,l1)=0                                                         2406
      return                                                             2407
      end                                                                2408
      subroutine jfv(m,tb,jv)                                            2409
      integer jv(1)                                                      2410
      real tb(5,1)                                                       2411
      ip=m                                                               2412
      j=0                                                                2413
    1 if(ip.le.0) go to 2                                                2414
      j=j+1                                                              2415
      jv(j)=abs(tb(2,ip))+.1                                             2416
      ip=tb(4,ip)+.1                                                     2417
      go to 1                                                            2418
    2 if(j.eq.1) return                                                  2419
      j=j-1                                                              2420
    3 l=0                                                                2421
      do 4 i=1,j                                                         2422
      if(jv(i) .le. jv(i+1)) go to 4                                     2423
      k=jv(i)                                                            2424
      jv(i)=jv(i+1)                                                      2425
      jv(i+1)=k                                                          2426
      l=1                                                                2427
    4 continue                                                           2428
      if(l.eq.0) go to 5                                                 2429
      go to 3                                                            2430
    5 return                                                             2431
      end                                                                2432
      subroutine side(l,nt,jv,xe,x)                                      2433
      integer jv(l)                                                      2434
      real xe(2,1),x(nt,1)                                               2435
      l2=l+l                                                             2436
      l3=l2+l                                                            2437
      l4=l3+l                                                            2438
      do 7 k=1,l                                                         2439
      xl=xe(1,jv(k))                                                     2440
      xr=xe(2,jv(k))                                                     2441
      do 6 j=1,nt                                                        2442
      z=x(j,k)                                                           2443
      if(z .gt. xl) go to 1                                              2444
      x(j,k+l)=xl                                                        2445
      x(j,k+l2)=x(j,k+l)                                                 2446
      x(j,k+l3)=0.0                                                      2447
      x(j,k+l4)=x(j,k+l3)                                                2448
      go to 6                                                            2449
    1 dl=z-xl                                                            2450
      dr=xr-z                                                            2451
      x1=xl                                                              2452
      x2=xr                                                              2453
      do 3 m=1,nt                                                        2454
      a=x(m,k)                                                           2455
      if(a.eq.z) go to 3                                                 2456
      dx=a-z                                                             2457
      if(dx .ge. 0.0 .or. -dx .ge. dl) go to 2                           2458
      dl=-dx                                                             2459
      x1=a                                                               2460
    2 if(dx .le. 0.0 .or. dx .ge. dr) go to 3                            2461
      dr=dx                                                              2462
      x2=a                                                               2463
    3 continue                                                           2464
      x1=0.5*(x1+z)                                                      2465
      x2=0.5*(x2+z)                                                      2466
      if(x(j,k+l) .le. 0.0) go to 4                                      2467
      x(j,k+l)=x1                                                        2468
      x(j,k+l2)=x2                                                       2469
      go to 5                                                            2470
    4 x(j,k+l)=x2                                                        2471
      x(j,k+l2)=x1                                                       2472
    5 call pr(x(j,k+l),x(j,k),x(j,k+l2),x(j,k+l3),x(j,k+l4))             2473
    6 continue                                                           2474
    7 continue                                                           2475
      return                                                             2476
      end                                                                2477
      subroutine que(jp,l,nt,jv,n,x,tc,t)                                2478
      integer jv(l)                                                      2479
      real x(n,1),tc(nt,1),t(n)                                          2480
      l2=l+l                                                             2481
      l3=l2+l                                                            2482
      l4=l3+l                                                            2483
      do 3 i=1,n                                                         2484
      if(t(i).eq.0.0) go to 3                                            2485
      q=1.0                                                              2486
      do 1 k=1,l                                                         2487
      j=jv(k)                                                            2488
      q=q*cue(x(i,j),tc(jp,k+l),tc(jp,k),tc(jp,k+l2),tc(jp,k+l3),tc(jp,k 2489
     1+l4))                                                              2490
      if(q.eq.0.0) go to 2                                               2491
    1 continue                                                           2492
    2 t(i)=q                                                             2493
    3 continue                                                           2494
      return                                                             2495
      end                                                                2496
      subroutine scpc(xm,xs,jp,l,nt,jv,tc,b)                             2497
      integer jv(l)                                                      2498
      real xm(1),xs(1),tc(nt,1)                                          2499
      double precision g,h,q                                             2500
      l2=l+l                                                             2501
      l3=l2+l                                                            2502
      l4=l3+l                                                            2503
      q=1.d0                                                             2504
      do 1 k=1,l                                                         2505
      j=jv(k)                                                            2506
      g=xm(j)                                                            2507
      h=xs(j)                                                            2508
      q=q*h                                                              2509
      tc(jp,k+l)=g+h*tc(jp,k+l)                                          2510
      tc(jp,k)=g+h*tc(jp,k)                                              2511
      tc(jp,k+l2)=g+h*tc(jp,k+l2)                                        2512
      tc(jp,k+l3)=tc(jp,k+l3)/h                                          2513
      tc(jp,k+l4)=tc(jp,k+l4)/h**2                                       2514
    1 continue                                                           2515
      b=b/q                                                              2516
      return                                                             2517
      end                                                                2518
      subroutine update(il,n,m,kr,x,y,w,sw,yb,tb,cm,sc,bl,d,dy,db)       2519
      real x(n,1),y(n),w(n),tb(5,1),cm(1),sc(n,1),bl(n)                  2520
      double precision d(n,1),dy(1),db(1),b,s,yb,sw,dv,eps,v,q           2521
      data eps /1.d-4/                                                   2522
      kp=kr+1                                                            2523
      b=0.d0                                                             2524
      t=tb(2,m)                                                          2525
      j=abs(t)+.1                                                        2526
      if(il .ne. 1) go to 3                                              2527
      tk=tb(3,m)                                                         2528
      do 2 i=1,n                                                         2529
      h=bl(i)                                                            2530
      if(h .gt. 0.0) go to 1                                             2531
      sc(i,m)=0.0                                                        2532
      go to 2                                                            2533
    1 sc(i,m)=h*(x(i,j)-tk)                                              2534
      b=b+w(i)*sc(i,m)                                                   2535
    2 continue                                                           2536
      go to 17                                                           2537
    3 if(cm(2*j) .le. 0.0) go to 12                                      2538
      k=tb(3,m)+.1                                                       2539
      nw=0                                                               2540
      n0=nw                                                              2541
      do 11 i=1,n                                                        2542
      h=bl(i)                                                            2543
      if(h .gt. 0.0) go to 4                                             2544
      sc(i,m)=0.0                                                        2545
      go to 11                                                           2546
    4 u=cm(int(x(i,j)+.1)+k)                                             2547
      if(w(i) .le. 0.0) go to 5                                          2548
      nw=nw+1                                                            2549
      if(u.eq.0.0) n0=n0+1                                               2550
    5 if(t .ge. 0.0) go to 8                                             2551
      if(u .ne. 0.0) go to 6                                             2552
      sc(i,m)=h                                                          2553
      go to 10                                                           2554
    6 sc(i,m)=0.0                                                        2555
      go to 10                                                           2556
    8 if(u .gt. 0.0) go to 9                                             2557
      sc(i,m)=0.0                                                        2558
      go to 10                                                           2559
    9 sc(i,m)=h                                                          2560
   10 b=b+w(i)*sc(i,m)                                                   2561
   11 continue                                                           2562
      if(n0.eq.0.or.n0.eq.nw) return                                     2563
      go to 17                                                           2564
   12 tk=tb(3,m)                                                         2565
      sg=sign(1.0,t)                                                     2566
      do 16 i=1,n                                                        2567
      h=bl(i)                                                            2568
      if(h .gt. 0.0) go to 13                                            2569
      sc(i,m)=0.0                                                        2570
      go to 16                                                           2571
   13 u=amax1(0.0,sg*(x(i,j)-tk))                                        2572
      if(u .gt. 0.0) go to 14                                            2573
      sc(i,m)=0.0                                                        2574
      go to 15                                                           2575
   14 sc(i,m)=h*u                                                        2576
   15 b=b+w(i)*sc(i,m)                                                   2577
   16 continue                                                           2578
   17 b=b/sw                                                             2579
      s=0.d0                                                             2580
      v=s                                                                2581
      do 18 j=1,kr                                                       2582
      db(j)=0.d0                                                         2583
   18 continue                                                           2584
      do 21 i=1,n                                                        2585
      d(i,kp)=sc(i,m)-b                                                  2586
      if(sc(i,m).le.0.0) go to 21                                        2587
      q=w(i)*sc(i,m)                                                     2588
      s=s+q*(sc(i,m)-b)                                                  2589
      v=v+q*(y(i)-yb)                                                    2590
      j=1                                                                2591
      go to 20                                                           2592
   19 j=j+1                                                              2593
   20 if((j).gt.(kr)) go to 21                                           2594
      db(j)=db(j)+q*d(i,j)                                               2595
      go to 19                                                           2596
   21 continue                                                           2597
      if(s.le.0.d0) return                                               2598
      dv=s                                                               2599
      j=1                                                                2600
      go to 23                                                           2601
   22 j=j+1                                                              2602
   23 if((j).gt.(kr)) go to 24                                           2603
      s=s-db(j)**2                                                       2604
      go to 22                                                           2605
   24 if(s.lt.eps*dv) return                                             2606
      j=1                                                                2607
      go to 26                                                           2608
   25 j=j+1                                                              2609
   26 if((j).gt.(kr)) go to 28                                           2610
      do 27 i=1,n                                                        2611
      d(i,kp)=d(i,kp)-db(j)*d(i,j)                                       2612
   27 continue                                                           2613
      go to 25                                                           2614
   28 s=1.d0/dsqrt(s)                                                    2615
      j=1                                                                2616
      go to 30                                                           2617
   29 j=j+1                                                              2618
   30 if((j).gt.(kr)) go to 31                                           2619
      v=v-db(j)*dy(j)                                                    2620
      go to 29                                                           2621
   31 dy(kp)=v*s                                                         2622
      do 32 i=1,n                                                        2623
      d(i,kp)=d(i,kp)*s                                                  2624
   32 continue                                                           2625
      kr=kp                                                              2626
      return                                                             2627
      end                                                                2628
      subroutine pr(um,u,up,p,r)                                         2629
      s=1.0                                                              2630
      if(um.gt.up) s=-1.0                                                2631
      p=s*(2.0*up+um-3.0*u)/(up-um)**2                                   2632
      r=s*(2.0*u-up-um)/(up-um)**3                                       2633
      return                                                             2634
      end                                                                2635
      function cue(x,um,u,up,p,r)                                        2636
      s=1.0                                                              2637
      if(um.gt.up) s=-1.0                                                2638
      y=s*x                                                              2639
      if(y .gt. s*um) go to 1                                            2640
      cue=0.0                                                            2641
      return                                                             2642
    1 if(y .lt. s*up) go to 2                                            2643
      cue=y-s*u                                                          2644
      return                                                             2645
    2 cue=p*(x-um)**2+r*(x-um)**3                                        2646
      return                                                             2647
      end                                                                2648
      function varf(nk,d,a,sw,k1,k2)                                     2649
      double precision d(nk,1),a(nk),sw,s,t,u                            2650
      s=0.d0                                                             2651
      do 4 i=k1,k2                                                       2652
      t=0.d0                                                             2653
      do 3 j=k1,k2                                                       2654
      if(j .gt. i) go to 1                                               2655
      u=d(j,i)                                                           2656
      go to 2                                                            2657
    1 u=d(i,j)                                                           2658
    2 t=t+a(j)*u                                                         2659
    3 continue                                                           2660
      s=s+a(i)*t                                                         2661
    4 continue                                                           2662
      varf=s/sw                                                          2663
      return                                                             2664
      end                                                                2665
      subroutine exch(nk,m,k,d,a,b)                                      2666
      real a(m),b(m)                                                     2667
      double precision d(nk,m),t                                         2668
      l=k+1                                                              2669
      km1=k-1                                                            2670
      kp2=k+2                                                            2671
      r=a(k)                                                             2672
      a(k)=a(l)                                                          2673
      a(l)=r                                                             2674
      r=b(k)                                                             2675
      b(k)=b(l)                                                          2676
      b(l)=r                                                             2677
      do 1 j=1,2                                                         2678
      i=nk+j                                                             2679
      t=d(k,i)                                                           2680
      d(k,i)=d(l,i)                                                      2681
      d(l,i)=t                                                           2682
    1 continue                                                           2683
      t=d(k,k)                                                           2684
      d(k,k)=d(l,l)                                                      2685
      d(l,l)=t                                                           2686
      j=1                                                                2687
      go to 3                                                            2688
    2 j=j+1                                                              2689
    3 if((j).gt.(km1)) go to 4                                           2690
      t=d(j,k)                                                           2691
      d(j,k)=d(j,l)                                                      2692
      d(j,l)=t                                                           2693
      go to 2                                                            2694
    4 j=kp2                                                              2695
      go to 6                                                            2696
    5 j=j+1                                                              2697
    6 if((j).gt.(m)) go to 7                                             2698
      t=d(k,j)                                                           2699
      d(k,j)=d(l,j)                                                      2700
      d(l,j)=t                                                           2701
      go to 5                                                            2702
    7 return                                                             2703
      end                                                                2704
      function jft(m,j,tb)                                               2705
      real tb(5,1)                                                       2706
      k=1                                                                2707
      go to 2                                                            2708
    1 k=k+1                                                              2709
    2 if((k).gt.(m)) go to 4                                             2710
      if(int(abs(tb(2,k))+.1) .ne. j) go to 1                            2711
      jft=1                                                              2712
      return                                                             2713
    4 jft=0                                                              2714
      return                                                             2715
      end                                                                2716
      subroutine coefpr (it,nk,az,tb,cm,xs)                              2717
      real tb(5,1),cm(1),xs(1),a(6)                                      2718
      i2=0                                                               2719
    1 if(i2.ge.nk) go to 4                                               2720
      if(i2 .ne. 0) go to 2                                              2721
      i1=0                                                               2722
      i2=min0(5,nk)                                                      2723
      l2=i2+1                                                            2724
      a(1)=az                                                            2725
      call org(1,i2,tb,cm,xs,a(2))                                       2726
      go to 3                                                            2727
    2 i1=i2+1                                                            2728
      i2=i2+6                                                            2729
      if(i2.gt.nk) i2=nk                                                 2730
      l2=i2-i1+1                                                         2731
      call org(i1,i2,tb,cm,xs,a)                                         2732
    3 write(it,'(/,'' bsfn:'',6(''    '',i4,''    ''))') (i,i=i1,i2)     2733
      write(it,'('' coef:'',6g12.4)') (a(i),i=1,l2)                      2734
      go to 1                                                            2735
    4 return                                                             2736
      end                                                                2737
      subroutine org(m1,m2,tb,cm,xs,a)                                   2738
      real xs(1),tb(5,1),cm(1),a(1)                                      2739
      k=0                                                                2740
      do 4 m=m1,m2                                                       2741
      k=k+1                                                              2742
      if(tb(1,m) .ne. 0.0) go to 1                                       2743
      a(k)=0.0                                                           2744
      go to 4                                                            2745
    1 s=1.0                                                              2746
      ip=m                                                               2747
    2 if(ip.le.0) go to 3                                                2748
      j=abs(tb(2,ip))+.1                                                 2749
      if(cm(2*j).eq.0.0) s=s*xs(j)                                       2750
      ip=tb(4,ip)+.1                                                     2751
      go to 2                                                            2752
    3 a(k)=tb(1,m)/s                                                     2753
    4 continue                                                           2754
      return                                                             2755
      end                                                                2756
      subroutine hulset (n,x,big,nh,xh,y)                                2757
      real x(n,1),y(n),xh(3,nh)                                          2758
      do 5 j=1,n                                                         2759
      k=0                                                                2760
      x1=x(j,1)                                                          2761
      x2=x(j,2)                                                          2762
      do 3 i=1,nh                                                        2763
      a=xh(1,i)                                                          2764
      b=xh(2,i)                                                          2765
      sg=xh(3,i)                                                         2766
      if(a .lt. big) go to 1                                             2767
      s=x1-b                                                             2768
      go to 2                                                            2769
    1 s=x2-a*x1-b                                                        2770
    2 if(s*sg .ge. 0.0) go to 3                                          2771
      k=1                                                                2772
      go to 4                                                            2773
    3 continue                                                           2774
    4 if(k.eq.1) y(j)=big                                                2775
    5 continue                                                           2776
      return                                                             2777
      end                                                                2778
      subroutine cvxhul (n,x1,x2,big,nh,xh)                              2779
      real x1(n),x2(n),xh(3,1)                                           2780
      data eps /1.e-3/                                                   2781
      x0=big                                                             2782
      y0=x0                                                              2783
      xm=-big                                                            2784
      ym=xm                                                              2785
      xb=0.0                                                             2786
      yb=xb                                                              2787
      do 1 i=1,n                                                         2788
      xb=xb+x1(i)                                                        2789
      yb=yb+x2(i)                                                        2790
      x0=amin1(x0,x1(i))                                                 2791
      y0=amin1(y0,x2(i))                                                 2792
      xm=amax1(xm,x1(i))                                                 2793
      ym=amax1(ym,x2(i))                                                 2794
    1 continue                                                           2795
      x0=x0-eps*(xm-x0)                                                  2796
      y0=y0-eps*(ym-y0)                                                  2797
      xb=xb/n                                                            2798
      yb=yb/n                                                            2799
      nh=0                                                               2800
      a0=0.0                                                             2801
      lq=1                                                               2802
    2 am=big                                                             2803
      kq=4                                                               2804
      k=0                                                                2805
      do 15 i=1,n                                                        2806
      x=x1(i)                                                            2807
      y=x2(i)                                                            2808
      if(x .ne. x0) go to 5                                              2809
      if(y.eq.y0) go to 15                                               2810
      if(y .le. y0) go to 3                                              2811
      iq=2                                                               2812
      go to 10                                                           2813
    3 iq=4                                                               2814
      go to 10                                                           2815
    5 if(x .le. x0) go to 8                                              2816
      if(y .lt. y0) go to 6                                              2817
      iq=1                                                               2818
      go to 10                                                           2819
    6 iq=4                                                               2820
      go to 10                                                           2821
    8 if(y .le. y0) go to 9                                              2822
      iq=2                                                               2823
      go to 10                                                           2824
    9 iq=3                                                               2825
   10 if(iq.gt.kq) go to 15                                              2826
      if(iq.lt.lq) go to 15                                              2827
      if((iq .ne. 1) .and. (iq .ne. 3)) go to 11                         2828
      a=abs((y-y0)/(x-x0))                                               2829
      go to 12                                                           2830
   11 a=abs((x-x0)/(y-y0))                                               2831
   12 if(iq.eq.lq.and.a.lt.a0) go to 15                                  2832
      if(iq .ge. kq) go to 13                                            2833
      kq=iq                                                              2834
      am=a                                                               2835
      k=i                                                                2836
      go to 15                                                           2837
   13 if(a .ge. am) go to 14                                             2838
      am=a                                                               2839
      k=i                                                                2840
      go to 15                                                           2841
   14 if(a .ne. am) go to 15                                             2842
      if((x-x0)**2+(y-y0)**2.gt.(x1(k)-x0)**2+(x2(k)-y0)**2) k=i         2843
   15 continue                                                           2844
      if(k .ne. 0) go to 16                                              2845
      a0=0.0                                                             2846
      lq=1                                                               2847
      go to 2                                                            2848
   16 if(nh .ne. 0) go to 17                                             2849
      k0=k                                                               2850
      go to 20                                                           2851
   17 if(x1(k) .ne. x0) go to 18                                         2852
      a=big                                                              2853
      b=x0                                                               2854
      s=xb-b                                                             2855
      go to 19                                                           2856
   18 a=(x2(k)-y0)/(x1(k)-x0)                                            2857
      b=y0-a*x0                                                          2858
      s=yb-a*xb-b                                                        2859
   19 xh(1,nh)=a                                                         2860
      xh(2,nh)=b                                                         2861
      xh(3,nh)=sign(1.0,s)                                               2862
      if(k.eq.k0) go to 21                                               2863
   20 nh=nh+1                                                            2864
      x0=x1(k)                                                           2865
      y0=x2(k)                                                           2866
      lq=kq                                                              2867
      a0=am                                                              2868
      go to 2                                                            2869
   21 return                                                             2870
      end                                                                2871
      subroutine knts (l,nt,jv,jl,kv,nk,tb,cm,x,js)                      2872
      integer jv(l),kv(2,jl),js(1)                                       2873
      real tb(5,nk),cm(1),x(nt,l)                                        2874
      l1=0                                                               2875
      do 7 m=1,nk                                                        2876
      if(icf(m,tb,cm,jl,kv,js).eq.0) go to 7                             2877
      if(nordc(1,m,tb,cm).ne.l) go to 7                                  2878
      k=0                                                                2879
      do 1 j=1,l                                                         2880
      if(jf(m,jv(j),tb).eq.1) go to 1                                    2881
      k=1                                                                2882
      go to 2                                                            2883
    1 continue                                                           2884
    2 if(k.eq.1) go to 7                                                 2885
      ip=m                                                               2886
      l1=l1+1                                                            2887
    3 if(ip.le.0) go to 7                                                2888
      t=tb(2,ip)                                                         2889
      j=abs(t)+.1                                                        2890
      if(cm(2*j) .eq. 0.0) go to 4                                       2891
      ip=tb(4,ip)+.1                                                     2892
      go to 3                                                            2893
    4 k=1                                                                2894
    5 if(jv(k).eq.j) go to 6                                             2895
      k=k+1                                                              2896
      go to 5                                                            2897
    6 x(l1,k)=tb(3,ip)                                                   2898
      x(l1,l+k)=sign(1.0,t)                                              2899
      ip=tb(4,ip)+.1                                                     2900
      go to 3                                                            2901
    7 continue                                                           2902
      return                                                             2903
      end                                                                2904
      subroutine sclato (n,p,x,xm,xs,cm,z)                               2905
      integer p                                                          2906
      real x(n,p),xm(p),xs(p),cm(1),z(n,p)                               2907
      do 4 j=1,p                                                         2908
      j1=cm(2*j)+.1                                                      2909
      if(j1 .ne. 0) go to 2                                              2910
      if(xs(j).le.0.0) go to 4                                           2911
      do 1 i=1,n                                                         2912
      z(i,j)=xs(j)*x(i,j)+xm(j)                                          2913
    1 continue                                                           2914
      go to 4                                                            2915
    2 j1=j1-1                                                            2916
      do 3 i=1,n                                                         2917
      l=x(i,j)+.1                                                        2918
      z(i,j)=cm(l+j1)                                                    2919
    3 continue                                                           2920
    4 continue                                                           2921
      call stfmrs(0)                                                     2922
      call stcmrs(0)                                                     2923
      return                                                             2924
      end                                                                2925
      function ncat(kp)                                                  2926
      integer kp(5,1)                                                    2927
      ncat=0                                                             2928
      ll=1                                                               2929
    1 if(kp(1,ll).lt.0) go to 2                                          2930
      if(kp(1,ll).gt.0.and.kp(3,ll).le.0) ncat=ncat+1                    2931
      ll=ll+1                                                            2932
      go to 1                                                            2933
    2 return                                                             2934
      end                                                                2935
      subroutine catv (jl,kp,kv,nv,jv)                                   2936
      integer kp(5,1),kv(2,1),jv(jl,1)                                   2937
      nv=0                                                               2938
      ll=1                                                               2939
    1 if(kp(1,ll).lt.0) go to 20                                         2940
      if(kp(3,ll) .le. 0) go to 2                                        2941
      ll=ll+1                                                            2942
      go to 1                                                            2943
    2 if(kp(1,ll) .eq. jl) go to 3                                       2944
      ll=ll+1                                                            2945
      go to 1                                                            2946
    3 jg=0                                                               2947
      j=1                                                                2948
      go to 5                                                            2949
    4 j=j+1                                                              2950
    5 if((j).gt.(nv)) go to 9                                            2951
      ig=0                                                               2952
      do 6 i=1,jl                                                        2953
      if(jv(i,j).eq.iabs(kv(1,kp(2,ll)+i-1))) go to 6                    2954
      ig=1                                                               2955
      go to 7                                                            2956
    6 continue                                                           2957
    7 if(ig .ne. 0) go to 4                                              2958
      jg=1                                                               2959
    9 if(jg .ne. 1) go to 10                                             2960
      ll=ll+1                                                            2961
      go to 1                                                            2962
   10 l1=ll+1                                                            2963
      ig=0                                                               2964
   11 if(kp(1,l1).lt.0) go to 17                                         2965
      k1=kp(1,l1)                                                        2966
      if((k1 .ne. jl) .and. (kp(3,l1) .le. 0)) go to 12                  2967
      l1=l1+1                                                            2968
      go to 11                                                           2969
   12 do 15 i=1,k1                                                       2970
      k=iabs(kv(1,kp(2,l1)+i-1))                                         2971
      do 13 j=1,jl                                                       2972
      l=iabs(kv(1,kp(2,ll)+j-1))                                         2973
      if(l .ne. k) go to 13                                              2974
      ig=1                                                               2975
      go to 14                                                           2976
   13 continue                                                           2977
   14 if(ig.eq.1) go to 16                                               2978
   15 continue                                                           2979
   16 if(ig.eq.1) go to 17                                               2980
      l1=l1+1                                                            2981
      go to 11                                                           2982
   17 if(ig .ne. 1) go to 18                                             2983
      ll=ll+1                                                            2984
      go to 1                                                            2985
   18 nv=nv+1                                                            2986
      do 19 i=1,jl                                                       2987
      jv(i,nv)=iabs(kv(1,kp(2,ll)+i-1))                                  2988
   19 continue                                                           2989
      ll=ll+1                                                            2990
      go to 1                                                            2991
   20 return                                                             2992
      end                                                                2993
      function cvlv (m,jl,jv,lv,nk,kp,kv,tb,cm,tc)                       2994
      integer jv(jl),lv(jl),kp(5,1),kv(2,1)                              2995
      real tb(5,nk),cm(1),tc(1)                                          2996
      if(m .ne. 1) go to 1                                               2997
      cvlv=cvll(jl,jv,lv,nk,tb,cm)                                       2998
      go to 2                                                            2999
    1 cvlv=cvlq(jl,jv,lv,kp,kv,cm,tc)                                    3000
    2 return                                                             3001
      end                                                                3002
      function cvll (jl,jv,lv,nk,tb,cm)                                  3003
      integer jv(jl),lv(jl),iv(2)                                        3004
      real tb(5,nk),cm(1)                                                3005
      cvll=0.0                                                           3006
      if(jl.gt.2) return                                                 3007
      do 11 m=1,nk                                                       3008
      if(tb(1,m).eq.0.0) go to 11                                        3009
      if(nordc(1,m,tb,cm).gt.0) go to 11                                 3010
      if(nord(m,tb).ne.jl) go to 11                                      3011
      call jfv(m,tb,iv)                                                  3012
      ig=0                                                               3013
      do 1 i=1,jl                                                        3014
      if(jv(i).eq.iv(i)) go to 1                                         3015
      ig=1                                                               3016
      go to 2                                                            3017
    1 continue                                                           3018
    2 if(ig.eq.1) go to 11                                               3019
      phi=1.0                                                            3020
      ip=m                                                               3021
    3 if(ip.le.0) go to 10                                               3022
      t=tb(2,ip)                                                         3023
      j=abs(t)+.1                                                        3024
      i=1                                                                3025
      go to 5                                                            3026
    4 i=i+1                                                              3027
    5 if((i).gt.(jl)) go to 6                                            3028
      if(jv(i).eq.j) go to 6                                             3029
      go to 4                                                            3030
    6 u=cm(lv(i)+int(tb(3,ip)+.1))                                       3031
      if(t .ge. 0.0) go to 8                                             3032
      if(u .ne. 0.0) go to 7                                             3033
      u=1.0                                                              3034
      go to 8                                                            3035
    7 u=0.0                                                              3036
    8 if(u .ne. 0.0) go to 9                                             3037
      phi=0.0                                                            3038
      go to 10                                                           3039
    9 ip=tb(4,ip)+.1                                                     3040
      go to 3                                                            3041
   10 if(phi.gt.0.0) cvll=cvll+tb(1,m)                                   3042
   11 continue                                                           3043
      return                                                             3044
      end                                                                3045
      function cvlq (jl,jv,lv,kp,kv,cm,tc)                               3046
      integer jv(jl),lv(jl),kp(5,1),kv(2,1)                              3047
      real cm(1),tc(1)                                                   3048
      ll=1                                                               3049
      cvlq=0.0                                                           3050
    1 if(kp(1,ll).lt.0) go to 12                                         3051
      if(kp(3,ll) .le. 0) go to 2                                        3052
      ll=ll+1                                                            3053
      go to 1                                                            3054
    2 if(kp(1,ll) .eq. jl) go to 3                                       3055
      ll=ll+1                                                            3056
      go to 1                                                            3057
    3 ig=0                                                               3058
      do 4 i=1,jl                                                        3059
      if(jv(i).eq.iabs(kv(1,kp(2,ll)+i-1))) go to 4                      3060
      ig=1                                                               3061
      go to 5                                                            3062
    4 continue                                                           3063
    5 if(ig .ne. 1) go to 6                                              3064
      ll=ll+1                                                            3065
      go to 1                                                            3066
    6 kt=1                                                               3067
      do 9 j=1,jl                                                        3068
      k=kp(2,ll)+j-1                                                     3069
      jt=cm(lv(j)+kv(2,k))+.1                                            3070
      if(kv(1,k) .ge. 0) go to 8                                         3071
      if(jt .ne. 0) go to 7                                              3072
      jt=1                                                               3073
      go to 8                                                            3074
    7 jt=0                                                               3075
    8 if(jt .ne. 0) go to 9                                              3076
      kt=0                                                               3077
      go to 10                                                           3078
    9 continue                                                           3079
   10 if(kt .ne. 1) go to 11                                             3080
      cvlq=cvlq+tc(-kp(3,ll))                                            3081
   11 ll=ll+1                                                            3082
      go to 1                                                            3083
   12 return                                                             3084
      end                                                                3085
      subroutine ccoll (nk,tb,cm,kp,kv,lp,lv,jv)                         3086
      integer kp(5,1),kv(2,1),lp(3,1),lv(1),jv(1)                        3087
      real tb(5,1),cm(1)                                                 3088
      call collc(nk,tb,cm,kp,kv,jv)                                      3089
      call purcat(nk,tb,cm,kp,kv,li,jv)                                  3090
      ll=li+1                                                            3091
      l1=1                                                               3092
      l2=l1                                                              3093
    1 if(kp(1,ll).lt.0) go to 2                                          3094
      kp(4,ll)=l1                                                        3095
      call collf(nk,tb,cm,kp(1,ll),kv(1,kp(2,ll)),l1,l2,lp,lv,jv)        3096
      kp(3,ll)=l1-kp(4,ll)                                               3097
      ll=ll+1                                                            3098
      go to 1                                                            3099
    2 lp(1,l1)=0                                                         3100
      return                                                             3101
      end                                                                3102
      subroutine collc (nk,tb,cm,kp,kv,jv)                               3103
      integer kp(5,1),kv(2,1),jv(1)                                      3104
      real tb(5,nk),cm(1)                                                3105
      kp(1,1)=0                                                          3106
      kp(2,1)=1                                                          3107
      l1=2                                                               3108
      l2=1                                                               3109
      mc=0                                                               3110
      do 1 m=1,nk                                                        3111
      if(tb(1,m).ne.0.0) mc=max0(mc,nordc(2,m,tb,cm))                    3112
    1 continue                                                           3113
      mt=1                                                               3114
      go to 3                                                            3115
    2 mt=mt+1                                                            3116
    3 if((mt).gt.(mc)) go to 18                                          3117
      l10=l1                                                             3118
      do 17 m=1,nk                                                       3119
      if(tb(1,m).eq.0.0.or.nordc(2,m,tb,cm).ne.mt) go to 17              3120
      call jfvc(2,m,tb,cm,nv,jv,jv(mt+1))                                3121
      jg=0                                                               3122
      l1m1=l1-1                                                          3123
      i=l10                                                              3124
      go to 5                                                            3125
    4 i=i+1                                                              3126
    5 if((i).gt.(l1m1)) go to 15                                         3127
      k=kp(2,i)-1                                                        3128
      ig=0                                                               3129
      do 6 j=1,mt                                                        3130
      if(iabs(jv(j)).eq.iabs(kv(1,k+j))) go to 6                         3131
      ig=1                                                               3132
      go to 7                                                            3133
    6 continue                                                           3134
    7 if(ig .ne. 0) go to 13                                             3135
      do 12 j=1,mt                                                       3136
      m1=kv(2,k+j)                                                       3137
      m2=jv(mt+j)                                                        3138
      jj=iabs(jv(j))                                                     3139
      nc=int(cm(2*jj+1)+.1)-int(cm(2*jj)+.1)+1                           3140
      kk=jv(j)*kv(1,k+j)                                                 3141
      do 10 jk=1,nc                                                      3142
      z=cm(jk+m2)                                                        3143
      if(kk .ge. 0) go to 9                                              3144
      if(z .ne. 0.0) go to 8                                             3145
      z=1.0                                                              3146
      go to 9                                                            3147
    8 z=0.0                                                              3148
    9 if(cm(jk+m1).eq.z) go to 10                                        3149
      ig=1                                                               3150
      go to 11                                                           3151
   10 continue                                                           3152
   11 if(ig.eq.1) go to 13                                               3153
   12 continue                                                           3154
   13 if(ig .ne. 0) go to 4                                              3155
      jg=1                                                               3156
   15 if(jg .ne. 0) go to 17                                             3157
      kp(1,l1)=mt                                                        3158
      kp(2,l1)=l2                                                        3159
      k=l2-1                                                             3160
      do 16 i=1,mt                                                       3161
      kv(1,i+k)=jv(i)                                                    3162
      kv(2,i+k)=jv(i+mt)                                                 3163
   16 continue                                                           3164
      l1=l1+1                                                            3165
      l2=l2+mt                                                           3166
   17 continue                                                           3167
      go to 2                                                            3168
   18 kp(1,l1)=-1                                                        3169
      return                                                             3170
      end                                                                3171
      function nordc (l,m,tb,cm)                                         3172
      real tb(5,1),cm(1)                                                 3173
      ip=m                                                               3174
      nordc=0                                                            3175
    1 if(ip.le.0) go to 4                                                3176
      j=abs(tb(2,ip))+.1                                                 3177
      if(l .ne. 1) go to 2                                               3178
      if(cm(2*j).eq.0.0) nordc=nordc+1                                   3179
      go to 3                                                            3180
    2 if(cm(2*j).gt.0.0) nordc=nordc+1                                   3181
    3 ip=tb(4,ip)+.1                                                     3182
      go to 1                                                            3183
    4 return                                                             3184
      end                                                                3185
      subroutine jfvc (l,m,tb,cm,nv,jv,jp)                               3186
      integer jv(1),jp(1)                                                3187
      real tb(5,1),cm(1)                                                 3188
      ip=m                                                               3189
      nv=0                                                               3190
    1 if(ip.le.0) go to 5                                                3191
      j=abs(tb(2,ip))+.1                                                 3192
      if(l .ne. 1) go to 3                                               3193
      if(cm(2*j) .le. 0.0) go to 4                                       3194
      ip=tb(4,ip)+.1                                                     3195
      go to 1                                                            3196
    3 if(cm(2*j) .ne. 0.0) go to 4                                       3197
      ip=tb(4,ip)+.1                                                     3198
      go to 1                                                            3199
    4 nv=nv+1                                                            3200
      jv(nv)=j                                                           3201
      if(l.ne.1.and.tb(2,ip).lt.0.0) jv(nv)=-j                           3202
      if(l.ne.1) jp(nv)=tb(3,ip)+.1                                      3203
      ip=tb(4,ip)+.1                                                     3204
      go to 1                                                            3205
    5 if(nv.le.1) return                                                 3206
      j=nv-1                                                             3207
    6 ll=0                                                               3208
      do 7 i=1,j                                                         3209
      if(iabs(jv(i)) .le. iabs(jv(i+1))) go to 7                         3210
      ll=1                                                               3211
      k=jv(i)                                                            3212
      jv(i)=jv(i+1)                                                      3213
      jv(i+1)=k                                                          3214
      if(l .eq. 1) go to 7                                               3215
      k=jp(i)                                                            3216
      jp(i)=jp(i+1)                                                      3217
      jp(i+1)=k                                                          3218
    7 continue                                                           3219
      if(ll.eq.0) go to 8                                                3220
      go to 6                                                            3221
    8 return                                                             3222
      end                                                                3223
      subroutine purcat (nk,tb,cm,kp,kv,li,jv)                           3224
      integer kp(5,1),kv(2,1),jv(1)                                      3225
      real tb(5,nk),cm(1)                                                3226
      lm=1                                                               3227
    1 if(kp(1,lm).lt.0) go to 2                                          3228
      lm=lm+1                                                            3229
      go to 1                                                            3230
    2 ll=1                                                               3231
      li=0                                                               3232
    3 if(kp(1,ll).lt.0) go to 20                                         3233
      jl=kp(1,ll)                                                        3234
      if(jl .gt. 0) go to 4                                              3235
      ll=ll+1                                                            3236
      go to 3                                                            3237
    4 ifg=0                                                              3238
      jfg=ifg                                                            3239
      do 6 m=1,nk                                                        3240
      if(icf(m,tb,cm,jl,kv(1,kp(2,ll)),jv).eq.0) go to 6                 3241
      if(nord(m,tb) .ne. jl) go to 5                                     3242
      ifg=1                                                              3243
      go to 6                                                            3244
    5 jfg=1                                                              3245
    6 continue                                                           3246
      if(ifg .ne. 0) go to 9                                             3247
      if(jfg .ne. 0) go to 8                                             3248
      write(6,7)                                                         3249
    7 format (' bug in purcat - term not found.')                        3250
      stop                                                               3251
    8 ll=ll+1                                                            3252
      go to 3                                                            3253
    9 li=li+1                                                            3254
      j=lm                                                               3255
      go to 11                                                           3256
   10 j=j+(-1)                                                           3257
   11 if((-1)*((j)-(li)).gt.0) go to 13                                  3258
      do 12 i=1,5                                                        3259
      kp(i,j+1)=kp(i,j)                                                  3260
   12 continue                                                           3261
      go to 10                                                           3262
   13 lm=lm+1                                                            3263
      ll=ll+1                                                            3264
      do 14 i=1,5                                                        3265
      kp(i,li)=kp(i,ll)                                                  3266
   14 continue                                                           3267
      kp(3,li)=0                                                         3268
      kp(4,li)=1                                                         3269
      kp(5,li)=0                                                         3270
      if(jfg .ne. 1) go to 15                                            3271
      ll=ll+1                                                            3272
      go to 3                                                            3273
   15 j=ll+1                                                             3274
      go to 17                                                           3275
   16 j=j+1                                                              3276
   17 if((j).gt.(lm)) go to 19                                           3277
      do 18 i=1,5                                                        3278
      kp(i,j-1)=kp(i,j)                                                  3279
   18 continue                                                           3280
      go to 16                                                           3281
   19 lm=lm-1                                                            3282
      go to 3                                                            3283
   20 return                                                             3284
      end                                                                3285
      subroutine collf (nk,tb,cm,jl,kv,l1,l2,lp,lv,jv)                   3286
      integer kv(2,1),lp(3,1),lv(1),jv(1)                                3287
      real tb(5,1),cm(1)                                                 3288
      mo=0                                                               3289
      do 1 m=1,nk                                                        3290
      if(icf(m,tb,cm,jl,kv,jv).ne.0) mo=max0(mo,nordc(1,m,tb,cm))        3291
    1 continue                                                           3292
      if(mo.eq.0) return                                                 3293
      do 10 mt=1,mo                                                      3294
      l10=l1                                                             3295
      do 9 m=1,nk                                                        3296
      if(icf(m,tb,cm,jl,kv,jv).eq.0) go to 9                             3297
      if(nordc(1,m,tb,cm).ne.mt) go to 9                                 3298
      call jfvc(1,m,tb,cm,nv,jv,jv)                                      3299
      jg=0                                                               3300
      l1m1=l1-1                                                          3301
      i=l10                                                              3302
      go to 3                                                            3303
    2 i=i+1                                                              3304
    3 if((i).gt.(l1m1)) go to 7                                          3305
      k=lp(2,i)-1                                                        3306
      ig=0                                                               3307
      do 4 j=1,mt                                                        3308
      if(jv(j).eq.lv(k+j)) go to 4                                       3309
      ig=1                                                               3310
      go to 5                                                            3311
    4 continue                                                           3312
    5 if(ig .ne. 0) go to 2                                              3313
      jg=1                                                               3314
      lp(3,i)=lp(3,i)+1                                                  3315
    7 if(jg .ne. 0) go to 9                                              3316
      lp(1,l1)=mt                                                        3317
      lp(2,l1)=l2                                                        3318
      lp(3,l1)=1                                                         3319
      k=l2-1                                                             3320
      do 8 i=1,mt                                                        3321
      lv(i+k)=jv(i)                                                      3322
    8 continue                                                           3323
      l1=l1+1                                                            3324
      l2=l2+mt                                                           3325
    9 continue                                                           3326
   10 continue                                                           3327
      return                                                             3328
      end                                                                3329
      function icf (m,tb,cm,jl,kv,jv)                                    3330
      integer kv(2,jl),jv(1)                                             3331
      real tb(5,1),cm(1)                                                 3332
      icf=0                                                              3333
      if(tb(1,m).eq.0.0.or.nordc(2,m,tb,cm).ne.jl) return                3334
      if(jl .ne. 0) go to 1                                              3335
      icf=1                                                              3336
      return                                                             3337
    1 call jfvc(2,m,tb,cm,nv,jv,jv(jl+1))                                3338
      do 2 j=1,jl                                                        3339
      if(iabs(jv(j)).ne.iabs(kv(1,j))) return                            3340
    2 continue                                                           3341
      do 6 j=1,jl                                                        3342
      l1=kv(2,j)                                                         3343
      l2=jv(jl+j)                                                        3344
      k=2*iabs(jv(j))                                                    3345
      kk=jv(j)*kv(1,j)                                                   3346
      nc=int(cm(k+1)+.1)-int(cm(k)+.1)+1                                 3347
      do 5 i=1,nc                                                        3348
      z=cm(i+l2)                                                         3349
      if(kk .ge. 0) go to 4                                              3350
      if(z .ne. 0.0) go to 3                                             3351
      z=1.0                                                              3352
      go to 4                                                            3353
    3 z=0.0                                                              3354
    4 if(cm(i+l1).ne.z) return                                           3355
    5 continue                                                           3356
    6 continue                                                           3357
      icf=1                                                              3358
      return                                                             3359
      end                                                                3360
      function icat (x,j,cm)                                             3361
      real cm(1)                                                         3362
      j0=cm(2*j)+.1                                                      3363
      j1=j0                                                              3364
      j2=cm(2*j+1)+.1                                                    3365
    1 if(j2.eq.j1+1) go to 5                                             3366
      k=(j1+j2)/2                                                        3367
      if(cm(k) .ne. x) go to 2                                           3368
      icat=k-j0+1                                                        3369
      return                                                             3370
    2 if(cm(k) .ge. x) go to 3                                           3371
      j1=k                                                               3372
      go to 1                                                            3373
    3 j2=k                                                               3374
      go to 1                                                            3375
    5 if(x .ne. cm(j1)) go to 6                                          3376
      icat=j1-j0+1                                                       3377
      go to 8                                                            3378
    6 if(x .ne. cm(j2)) go to 7                                          3379
      icat=j2-j0+1                                                       3380
      go to 8                                                            3381
    7 icat=0                                                             3382
    8 return                                                             3383
      end                                                                3384
      subroutine csp  (jp,nc,m,n,x,y,w,nk,tb,cm,kcp,yb,d,kr,ntt,sw,me,mk 3385
     1p2,nop,sc,db,sp,mm)                                                3386
      integer mm(nc,2)                                                   3387
      real x(n,1),y(n),w(n),tb(5,nk),cm(1),sc(n)                         3388
      double precision yb,sw,d(nk,1),db(n,1),sp(mkp2,1),a,b,s,eps,dv,dy  3389
      data eps,big /1.d-4,9.9e30/                                        3390
      nop=0                                                              3391
      if(nc .gt. 1) go to 1                                              3392
      tb(1,m)=big                                                        3393
      return                                                             3394
    1 mk=mkp2-2                                                          3395
      mkp1=mk+1                                                          3396
      n1=nc+1                                                            3397
      do 3 j=1,n1                                                        3398
      do 2 i=1,mkp2                                                      3399
      sp(i,j)=0.d0                                                       3400
    2 continue                                                           3401
    3 continue                                                           3402
      do 4 j=1,nc                                                        3403
      mm(j,2)=0                                                          3404
    4 continue                                                           3405
      do 7 i=1,n                                                         3406
      h=sc(i)                                                            3407
      if(h.le.0.0.or.w(i).le.0.0) go to 7                                3408
      wh=w(i)*h                                                          3409
      k=x(i,jp)+.1                                                       3410
      mm(k,2)=mm(k,2)+1                                                  3411
      sp(mkp2,k)=sp(mkp2,k)+wh                                           3412
      sp(mkp1,k)=sp(mkp1,k)+wh*(y(i)-yb)                                 3413
      sp(m,k)=sp(m,k)+wh*h                                               3414
      j=1                                                                3415
      go to 6                                                            3416
    5 j=j+1                                                              3417
    6 if((j).gt.(kr)) go to 7                                            3418
      sp(j,k)=sp(j,k)+wh*db(i,j)                                         3419
      go to 5                                                            3420
    7 continue                                                           3421
      do 8 j=1,nc                                                        3422
      mm(j,1)=j                                                          3423
    8 continue                                                           3424
      bof0=big                                                           3425
      ns=0                                                               3426
      jj=nc                                                              3427
      nrt=0                                                              3428
      k1=1                                                               3429
    9 bof1=big                                                           3430
      js=0                                                               3431
      do 18 j=1,jj                                                       3432
      k=mm(j,1)                                                          3433
      if(mm(k,2).eq.0) go to 18                                          3434
      nr=nrt+mm(k,2)                                                     3435
      if(nr.le.me.or.ntt-nr.le.me) go to 18                              3436
      dy=sp(mkp1,n1)+sp(mkp1,k)                                          3437
      a=sp(mkp2,n1)+sp(mkp2,k)                                           3438
      dv=sp(m,n1)+sp(m,k)-a**2/sw                                        3439
      if(dv .le. 0.d0) go to 17                                          3440
      i=1                                                                3441
      go to 11                                                           3442
   10 i=i+1                                                              3443
   11 if((i).gt.(kr)) go to 12                                           3444
      d(i,2)=sp(i,n1)+sp(i,k)                                            3445
      go to 10                                                           3446
   12 a=0.d0                                                             3447
      b=a                                                                3448
      i=1                                                                3449
      go to 14                                                           3450
   13 i=i+1                                                              3451
   14 if((i).gt.(kr)) go to 15                                           3452
      s=d(i,2)                                                           3453
      a=a+s*d(i,1)                                                       3454
      b=b+s**2                                                           3455
      go to 13                                                           3456
   15 b=dv-b                                                             3457
      if(b .le. eps*dv) go to 17                                         3458
      nop=nop+1                                                          3459
      b=-(dy-a)**2/b                                                     3460
      if(b .ge. bof1) go to 16                                           3461
      bof1=b                                                             3462
      js=j                                                               3463
   16 if(b .ge. bof0) go to 17                                           3464
      bof0=b                                                             3465
      ns=jj                                                              3466
   17 if(nc.eq.2) go to 19                                               3467
   18 continue                                                           3468
   19 if(js.eq.0) go to 23                                               3469
      k=mm(js,1)                                                         3470
      mm(js,1)=mm(jj,1)                                                  3471
      mm(jj,1)=k                                                         3472
      sp(mkp1,n1)=sp(mkp1,n1)+sp(mkp1,k)                                 3473
      sp(mkp2,n1)=sp(mkp2,n1)+sp(mkp2,k)                                 3474
      nrt=nrt+mm(k,2)                                                    3475
      sp(m,n1)=sp(m,n1)+sp(m,k)                                          3476
      i=1                                                                3477
      go to 21                                                           3478
   20 i=i+1                                                              3479
   21 if((i).gt.(kr)) go to 22                                           3480
      sp(i,n1)=sp(i,n1)+sp(i,k)                                          3481
      go to 20                                                           3482
   22 jj=jj-1                                                            3483
      if(jj.le.2) go to 23                                               3484
      go to 9                                                            3485
   23 tb(1,m)=bof0                                                       3486
      tb(3,m)=kcp                                                        3487
      do 24 j=1,nc                                                       3488
      cm(j+kcp)=0.0                                                      3489
   24 continue                                                           3490
      if(ns.eq.0) return                                                 3491
      do 25 j=ns,nc                                                      3492
      cm(mm(j,1)+kcp)=1.0                                                3493
   25 continue                                                           3494
      return                                                             3495
      end                                                                3496
      subroutine rspnpr (it,il,n,y,w,m)                                  3497
      integer m(n)                                                       3498
      real y(n),w(n),wm(2)                                               3499
      if(it.le.0) return                                                 3500
      if(il .ne. 1) go to 2                                              3501
      wm(1)=0.0                                                          3502
      wm(2)=wm(1)                                                        3503
      do 1 i=1,n                                                         3504
      k=y(i)+1.1                                                         3505
      wm(k)=wm(k)+w(i)                                                   3506
    1 continue                                                           3507
      wt=wm(1)+wm(2)                                                     3508
      wm(1)=wm(1)/wt                                                     3509
      wm(2)=wm(2)/wt                                                     3510
      write(it,'(/,'' binary (0/1) response:  mass(0) ='',g12.4,         3511
     1                           ''   mass(1) ='',g12.4)') wm(1),wm(2)   3512
      return                                                             3513
    2 write(it,'(/,'' ordinal response:'')')                             3514
      write(it,'(''      min         n/4         n/2        3n/4         3515
     1 max'')')                                                          3516
      do 3 i=1,n                                                         3517
      m(i)=i                                                             3518
    3 continue                                                           3519
      call psort(y,m,1,n)                                                3520
      write(it,'('' '',5g12.4)') y(m(1)),y(m(n/4)),y(m(n/2)),y(m(n-n/4)) 3521
     1,y(m(n))                                                           3522
      return                                                             3523
      end                                                                3524
      subroutine ordpr (it,n,p,x,lx,m)                                   3525
      integer p,lx(p),m(n,p)                                             3526
      real x(n,p)                                                        3527
      if(it.le.0) return                                                 3528
      no=0                                                               3529
      do 1 j=1,p                                                         3530
      if(lx(j).gt.0) no=no+1                                             3531
    1 continue                                                           3532
      if(no.eq.0) return                                                 3533
      write(it,'(/,'' there are'',i3,'' ordinal predictor variables.'',/ 3534
     1)') no                                                             3535
      write(it,'(''  var     min         n/4         n/2        3n/4     3536
     1     max'')')                                                      3537
      n1=n/4                                                             3538
      n2=n/2                                                             3539
      n3=n-n1                                                            3540
      do 2 j=1,p                                                         3541
      if(lx(j).le.0) go to 2                                             3542
      write(it,'('' '',i3,'' '',5g12.4)') j,  x(m(1,j),j),x(m(n1,j),j),x 3543
     1(m(n2,j),j),x(m(n3,j),j),x(m(n,j),j)                               3544
    2 continue                                                           3545
      return                                                             3546
      end                                                                3547
      subroutine catpr(it,n,p,x,cm,mm)                                   3548
      integer p,mm(1)                                                    3549
      real x(n,p),cm(1)                                                  3550
      if(it.le.0) return                                                 3551
      nct=cm(1)+.1                                                       3552
      if(nct.eq.0) return                                                3553
      n2=2*p+1                                                           3554
      np=0                                                               3555
      write(it,'(/,'' there are'',i3,'' categorical predictor variables. 3556
     1'')') nct                                                          3557
      i=2                                                                3558
      go to 2                                                            3559
    1 i=i+(2)                                                            3560
    2 if((2)*((i)-(n2)).gt.0) go to 6                                    3561
      np=np+1                                                            3562
      j1=cm(i)+.1                                                        3563
      if(j1.eq.0) go to 1                                                3564
      j2=cm(i+1)+.1                                                      3565
      nv=j2-j1+1                                                         3566
      do 3 j=1,nv                                                        3567
      mm(j)=0                                                            3568
    3 continue                                                           3569
      do 4 j=1,n                                                         3570
      ic=x(j,np)+.1                                                      3571
      mm(ic)=mm(ic)+1                                                    3572
    4 continue                                                           3573
      write(it,'(/,'' categorical variable'',i3,'' has'',i3,'' values.'' 3574
     1)') np,nv                                                          3575
      write(it,'(''  value     internal code     counts'')')             3576
      k=0                                                                3577
      do 5 j=j1,j2                                                       3578
      k=k+1                                                              3579
      write(it,'(f6.0,i13,i15)') cm(j),k,mm(k)                           3580
    5 continue                                                           3581
      go to 1                                                            3582
    6 return                                                             3583
      end                                                                3584
      subroutine holl (jp,cm,t,h)                                        3585
      real cm(1)                                                         3586
      character*28 h                                                     3587
      j1=cm(2*jp)+.1                                                     3588
      j2=cm(2*jp+1)+.1                                                   3589
      j2=j2-j1+1                                                         3590
      if(j2 .le. 28) go to 1                                             3591
      h='   cat. factor > 28 values  '                                   3592
      return                                                             3593
    1 h='                            '                                   3594
      j1=(28-j2)/2                                                       3595
      j2=j1+j2-1                                                         3596
      k=t+.1                                                             3597
      do 3 j=j1,j2                                                       3598
      if(cm(k+j-j1+1) .le. 0.0) go to 2                                  3599
      h(j:j)='1'                                                         3600
      go to 3                                                            3601
    2 h(j:j)='0'                                                         3602
    3 continue                                                           3603
      return                                                             3604
      end                                                                3605
      subroutine slova (nk,it,tb,ni,lp,lv)                               3606
      integer lp(3,1),lv(1)                                              3607
      real tb(5,nk)                                                      3608
      write(it,4) ni                                                     3609
      call coll(nk,tb,lp,lv,lp(1,nk+1))                                  3610
      m=1                                                                3611
    1 if(lp(1,m).eq.0) go to 2                                           3612
      m=m+1                                                              3613
      go to 1                                                            3614
    2 na=m-1                                                             3615
      do 3 m=1,na                                                        3616
      k2=lp(2,m)                                                         3617
      i2=lp(1,m)+k2-1                                                    3618
      write(it,5) m,lp(3,m),(lv(i),i=k2,i2)                              3619
    3 continue                                                           3620
      return                                                             3621
    4 format(/,' sliced anova decomposition on',i3,' basis functions:',/ 3622
     1,  '   fun      #bsfns      variable(s)')                          3623
    5 format('  ',i3,'         ',i2,'       ',20i4)                      3624
      end                                                                3625
      subroutine reducq (flg,x,nk,tb,cm,tc,kp,kv,lp,lv,r,td,sc,fc)       3626
      integer kp(5,1),kv(2,1),lp(3,1),lv(1)                              3627
      real x(1),tb(5,nk),cm(1),tc(1),r(1),td(2,nk),sc(2,1),fc(1)         3628
      ll=1                                                               3629
      la=ll                                                              3630
      l1=la                                                              3631
      laa=0                                                              3632
      do 1 m=1,nk                                                        3633
      td(1,m)=0.0                                                        3634
    1 continue                                                           3635
    2 if(kp(1,ll).lt.0) go to 9                                          3636
      nv=0                                                               3637
      if(kp(1,ll) .le. 0) go to 4                                        3638
      jl=kp(1,ll)                                                        3639
      do 3 il=1,jl                                                       3640
      k=kp(2,ll)+il-1                                                    3641
      nv=nv+1                                                            3642
      sc(1,nv)=kv(1,k)                                                   3643
      sc(2,nv)=kv(2,k)                                                   3644
    3 continue                                                           3645
      go to 5                                                            3646
    4 if(kp(3,ll) .gt. 0) go to 5                                        3647
      ll=ll+1                                                            3648
      go to 2                                                            3649
    5 if(kp(3,ll) .gt. 0) go to 6                                        3650
      m=match(nv,sc,nk,tb,cm,r,0)                                        3651
      td(1,m)=tc(-kp(3,ll))                                              3652
      ll=ll+1                                                            3653
      go to 2                                                            3654
    6 kp3=kp(3,ll)                                                       3655
      do 8 k=1,kp3                                                       3656
      l=lp(1,l1)                                                         3657
      nt=lp(3,l1)                                                        3658
      laa=laa+5*l*nt                                                     3659
      do 7 jp=1,nt                                                       3660
      call gtrm(1,jp,l,nt,lv(lp(2,l1)),flg,x,nk,tb,tc(la),sc(1,nv+1),fc) 3661
      m=match(nv+l,sc,nk,tb,cm,r,0)                                      3662
      td(1,m)=tc(jp+laa)                                                 3663
      call std(m,flg,x,l,sc(1,nv+1),fc,nk,tb,r,td)                       3664
    7 continue                                                           3665
      laa=laa+nt                                                         3666
      l1=l1+1                                                            3667
      la=la+nt*(5*l+1)                                                   3668
    8 continue                                                           3669
      ll=ll+1                                                            3670
      go to 2                                                            3671
    9 return                                                             3672
      end                                                                3673
      subroutine gtrm (il,jp,l,nt,jv,flg,x,nk,tb,tc,te,fc)               3674
      integer jv(l)                                                      3675
      real x(1),tb(5,nk),tc(nt,1),te(2,1),fc(1)                          3676
      l2=l+l                                                             3677
      nf=0                                                               3678
      l3=l2+l                                                            3679
      l4=l3+l                                                            3680
      do 1 k=1,l                                                         3681
      j=jv(k)                                                            3682
      jj=j                                                               3683
      if(tc(jp,k+l).gt.tc(jp,k+l2)) jj=-jj                               3684
      te(1,k)=jj                                                         3685
      te(2,k)=tc(jp,k)                                                   3686
      if(il.eq.2) go to 1                                                3687
      if(x(j).eq.flg) go to 1                                            3688
      nf=nf+1                                                            3689
      fc(nf)=cue(x(j),tc(jp,k+l),tc(jp,k),tc(jp,k+l2),tc(jp,k+l3),tc(jp, 3690
     1k+l4))                                                             3691
    1 continue                                                           3692
      return                                                             3693
      end                                                                3694
      function match (nv,te,nk,tb,cm,r,iz)                               3695
      real te(2,nv),tb(5,nk),cm(1),r(1)                                  3696
      match=0                                                            3697
      do 15 m=1,nk                                                       3698
      if(tb(1,m).eq.0.0) go to 15                                        3699
      if(nord(m,tb).ne.nv) go to 15                                      3700
      jg=0                                                               3701
      do 13 j=1,nv                                                       3702
      t=te(1,j)                                                          3703
      u=te(2,j)                                                          3704
      jp=abs(t)+.1                                                       3705
      jp2=2*jp                                                           3706
      jp21=jp2+1                                                         3707
      jpp=jp                                                             3708
      if(t.lt.0.0) jpp=-jpp                                              3709
      ig=0                                                               3710
      ip=m                                                               3711
    1 if(ip.le.0) go to 12                                               3712
      t=tb(2,ip)                                                         3713
      jq=abs(t)+.1                                                       3714
      jqq=jq                                                             3715
      if(t.lt.0.0) jqq=-jqq                                              3716
      if(jp .eq. jq) go to 2                                             3717
      ip=tb(4,ip)+.1                                                     3718
      go to 1                                                            3719
    2 if(cm(jp2) .ne. 0.0) go to 4                                       3720
      if(jpp .ne. jqq .or. ieq(tb(3,ip),u,r(jp)) .ne. 1) go to 3         3721
      ig=1                                                               3722
      go to 12                                                           3723
    3 ip=tb(4,ip)+.1                                                     3724
      go to 1                                                            3725
    4 nc=int(cm(jp21)+.1)-int(cm(jp2)+.1)+1                              3726
      i1=u+.1                                                            3727
      i2=tb(3,ip)+.1                                                     3728
      kg=0                                                               3729
      do 9 i=1,nc                                                        3730
      j1=cm(i1+i)                                                        3731
      j2=cm(i2+i)                                                        3732
      if(jpp .ge. 0) go to 6                                             3733
      if(j1 .ne. 0) go to 5                                              3734
      j1=1                                                               3735
      go to 6                                                            3736
    5 j1=0                                                               3737
    6 if(jqq .ge. 0) go to 8                                             3738
      if(j2 .ne. 0) go to 7                                              3739
      j2=1                                                               3740
      go to 8                                                            3741
    7 j2=0                                                               3742
    8 if(j1 .eq. j2) go to 9                                             3743
      kg=1                                                               3744
      go to 10                                                           3745
    9 continue                                                           3746
   10 if(kg .ne. 0) go to 11                                             3747
      ig=1                                                               3748
      go to 12                                                           3749
   11 ip=tb(4,ip)+.1                                                     3750
      go to 1                                                            3751
   12 if(ig .ne. 0) go to 13                                             3752
      jg=1                                                               3753
      go to 14                                                           3754
   13 continue                                                           3755
   14 if(jg .ne. 0) go to 15                                             3756
      match=m                                                            3757
      go to 16                                                           3758
   15 continue                                                           3759
   16 if(match.gt.0.or.iz.ne.0) return                                   3760
      write(6,17)                                                        3761
   17 format (' bug in match - term not found.')                         3762
      do 19 j=1,nv                                                       3763
      write(6,18)j,te(1,j),te(2,j)                                       3764
   18 format (' te(',i2,')=',2g12.4)                                     3765
   19 continue                                                           3766
      do 21 j=1,nk                                                       3767
      write(6,20)j,(tb(i,j),i=1,4)                                       3768
   20 format (' tb(',i2,')=',4g12.4)                                     3769
   21 continue                                                           3770
      stop                                                               3771
      end                                                                3772
      subroutine std (m,flg,x,l,te,fc,nk,tb,r,td)                        3773
      real x(1),te(2,1),fc(1),tb(5,nk),r(1),td(2,nk)                     3774
      ip=m                                                               3775
    1 if(ip.le.0) go to 4                                                3776
      t=tb(2,ip)                                                         3777
      j=abs(t)+.1                                                        3778
      jj=j                                                               3779
      if(t.lt.0.0) jj=-jj                                                3780
      u=tb(3,ip)                                                         3781
      k=0                                                                3782
      ig=0                                                               3783
      do 2 i=1,l                                                         3784
      t=te(1,i)                                                          3785
      jp=abs(t)+.1                                                       3786
      if(x(jp).ne.flg) k=k+1                                             3787
      if(t.lt.0.0) jp=-jp                                                3788
      if(jj .ne. jp .or. ieq(te(2,i),u,r(j)) .ne. 1) go to 2             3789
      ig=1                                                               3790
      go to 3                                                            3791
    2 continue                                                           3792
    3 if(ig.eq.1.and.x(j).ne.flg) td(2,ip)=fc(k)                         3793
      ip=tb(4,ip)+.1                                                     3794
      go to 1                                                            3795
    4 return                                                             3796
      end                                                                3797
      subroutine reducl (flg,x,nk,az,tb,cm,bz,td,r,azn,tbn,bzn,sc)       3798
      real x(1),tb(5,nk),cm(1),td(2,nk),r(1),tbn(5,nk),sc(1)             3799
      azn=az                                                             3800
      do 2 m=1,nk                                                        3801
      do 1 i=1,5                                                         3802
      tbn(i,m)=tb(i,m)                                                   3803
    1 continue                                                           3804
    2 continue                                                           3805
      bzn=bz                                                             3806
      do 9 m=1,nk                                                        3807
      t=tb(2,m)                                                          3808
      j=abs(t)+.1                                                        3809
      if(x(j).eq.flg) go to 9                                            3810
      if(cm(2*j) .le. 0.0) go to 7                                       3811
      k=icat(x(j),j,cm)                                                  3812
      if(k .ne. 0) go to 3                                               3813
      u=0.0                                                              3814
      go to 4                                                            3815
    3 u=cm(k+int(tb(3,m)+.1))                                            3816
    4 if(t .ge. 0.0) go to 6                                             3817
      if(u .ne. 0.0) go to 5                                             3818
      u=1.0                                                              3819
      go to 6                                                            3820
    5 u=0.0                                                              3821
    6 td(2,m)=u                                                          3822
      go to 8                                                            3823
    7 u=amax1(0.0,sign(1.0,t)*(x(j)-tb(3,m)))                            3824
    8 sc(m)=u                                                            3825
    9 continue                                                           3826
      m=nk                                                               3827
      go to 11                                                           3828
   10 m=m+(-1)                                                           3829
   11 if((-1)*((m)-(1)).gt.0) go to 21                                   3830
      ip=tbn(4,m)+.1                                                     3831
      t=tbn(2,m)                                                         3832
      j=abs(t)+.1                                                        3833
      if(x(j) .ne. flg) go to 15                                         3834
      if(tbn(1,m) .eq. 0.0) go to 10                                     3835
      iq=ip                                                              3836
   12 if(iq.le.0) go to 10                                               3837
      t=tbn(2,iq)                                                        3838
      j=abs(t)+.1                                                        3839
      if(x(j) .eq. flg) go to 13                                         3840
      tbn(1,m)=tbn(1,m)*sc(iq)                                           3841
      td(1,m)=td(1,m)*td(2,iq)                                           3842
   13 iq=tbn(4,iq)+.1                                                    3843
      go to 12                                                           3844
   15 k=m+1                                                              3845
      go to 17                                                           3846
   16 k=k+1                                                              3847
   17 if((k).gt.(nk)) go to 18                                           3848
      if(int(tbn(4,k)+.1).eq.m) tbn(4,k)=tbn(4,m)                        3849
      go to 16                                                           3850
   18 if(tbn(1,m).eq.0.0) go to 10                                       3851
      if(ip .ne. 0) go to 19                                             3852
      azn=azn+tbn(1,m)*sc(m)                                             3853
      bzn=bzn+td(1,m)*td(2,m)                                            3854
      go to 10                                                           3855
   19 tbn(1,ip)=tbn(1,ip)+tbn(1,m)*sc(m)                                 3856
      td(1,ip)=td(1,ip)+td(1,m)*td(2,m)                                  3857
      go to 10                                                           3858
   21 no=nk                                                              3859
      m=nk                                                               3860
      go to 23                                                           3861
   22 m=m+(-1)                                                           3862
   23 if((-1)*((m)-(1)).gt.0) go to 31                                   3863
      t=tb(2,m)                                                          3864
      j=abs(t)+.1                                                        3865
      if(x(j).eq.flg) go to 22                                           3866
      k=m+1                                                              3867
      go to 25                                                           3868
   24 k=k+1                                                              3869
   25 if((k).gt.(no)) go to 30                                           3870
      td(1,k-1)=td(1,k)                                                  3871
      do 26 i=1,5                                                        3872
      tbn(i,k-1)=tbn(i,k)                                                3873
   26 continue                                                           3874
      i=k+1                                                              3875
      go to 28                                                           3876
   27 i=i+1                                                              3877
   28 if((i).gt.(no)) go to 24                                           3878
      if(int(tbn(4,i)+.1).eq.k) tbn(4,i)=k-1                             3879
      go to 27                                                           3880
   30 no=no-1                                                            3881
      go to 22                                                           3882
   31 m=no+1                                                             3883
      go to 33                                                           3884
   32 m=m+1                                                              3885
   33 if((m).gt.(nk)) go to 34                                           3886
      tbn(1,m)=0.0                                                       3887
      go to 32                                                           3888
   34 m=no                                                               3889
      go to 36                                                           3890
   35 m=m+(-1)                                                           3891
   36 if((-1)*((m)-(2)).gt.0) go to 39                                   3892
      if(tbn(1,m).eq.0.0) go to 35                                       3893
      nv=0                                                               3894
      ip=m                                                               3895
   37 if(ip.le.0) go to 38                                               3896
      nv=nv+1                                                            3897
      sc(2*nv-1)=tbn(2,ip)                                               3898
      sc(2*nv)=tbn(3,ip)                                                 3899
      ip=tbn(4,ip)+.1                                                    3900
      go to 37                                                           3901
   38 k=match(nv,sc,m-1,tbn,cm,r,1)                                      3902
      if(k.eq.0) go to 35                                                3903
      tbn(1,k)=tbn(1,k)+tbn(1,m)                                         3904
      td(1,k)=td(1,k)+td(1,m)                                            3905
      tbn(1,m)=0.0                                                       3906
      go to 35                                                           3907
   39 return                                                             3908
      end                                                                3909
      subroutine qslice (p,nk,tb,cm,td,kp,kv,lp,lv,tc,r,sc,js)           3910
      integer p,kp(5,1),kv(2,1),lp(3,1),lv(1),js(1)                      3911
      real tb(5,nk),cm(1),td(2,1),tc(1),r(p,2),sc(2,p)                   3912
      do 1 j=1,p                                                         3913
      sc(1,j)=r(j,2)                                                     3914
      sc(2,j)=sc(1,j)+r(j,1)                                             3915
    1 continue                                                           3916
      ll=1                                                               3917
      la=ll                                                              3918
      l1=la                                                              3919
    2 if(kp(1,ll).lt.0) go to 5                                          3920
      if(kp(3,ll) .gt. 0) go to 3                                        3921
      kp(5,ll)=0                                                         3922
      ll=ll+1                                                            3923
      go to 2                                                            3924
    3 kp3=kp(3,ll)                                                       3925
      kp(5,ll)=la                                                        3926
      do 4 m=1,kp3                                                       3927
      l=lp(1,l1)                                                         3928
      nt=lp(3,l1)                                                        3929
      call knts(l,nt,lv(lp(2,l1)),kp(1,ll),kv(1,kp(2,ll)),nk,tb,cm,tc(la 3930
     1),js)                                                              3931
      call side(l,nt,lv(lp(2,l1)),sc,tc(la))                             3932
      l1=l1+1                                                            3933
      la=la+nt*(5*l+1)                                                   3934
    4 continue                                                           3935
      ll=ll+1                                                            3936
      go to 2                                                            3937
    5 le=la-1                                                            3938
      ll=1                                                               3939
      la=ll                                                              3940
      l1=la                                                              3941
      laa=0                                                              3942
    6 if(kp(1,ll).lt.0) go to 13                                         3943
      nv=0                                                               3944
      if(kp(1,ll) .le. 0) go to 8                                        3945
      jl=kp(1,ll)                                                        3946
      do 7 il=1,jl                                                       3947
      k=kp(2,ll)+il-1                                                    3948
      nv=nv+1                                                            3949
      sc(1,nv)=kv(1,k)                                                   3950
      sc(2,nv)=kv(2,k)                                                   3951
    7 continue                                                           3952
      go to 9                                                            3953
    8 if(kp(3,ll) .gt. 0) go to 9                                        3954
      ll=ll+1                                                            3955
      go to 6                                                            3956
    9 if(kp(3,ll) .gt. 0) go to 10                                       3957
      m=match(nv,sc,nk,tb,cm,r,0)                                        3958
      le=le+1                                                            3959
      kp(3,ll)=-le                                                       3960
      tc(le)=td(1,m)                                                     3961
      ll=ll+1                                                            3962
      go to 6                                                            3963
   10 kp3=kp(3,ll)                                                       3964
      do 12 k=1,kp3                                                      3965
      l=lp(1,l1)                                                         3966
      nt=lp(3,l1)                                                        3967
      laa=laa+5*l*nt                                                     3968
      do 11 jp=1,nt                                                      3969
      call gtrm(2,jp,l,nt,lv(lp(2,l1)),dum,dum,nk,tb,tc(la),sc(1,nv+1),d 3970
     1um)                                                                3971
      m=match(nv+l,sc,nk,tb,cm,r,0)                                      3972
      tc(jp+laa)=td(1,m)                                                 3973
   11 continue                                                           3974
      laa=laa+nt                                                         3975
      l1=l1+1                                                            3976
      la=la+nt*(5*l+1)                                                   3977
   12 continue                                                           3978
      ll=ll+1                                                            3979
      go to 6                                                            3980
   13 return                                                             3981
      end                                                                3982
      function ieq(a,b,r)                                                3983
      ieq=0                                                              3984
      if(abs((a-b)/r).lt.1.e-5) ieq=1                                    3985
      return                                                             3986
      end                                                                3987
      function lcm (p,nk,tb,cm)                                          3988
      integer p                                                          3989
      real tb(5,nk),cm(1)                                                3990
      ix=0                                                               3991
      do 1 m=1,nk                                                        3992
      j=abs(tb(2,m))+.1                                                  3993
      if(cm(2*j).eq.0.0) go to 1                                         3994
      if(int(tb(3,m)+.1) .le. ix) go to 1                                3995
      ix=tb(3,m)+.1                                                      3996
      jj=j                                                               3997
    1 continue                                                           3998
      if(ix .le. 0) go to 2                                              3999
      lcm=ix+int(cm(2*jj+1)+.1)-int(cm(2*jj)+.1)+1                       4000
      return                                                             4001
    2 lcm=2*p+1                                                          4002
      do 3 j=1,p                                                         4003
      if(cm(2*j).eq.0.0) go to 3                                         4004
      lcm=lcm+int(cm(2*j+1)+.1)-int(cm(2*j)+.1)+1                        4005
    3 continue                                                           4006
      return                                                             4007
      end                                                                4008
      function newb (m,tb)                                               4009
      real tb(5,m)                                                       4010
      newb=0                                                             4011
      mm1=m-1                                                            4012
      do 1 k=1,mm1                                                       4013
      if(ieq(tb(2,k),tb(2,m),1.0).eq.0) go to 1                          4014
      if(ieq(tb(3,k),tb(3,m),1.0).eq.0) go to 1                          4015
      if(ieq(tb(4,k),tb(4,m),1.0).eq.0) go to 1                          4016
      newb=1                                                             4017
      go to 2                                                            4018
    1 continue                                                           4019
    2 return                                                             4020
      end                                                                4021
      subroutine sscp (n,m,sc,y,w,yb,yv,sw,d,da)                         4022
      real sc(n,1),y(n),w(n)                                             4023
      double precision d(m,m),da(1),yb,yv,sw,s                           4024
      mm1=m-1                                                            4025
      do 6 k=1,mm1                                                       4026
      s=0.d0                                                             4027
      do 1 i=1,n                                                         4028
      s=s+w(i)*sc(i,k)                                                   4029
    1 continue                                                           4030
      s=s/sw                                                             4031
      da(k)=s                                                            4032
      do 2 i=1,n                                                         4033
      sc(i,k)=sc(i,k)-s                                                  4034
    2 continue                                                           4035
      do 4 j=1,k                                                         4036
      s=0.d0                                                             4037
      do 3 i=1,n                                                         4038
      s=s+w(i)*sc(i,j)*sc(i,k)                                           4039
    3 continue                                                           4040
      d(j,k)=s                                                           4041
    4 continue                                                           4042
      s=0.d0                                                             4043
      do 5 i=1,n                                                         4044
      s=s+w(i)*sc(i,k)*(y(i)-yb)                                         4045
    5 continue                                                           4046
      d(k,m)=s                                                           4047
    6 continue                                                           4048
      d(m,m)=sw*yv                                                       4049
      return                                                             4050
      end                                                                4051
      subroutine lsf1 (d,m,xb,yb,al,rss,a,a0,dp)                         4052
      double precision d(m,m),xb(1),yb,al,rss,a(1),a0,dp(1),eps,s        4053
      data eps /1.d-4/                                                   4054
      mm1=m-1                                                            4055
      do 1 i=1,mm1                                                       4056
      dp(i)=d(i,i)                                                       4057
      d(i,i)=d(i,i)*(1.d0+al)                                            4058
    1 continue                                                           4059
      do 5 i=1,mm1                                                       4060
      if(dp(i).le.0.d0) go to 5                                          4061
      im1=i-1                                                            4062
      s=dp(i)                                                            4063
      j=1                                                                4064
      go to 3                                                            4065
    2 j=j+1                                                              4066
    3 if((j).gt.(im1)) go to 4                                           4067
      if(d(j,j).lt.0.d0) s=s+dp(j)*d(j,i)**2                             4068
      go to 2                                                            4069
    4 if((d(i,i)-al*s)/dp(i).lt.eps) go to 5                             4070
      call sweep(d,m,i,-1.d0,dp(m))                                      4071
    5 continue                                                           4072
      rss=0.d0                                                           4073
      a0=yb                                                              4074
      do 6 i=1,mm1                                                       4075
      a(i)=0.d0                                                          4076
      if(d(i,i).ge.0.d0) go to 6                                         4077
      a(i)=d(i,m)                                                        4078
      a0=a0-a(i)*xb(i)                                                   4079
      rss=rss+dp(i)*a(i)**2                                              4080
    6 continue                                                           4081
      rss=d(m,m)-al*rss                                                  4082
      return                                                             4083
      end                                                                4084
      subroutine bkstp (d,m,xb,yb,al,rss,a,a0,k,dp)                      4085
      double precision d(m,m),xb(1),yb,al                                4086
      double precision a(1),a0,rss,dp(1),s                               4087
      data big /9.9e30/                                                  4088
      mm1=m-1                                                            4089
      rss=big                                                            4090
      k=0                                                                4091
      do 4 i=1,mm1                                                       4092
      if(d(i,i).ge.0.d0) go to 4                                         4093
      s=0.d0                                                             4094
      do 3 j=1,mm1                                                       4095
      if(d(j,j).ge.0.d0) go to 3                                         4096
      if(j.eq.i) go to 3                                                 4097
      if(j .ge. i) go to 1                                               4098
      a0=d(j,i)                                                          4099
      go to 2                                                            4100
    1 a0=d(i,j)                                                          4101
    2 s=s+dp(j)*(d(j,m)-a0*d(i,m)/d(i,i))**2                             4102
    3 continue                                                           4103
      s=d(m,m)-d(i,m)**2/d(i,i)-al*s                                     4104
      if(s .gt. rss) go to 4                                             4105
      rss=s                                                              4106
      k=i                                                                4107
    4 continue                                                           4108
      if(k.gt.0) call sweep(d,m,k,1.d0,dp(m))                            4109
      a0=yb                                                              4110
      rss=0.d0                                                           4111
      do 5 i=1,mm1                                                       4112
      a(i)=0.d0                                                          4113
      if(d(i,i).ge.0.d0) go to 5                                         4114
      a(i)=d(i,m)                                                        4115
      a0=a0-a(i)*xb(i)                                                   4116
      rss=rss+dp(i)*a(i)**2                                              4117
    5 continue                                                           4118
      rss=d(m,m)-al*rss                                                  4119
      return                                                             4120
      end                                                                4121
      subroutine sweep (a,m,k,fl,u)                                      4122
      double precision a(m,m),u(m),fl,c                                  4123
      c=a(k,k)                                                           4124
      do 1 i=1,k                                                         4125
      u(i)=a(i,k)                                                        4126
      a(i,k)=0.d0                                                        4127
    1 continue                                                           4128
      do 2 i=k,m                                                         4129
      u(i)=a(k,i)                                                        4130
      a(k,i)=0.d0                                                        4131
    2 continue                                                           4132
      u(k)=fl                                                            4133
      do 4 i=1,m                                                         4134
      do 3 j=i,m                                                         4135
      a(i,j)=a(i,j)-u(i)*u(j)/c                                          4136
    3 continue                                                           4137
    4 continue                                                           4138
      return                                                             4139
      end                                                                4140
      subroutine array(p,n,i,j)                                          4141
      integer p                                                          4142
      i=mod(p,n)                                                         4143
      if(i.eq.0) i=n                                                     4144
      j=(p-i)/n+1                                                        4145
      return                                                             4146
      end                                                                4147
      subroutine cvmars  (ix,n,p,x,y,w,nk,ms,df,fv,mi,lx,it,xm,xs,tb,cm, 4148
     1sc,db,d,mm,wt,cv)                                                  4149
      integer p,mm(n,1),lx(p)                                            4150
      real x(n,p),y(n),w(n),xm(p),xs(p),tb(5,nk),cm(1),sc(1),wt(n,2),cv( 4151
     1nk,4)                                                              4152
      double precision db(n,1),d(nk,1)                                   4153
      data eps,big,dfs,cvm,im /1.e-6,9.9e30,2*0.0,0/                     4154
      if(it.gt.0) write(it,'(/,'' sample reuse to estimate df:'')')      4155
      if(ix .le. 0) go to 1                                              4156
      nr=ix                                                              4157
      nd=nr                                                              4158
      if(it.gt.0) write(it,'('' '',i3,'' - fold cross-validation.'',/)') 4159
     1 ix                                                                4160
      go to 2                                                            4161
    1 nr=1                                                               4162
      nd=-ix                                                             4163
      if(it.gt.0) write(it,'('' independent test set - every'',i4,'' obs 4164
     1ervations.'',/)') nd                                               4165
    2 do 3 i=1,n                                                         4166
      wt(i,1)=w(i)                                                       4167
      wt(i,2)=i                                                          4168
    3 continue                                                           4169
      do 4 i=1,n                                                         4170
      call rnms(r,1)                                                     4171
      k=(n-i+1)*r+i                                                      4172
      t=wt(i,2)                                                          4173
      wt(i,2)=wt(k,2)                                                    4174
      wt(k,2)=t                                                          4175
    4 continue                                                           4176
      do 6 i=1,3                                                         4177
      do 5 m=1,nk                                                        4178
      cv(m,i)=0.0                                                        4179
    5 continue                                                           4180
    6 continue                                                           4181
      cv0=0.0                                                            4182
      sw=cv0                                                             4183
      wn=sw                                                              4184
      yv=wn                                                              4185
      fc=yv                                                              4186
      do 14 ir=1,nr                                                      4187
      i=ir                                                               4188
    7 if(i.gt.n) go to 8                                                 4189
      wt(int(wt(i,2)+.1),1)=0.0                                          4190
      i=i+nd                                                             4191
      go to 7                                                            4192
    8 call marsgo (n,p,x,y,wt,nk,ms,df,fv,mi,lx,0,xm,xs,az,tb,cm,sc,db,d 4193
     1,mm)                                                               4194
      yv1=sc(3)                                                          4195
      yv=yv+yv1                                                          4196
      wn1=sc(2)                                                          4197
      wn=wn+wn1                                                          4198
      fc=fc+sc(1)                                                        4199
      mk=sc((nk+1)**2+4)+.1                                              4200
      i=ir                                                               4201
    9 if(i.gt.n) go to 10                                                4202
      k=wt(i,2)+.1                                                       4203
      wt(k,1)=w(k)                                                       4204
      sw=sw+w(k)                                                         4205
      call cvmod(k,n,x,y,w,nk,mk,tb,cm,sc,cv0,cv(1,3))                   4206
      i=i+nd                                                             4207
      go to 9                                                            4208
   10 do 13 m=1,nk                                                       4209
      am=sc(m+4)                                                         4210
      cv(m,2)=cv(m,2)+am                                                 4211
      am1=yv1                                                            4212
      if(m.gt.1) am1=sc(m+3)                                             4213
      if(am1/yv1 .le. eps) go to 11                                      4214
      r=sqrt(am/am1)                                                     4215
      go to 12                                                           4216
   11 r=1.0                                                              4217
   12 cv(m,1)=cv(m,1)+((wn1-1.0)*(1.0-r)/(m-r*(m-1))-1.0)/sc(1)          4218
   13 continue                                                           4219
   14 continue                                                           4220
      do 15 m=1,nk                                                       4221
      cv(m,1)=cv(m,1)/nr                                                 4222
      cv(m,2)=cv(m,2)/nr                                                 4223
      cv(m,3)=cv(m,3)/sw                                                 4224
   15 continue                                                           4225
      fc=fc/nr                                                           4226
      yv=yv/nr                                                           4227
      wn=wn/nr                                                           4228
      cv0=cv0/sw                                                         4229
      if(it.gt.0) write(it,21)                                           4230
      im=0                                                               4231
      cvm=cv0                                                            4232
      dmx=-big                                                           4233
      cvl=cv(nk,1)                                                       4234
      m=nk                                                               4235
      go to 17                                                           4236
   16 m=m+(-1)                                                           4237
   17 if((-1)*((m)-(1)).gt.0) go to 19                                   4238
      if(cv(m,1).le.dmx) go to 16                                        4239
      dmx=cv(m,1)                                                        4240
      dfu=0.5*(cvl+cv(m,1))                                              4241
      cvl=cv(m,1)                                                        4242
      if(cv(m,3) .gt. cvm) go to 18                                      4243
      cvm=cv(m,3)                                                        4244
      df=dfu                                                             4245
      im=m                                                               4246
   18 gcv=cv(m,2)/(1.0-((dfu*fc+1.0)*m+1.0)/wn)**2                       4247
      if(it.gt.0) write(it,22) m,dfu,cv(m,2),gcv,cv(m,3)                 4248
      go to 16                                                           4249
   19 if(cv0 .gt. cvm) go to 20                                          4250
      cvm=cv0                                                            4251
      df=dmx                                                             4252
      im=0                                                               4253
   20 dfs=df                                                             4254
      gcv=yv/(1.0-1.0/wn)**2                                             4255
      if(it.gt.0) write(it,22) 0,dmx,yv,gcv,cv0                          4256
      if(it.gt.0) write(it,'(/,'' estimated optimal df('',i3,'') ='',f7. 4257
     12,               '' with (estimated) pse ='',g12.4)') im,df,cvm    4258
      return                                                             4259
      entry cvinfo(a1,a2,ia1)                                            4260
      a1=dfs                                                             4261
      a2=cvm                                                             4262
      ia1=im                                                             4263
      return                                                             4264
   21 format('  #bsfns     df        asr           gcv           cv')    4265
   22 format(' ',i5,f10.2,3g14.4)                                        4266
      end                                                                4267
      subroutine cvmod (i,n,x,y,w,nk,mk,tb,cm,sc,cv0,cv)                 4268
      real x(n,1),y(n),w(n),tb(5,nk),cm(1),sc(1),cv(nk,2)                4269
      do 8 m=1,mk                                                        4270
      t=tb(2,m)                                                          4271
      j=abs(t)+.1                                                        4272
      if(cm(2*j) .le. 0.0) go to 5                                       4273
      k=x(i,j)+.1                                                        4274
      if(k .ne. 0) go to 1                                               4275
      u=0.0                                                              4276
      go to 2                                                            4277
    1 u=cm(k+int(tb(3,m)+.1))                                            4278
    2 if(t .ge. 0.0) go to 6                                             4279
      if(u .ne. 0.0) go to 3                                             4280
      u=1.0                                                              4281
      go to 6                                                            4282
    3 u=0.0                                                              4283
      go to 6                                                            4284
    5 u=amax1(0.0,sign(1.0,t)*(x(i,j)-tb(3,m)))                          4285
    6 l=tb(4,m)+.1                                                       4286
      if(l .le. 0) go to 7                                               4287
      cv(m,2)=u*cv(l,2)                                                  4288
      go to 8                                                            4289
    7 cv(m,2)=u                                                          4290
    8 continue                                                           4291
      kp=nk+4                                                            4292
      cv0=cv0+w(i)*(y(i)-sc(4))**2                                       4293
      do 10 m=1,nk                                                       4294
      kp=kp+1                                                            4295
      s=sc(kp)                                                           4296
      do 9 l=1,nk                                                        4297
      kp=kp+1                                                            4298
      if(l.le.mk) s=s+sc(kp)*cv(l,2)                                     4299
    9 continue                                                           4300
      cv(m,1)=cv(m,1)+w(i)*(y(i)-s)**2                                   4301
   10 continue                                                           4302
      return                                                             4303
      end                                                                4304
      subroutine nest (i,j,nv,vals)                                      4305
      parameter (mlist=200, nlist=2000)                                  4306
      real vals(1),vm(nlist),tb(5,1),cm(1),x(n,1),bl(1)                  4307
      integer m(4,mlist),p,lx(1)                                         4308
      save m,vm                                                          4309
      data il,jl /2*0/                                                   4310
      if((i .ne. 0) .and. (j .ne. 0)) go to 1                            4311
      il=0                                                               4312
      jl=il                                                              4313
      return                                                             4314
    1 if(i.eq.j) return                                                  4315
      ig=0                                                               4316
      if(nv .le. 0) go to 8                                              4317
      k=1                                                                4318
      go to 3                                                            4319
    2 k=k+1                                                              4320
    3 if((k).gt.(il)) go to 4                                            4321
      if(m(1,k).eq.i.or.m(1,k).eq.j) return                              4322
      go to 2                                                            4323
    4 il=il+1                                                            4324
      if(il .le. mlist) go to 5                                          4325
      write(6,  '('' increase parameter mlist in subroutine nest to grea 4326
     1ter than'',               i5,'' and recompile.'')') il             4327
      stop                                                               4328
    5 m(1,il)=i                                                          4329
      m(2,il)=j                                                          4330
      m(3,il)=nv                                                         4331
      m(4,il)=jl                                                         4332
      if(jl+nv .le. nlist) go to 6                                       4333
      write(6,  '('' increase parameter nlist in subroutine nest to grea 4334
     1ter than'',               i5,'' and recompile.'')') jl+nv          4335
      stop                                                               4336
    6 do 7 k=1,nv                                                        4337
      jl=jl+1                                                            4338
      vm(jl)=vals(k)                                                     4339
    7 continue                                                           4340
      return                                                             4341
    8 k=1                                                                4342
      go to 10                                                           4343
    9 k=k+1                                                              4344
   10 if((k).gt.(il)) go to 12                                           4345
      if(m(1,k) .ne. i .or. m(2,k) .ne. j) go to 9                       4346
      ig=1                                                               4347
   12 if(ig.eq.0) return                                                 4348
      il=il-1                                                            4349
      ll=k                                                               4350
      go to 14                                                           4351
   13 ll=ll+1                                                            4352
   14 if((ll).gt.(il)) go to 16                                          4353
      do 15 l=1,4                                                        4354
      m(l,ll)=m(l,ll+1)                                                  4355
   15 continue                                                           4356
      go to 13                                                           4357
   16 return                                                             4358
      entry nstlst(it)                                                   4359
      if(it.le.0) return                                                 4360
      if(il.eq.0) return                                                 4361
      write(it,'(/,'' variable nesting:'',/)')                           4362
      do 18 k=1,il                                                       4363
      if(m(3,k) .le. 5) go to 17                                         4364
      write(it,'('' '',i3,'': var('',i3,'') exists for var('',i3,'') ='' 4365
     1)')  k,m(1,k),m(2,k)                                               4366
      write(it,'(100('' '',10f7.1))') (vm(l),l=m(4,k)+1,m(4,k)+m(3,k))   4367
      go to 18                                                           4368
   17 write(it,'('' '',i3,'': var('',i3,'') exists for var('',i3,'') ='' 4369
     1,5f7.1)')  k,m(1,k),m(2,k),(vm(l),l=m(4,k)+1,m(4,k)+m(3,k))        4370
   18 continue                                                           4371
      return                                                             4372
      entry oknest(it,p,lx,cm)                                           4373
      if(it.le.0) return                                                 4374
      l=1                                                                4375
      go to 20                                                           4376
   19 l=l+1                                                              4377
   20 if((l).gt.(il)) go to 24                                           4378
      j1=m(1,l)                                                          4379
      jn=m(2,l)                                                          4380
      jv=m(3,l)                                                          4381
      jp=m(4,l)                                                          4382
      if(j1.lt.1.or.j1.gt.p) write(it,25) l,j1                           4383
      if(jn.lt.1.or.jn.gt.p) write(it,26) l,jn                           4384
      if(lx(jn).ge.0) write(it,27) l,jn,lx(jn)                           4385
      k1=cm(2*jn)+.1                                                     4386
      k2=cm(2*jn+1)+.1                                                   4387
      do 23 k=jp+1,jp+jv                                                 4388
      ig=0                                                               4389
      do 21 kk=k1,k2                                                     4390
      if(vm(k) .ne. cm(kk)) go to 21                                     4391
      ig=1                                                               4392
      go to 22                                                           4393
   21 continue                                                           4394
   22 if(ig.eq.0) write(it,28) l,vm(k),jn                                4395
   23 continue                                                           4396
      go to 19                                                           4397
   24 return                                                             4398
   25 format(' nesting entry',i3,', invalid variable',i3,' to be nested. 4399
     1')                                                                 4400
   26 format(' nesting entry',i3,', invalid nesting variable',i3,'.')    4401
   27 format(' nesting entry',i3,', lx(',i3,') =',i2,'. must be < 0.')   4402
   28 format(' nesting entry',i3,', categorical value ',g12.4,/,  ' not  4403
     1among the data values for variable',i3,'.')                        4404
      entry isnstr(j,jb)                                                 4405
      jb=0                                                               4406
      k=1                                                                4407
      go to 30                                                           4408
   29 k=k+1                                                              4409
   30 if((k).gt.(il)) go to 32                                           4410
      if(m(2,k) .ne. j) go to 29                                         4411
      jb=m(1,k)                                                          4412
   32 return                                                             4413
      entry isfac (lm,j,mk,tb,cm,ja)                                     4414
      ja=0                                                               4415
      ig=ja                                                              4416
      l=1                                                                4417
      go to 34                                                           4418
   33 l=l+1                                                              4419
   34 if((l).gt.(il)) go to 36                                           4420
      if(j .ne. m(1,l)) go to 33                                         4421
      ig=1                                                               4422
   36 if(ig.eq.0) return                                                 4423
      jn=m(2,l)                                                          4424
      if(cm(2*jn).eq.0.0) return                                         4425
      jv=m(3,l)                                                          4426
      jp=m(4,l)                                                          4427
      ig=0                                                               4428
      ip=lm                                                              4429
   37 if(ip.le.0) go to 39                                               4430
      j1=abs(tb(2,ip))+.1                                                4431
      if(j1 .ne. jn) go to 38                                            4432
      ig=1                                                               4433
      go to 39                                                           4434
   38 ip=tb(4,ip)+.1                                                     4435
      go to 37                                                           4436
   39 if(ig .eq. 0) go to 45                                             4437
      nc=cm(2*jn+1)-cm(2*jn)+1.1                                         4438
      t=tb(2,ip)                                                         4439
      kp=tb(3,ip)+.1                                                     4440
      do 44 l=1,nc                                                       4441
      lp=l+kp                                                            4442
      if(t .le. 0) go to 40                                              4443
      if(cm(lp).eq.0.0) go to 44                                         4444
      go to 41                                                           4445
   40 if(cm(lp).ne.0.0) go to 44                                         4446
   41 ex=cm(int(cm(2*jn)+.1)+l-1)                                        4447
      ig=0                                                               4448
      do 42 k=jp+1,jp+jv                                                 4449
      if(ex .ne. vm(k)) go to 42                                         4450
      ig=1                                                               4451
      go to 43                                                           4452
   42 continue                                                           4453
   43 if(ig .ne. 0) go to 44                                             4454
      ja=-1                                                              4455
      return                                                             4456
   44 continue                                                           4457
      return                                                             4458
   45 ja=l                                                               4459
      norm=nord(lm,tb)+1                                                 4460
      nc=cm(2*jn+1)-cm(2*jn)+1.1                                         4461
      do 56 lk=1,mk                                                      4462
      if(nord(lk,tb).ne.norm) go to 56                                   4463
      jg=0                                                               4464
      ip=lk                                                              4465
   46 if(ip.le.0) go to 55                                               4466
      t1=tb(2,ip)                                                        4467
      j1=abs(t1)+.1                                                      4468
      if(j1 .ne. jn) go to 54                                            4469
      kp=tb(3,ip)+.1                                                     4470
      kg=0                                                               4471
      do 52 l=1,nc                                                       4472
      lp=l+kp                                                            4473
      lon=cm(lp)+.1                                                      4474
      if(t1 .ge. 0.0) go to 48                                           4475
      if(lon .ne. 0) go to 47                                            4476
      lon=1                                                              4477
      go to 48                                                           4478
   47 lon=0                                                              4479
   48 ex=cm(int(cm(2*jn)+.1)+l-1)                                        4480
      ig=0                                                               4481
      do 49 k=jp+1,jp+jv                                                 4482
      if(ex .ne. vm(k)) go to 49                                         4483
      ig=1                                                               4484
      go to 50                                                           4485
   49 continue                                                           4486
   50 if(lon .ne. 1 .or. ig .ne. 0) go to 51                             4487
      kg=1                                                               4488
      go to 53                                                           4489
   51 if(lon .ne. 0 .or. ig .ne. 1) go to 52                             4490
      kg=1                                                               4491
      go to 53                                                           4492
   52 continue                                                           4493
   53 if(kg .ne. 0) go to 54                                             4494
      jg=1                                                               4495
      go to 55                                                           4496
   54 ip=tb(4,ip)+.1                                                     4497
      go to 46                                                           4498
   55 if(jg.eq.0) go to 56                                               4499
      if(ieqbf(lk,lm,tb,cm) .ne. 1) go to 56                             4500
      ja=-1                                                              4501
      return                                                             4502
   56 continue                                                           4503
      return                                                             4504
      entry cmpnst(ja,n,x,cm,bl)                                         4505
      jn=m(2,ja)                                                         4506
      jv=m(3,ja)                                                         4507
      jp=m(4,ja)                                                         4508
      do 59 l=1,n                                                        4509
      kx=x(l,jn)+.1                                                      4510
      ex=cm(int(cm(2*jn)+.1)+kx-1)                                       4511
      ig=0                                                               4512
      do 57 k=jp+1,jp+jv                                                 4513
      if(ex .ne. vm(k)) go to 57                                         4514
      ig=1                                                               4515
      go to 58                                                           4516
   57 continue                                                           4517
   58 if(ig.eq.1) go to 59                                               4518
      bl(l)=0.0                                                          4519
   59 continue                                                           4520
      return                                                             4521
      entry getnst(ja,cm,j,nv,vals)                                      4522
      j=m(2,ja)                                                          4523
      jv=m(3,ja)                                                         4524
      jp=m(4,ja)                                                         4525
      nv=cm(2*j+1)-cm(2*j)+1.1                                           4526
      do 60 k=1,nv                                                       4527
      vals(k)=0.0                                                        4528
   60 continue                                                           4529
      do 61 l=jp+1,jp+jv                                                 4530
      k=icat(vm(l),j,cm)                                                 4531
      if(k.gt.0) vals(k)=1.0                                             4532
   61 continue                                                           4533
      return                                                             4534
      end                                                                4535
      subroutine blf0(l,ja,n,x,w,cm,sc,nnt,bl)                           4536
      real x(n,1),w(n),cm(1),sc(n,1),bl(n)                               4537
      nnt=0                                                              4538
      call blf(l,n,sc,bl)                                                4539
      if(ja.gt.0) call cmpnst(ja,n,x,cm,bl)                              4540
      do 1 i=1,n                                                         4541
      if(bl(i).gt.0.0.and.w(i).gt.0.0) nnt=nnt+1                         4542
    1 continue                                                           4543
      return                                                             4544
      end                                                                4545
      subroutine blf(l,n,sc,bl)                                          4546
      real sc(n,1),bl(n)                                                 4547
      if(l .gt. 0) go to 2                                               4548
      do 1 i=1,n                                                         4549
      bl(i)=1.0                                                          4550
    1 continue                                                           4551
      go to 4                                                            4552
    2 do 3 i=1,n                                                         4553
      bl(i)=sc(i,l)                                                      4554
    3 continue                                                           4555
    4 return                                                             4556
      end                                                                4557
      subroutine mnspan(ms,alf,nep,nnt,mn,me,mel)                        4558
      parameter(al2=0.693147,al25=1.732868)                              4559
      allf=-alog(1.0-alf)                                                4560
      fmn=-alog(allf/(nep*nnt))/al25                                     4561
      fme=-alog(alf*0.125/nep)/al2                                       4562
      if(ms .le. 0) go to 1                                              4563
      me=ms*fme/fmn+0.5                                                  4564
      mn=ms                                                              4565
      go to 2                                                            4566
    1 me=fme+0.5                                                         4567
      mn=fmn+0.5                                                         4568
    2 me=max0(me,mn,2)                                                   4569
      nst=nnt-2*me-1                                                     4570
      nnr=nst/mn                                                         4571
      nnl=nst-nnr*mn                                                     4572
      nnr=(nnr+1)*mn-nst                                                 4573
      nst=min0(nnl,nnr)                                                  4574
      if(nnl .gt. nnr) go to 3                                           4575
      nnl=1                                                              4576
      go to 4                                                            4577
    3 nnl=-1                                                             4578
    4 nnr=nst/2                                                          4579
      mel=me                                                             4580
      me=me+nnl*nnr                                                      4581
      mel=mel+nnl*nnr                                                    4582
      if(mod(nst,2).ne.0) mel=mel+nnl                                    4583
      return                                                             4584
      end                                                                4585
      function ieqbf(lk,lm,tb,cm)                                        4586
      real tb(5,1),cm(1)                                                 4587
      ipo=lm                                                             4588
      lg=0                                                               4589
    1 if(ipo.le.0) go to 16                                              4590
      to=tb(2,ipo)                                                       4591
      jo=abs(to)+.1                                                      4592
      jg=0                                                               4593
      if(cm(2*jo) .ne. 0.0) go to 2                                      4594
      t=tb(3,ipo)                                                        4595
      ic=0                                                               4596
      go to 3                                                            4597
    2 ko=tb(3,ipo)+.1                                                    4598
      nc=cm(2*jo+1)-cm(2*jo)+1.1                                         4599
      ic=1                                                               4600
    3 ip=lk                                                              4601
    4 if(ip.le.0) go to 14                                               4602
      t1=tb(2,ip)                                                        4603
      j1=abs(t1)+.1                                                      4604
      if(j1 .ne. jo) go to 13                                            4605
      if(ic .ne. 0) go to 6                                              4606
      if(to*t1 .le. 0.0) go to 13                                        4607
      if(ieq(t,tb(3,ip),1.0) .ne. 1) go to 13                            4608
      jg=1                                                               4609
      go to 14                                                           4610
    6 kp=tb(3,ip)+.1                                                     4611
      kg=0                                                               4612
      do 11 l=1,nc                                                       4613
      lo=l+ko                                                            4614
      lp=l+kp                                                            4615
      lon=cm(lo)+.1                                                      4616
      lop=cm(lp)+.1                                                      4617
      if(to .ge. 0.0) go to 8                                            4618
      if(lon .ne. 0) go to 7                                             4619
      lon=1                                                              4620
      go to 8                                                            4621
    7 lon=0                                                              4622
    8 if(t1 .ge. 0.0) go to 10                                           4623
      if(lop .ne. 0) go to 9                                             4624
      lop=1                                                              4625
      go to 10                                                           4626
    9 lop=0                                                              4627
   10 if(lon .eq. lop) go to 11                                          4628
      kg=1                                                               4629
      go to 12                                                           4630
   11 continue                                                           4631
   12 if(kg .ne. 0) go to 13                                             4632
      jg=1                                                               4633
      go to 14                                                           4634
   13 ip=tb(4,ip)+.1                                                     4635
      go to 4                                                            4636
   14 if(jg .ne. 0) go to 15                                             4637
      lg=1                                                               4638
      go to 16                                                           4639
   15 ipo=tb(4,ipo)+.1                                                   4640
      go to 1                                                            4641
   16 if(lg .ne. 0) go to 17                                             4642
      ieqbf=1                                                            4643
      go to 18                                                           4644
   17 ieqbf=0                                                            4645
   18 return                                                             4646
      end                                                                4647
      function ibfext(m,tb,cm)                                           4648
      real tb(5,1),cm(1)                                                 4649
      mm1=m-1                                                            4650
      ibfext=0                                                           4651
      norm=nord(m,tb)                                                    4652
      do 1 l=1,mm1                                                       4653
      if(nord(l,tb).ne.norm) go to 1                                     4654
      if(ieqbf(l,m,tb,cm) .eq. 0) go to 1                                4655
      ibfext=1                                                           4656
      return                                                             4657
    1 continue                                                           4658
      return                                                             4659
      end                                                                4660
      subroutine miss (n,p,x,lx,xm,flg,pn,xn,lxn,xs,xp)                  4661
      integer p,pn,lx(1),lxn(1)                                          4662
      real x(n,1),xm(1),xn(n,1),xs(1),xp(1)                              4663
      double precision s                                                 4664
      pn=p                                                               4665
      xp(1)=p                                                            4666
      do 1 j=2,2*p+1                                                     4667
      xp(j)=0.0                                                          4668
    1 continue                                                           4669
      ss=0.0                                                             4670
      do 7 j=1,p                                                         4671
      lxn(j)=lx(j)                                                       4672
      xs(j)=flg                                                          4673
      if(lx(j).eq.0) go to 7                                             4674
      s=0.d0                                                             4675
      mf=0                                                               4676
      do 4 i=1,n                                                         4677
      xn(i,j)=x(i,j)                                                     4678
      if(x(i,j) .ne. xm(j)) go to 2                                      4679
      mf=mf+1                                                            4680
      go to 4                                                            4681
    2 if(lx(j) .ge. 0) go to 3                                           4682
      ss=x(i,j)                                                          4683
      go to 4                                                            4684
    3 s=s+x(i,j)                                                         4685
    4 continue                                                           4686
      if(mf.eq.0) go to 7                                                4687
      if(mf .ne. n) go to 5                                              4688
      lxn(j)=0                                                           4689
      go to 7                                                            4690
    5 s=s/(n-mf)                                                         4691
      pn=pn+1                                                            4692
      lxn(pn)=-1                                                         4693
      xs(pn)=1.0                                                         4694
      xp(j+1)=pn                                                         4695
      call nest(j,pn,1,1.0)                                              4696
      if(lx(j).gt.0) ss=s                                                4697
      xp(j+p+1)=ss                                                       4698
      do 6 i=1,n                                                         4699
      xn(i,pn)=1.0                                                       4700
      if(x(i,j) .ne. xm(j)) go to 6                                      4701
      xn(i,j)=ss                                                         4702
      xn(i,pn)=0.0                                                       4703
    6 continue                                                           4704
    7 continue                                                           4705
      return                                                             4706
      end                                                                4707
      subroutine mkmiss (n,p,x,y,w,xm,pm,nnx,nn,xnn,yn,wn,sc)            4708
      parameter(nlist=500)                                               4709
      integer p,m(nlist)                                                 4710
      real pm(p),xm(p),x(n,p),y(n),w(n),xnn(1),yn(1),wn(1),sc(p,1)       4711
      data tol /0.001/                                                   4712
      if(p .le. nlist) go to 1                                           4713
      write(6,'('' increase parameter nlist in subroutine mkmiss to '',i 4714
     15,                      '' and recompile.'')') p                   4715
      stop                                                               4716
    1 do 3 j=1,p                                                         4717
      m(j)=0                                                             4718
      do 2 i=1,n                                                         4719
      if(x(i,j).eq.xm(j)) m(j)=m(j)+1                                    4720
      sc(j,i)=x(i,j)                                                     4721
      yn(i)=y(i)                                                         4722
      wn(i)=w(i)                                                         4723
    2 continue                                                           4724
    3 continue                                                           4725
      nn=n                                                               4726
      jp=0                                                               4727
      km=jp                                                              4728
    4 if(nn.ge.nnx.or.km.gt.p) go to 13                                  4729
      jp=jp+1                                                            4730
      if(jp.gt.p) jp=1                                                   4731
      fin=nn*pm(jp)-m(jp)                                                4732
      if(fin .le. 0.0) go to 5                                           4733
      in=fin+0.5                                                         4734
      go to 6                                                            4735
    5 in=0                                                               4736
    6 in=min0(in,nnx-nn)                                                 4737
      if(in .le. 0) go to 7                                              4738
      km=0                                                               4739
      go to 8                                                            4740
    7 km=km+1                                                            4741
      go to 4                                                            4742
    8 do 11 k=1,in                                                       4743
      call rnms(r,1)                                                     4744
      i=nn*r+1.0                                                         4745
      nnk=nn+k                                                           4746
      do 9 j=1,p                                                         4747
      sc(j,nnk)=sc(j,i)                                                  4748
    9 continue                                                           4749
      sc(jp,nnk)=xm(jp)                                                  4750
      yn(nnk)=yn(i)                                                      4751
      wn(nnk)=wn(i)                                                      4752
      do 10 j=1,p                                                        4753
      if(sc(j,nnk).eq.xm(j)) m(j)=m(j)+1                                 4754
   10 continue                                                           4755
   11 continue                                                           4756
      nn=nn+in                                                           4757
      cvx=-9.9e30                                                        4758
      do 12 j=1,p                                                        4759
      cvx=amax1(cvx,(nn*pm(j)-m(j))/float(nn))                           4760
   12 continue                                                           4761
      if(cvx.lt.tol) go to 13                                            4762
      go to 4                                                            4763
   13 k=0                                                                4764
      do 15 j=1,p                                                        4765
      do 14 i=1,nn                                                       4766
      k=k+1                                                              4767
      xnn(k)=sc(j,i)                                                     4768
   14 continue                                                           4769
   15 continue                                                           4770
      return                                                             4771
      entry smktol(val)                                                  4772
      tol=val                                                            4773
      return                                                             4774
      end                                                                4775
      subroutine xmiss (n,x,xm,xp,xn)                                    4776
      real x(n,1),xm(1),xp(1),xn(n,1)                                    4777
      integer p                                                          4778
      p=xp(1)+.1                                                         4779
      do 3 j=1,p                                                         4780
      k=xp(j+1)+.1                                                       4781
      do 2 i=1,n                                                         4782
      if(x(i,j) .eq. xm(j)) go to 1                                      4783
      xn(i,j)=x(i,j)                                                     4784
      if(k.gt.0) xn(i,k)=1.0                                             4785
      go to 2                                                            4786
    1 xn(i,j)=xp(j+p+1)                                                  4787
      if(k.gt.0) xn(i,k)=0.0                                             4788
    2 continue                                                           4789
    3 continue                                                           4790
      return                                                             4791
      end                                                                4792
      function nnord(m,tb)                                               4793
      real tb(5,1)                                                       4794
      ip=m                                                               4795
      nnord=0                                                            4796
    1 if(ip.le.0) go to 2                                                4797
      call isnstr(int(abs(tb(2,ip))+.1),jb)                              4798
      if(jb.eq.0) nnord=nnord+1                                          4799
      ip=tb(4,ip)+.1                                                     4800
      go to 1                                                            4801
    2 return                                                             4802
      end                                                                4803
      subroutine spofa(a,m,n,info)
      double precision a(m,1),s,t,u
         j1 = info
         do 30 j = j1, n
            info=j
            s = 0.0d0
            jm1 = j - 1
            if (jm1 .lt. 1) go to 20
            do 10 k = 1, jm1
               u=0.0
               km1=k-1
               if(km1.le.0) go to 40
               do 50 i=1,km1
                  u=u+a(i,k)*a(i,j)
   50          continue
   40          continue
               t = a(k,j) - u
               t = t/a(k,k)
               a(k,j) = t
               s = s + t*t
   10       continue
   20       continue
            s = a(j,j) - s
            if (s .le. 0.0d0)  return
            a(j,j) = dsqrt(s)
   30    continue
      info=0
      return
      end
      subroutine sposl(a,m,n,b)
      double precision a(m,1),b(1),t
      do 10 k = 1, n
         t = 0.0
         km1=k-1
         if(km1.le.0) go to 30
         do 40 i=1,km1
            t=t+a(i,k)*b(i)
   40    continue
   30    continue
         b(k) = (b(k) - t)/a(k,k)
   10 continue
      do 20 kb=1,n
      k=n+1-kb
      b(k)=b(k)/a(k,k)
      t=-b(k)
      km1=k-1
      if(km1.le.0) go to 50
      if(t.eq.0.0) go to 50
      do 60 i=1,km1
         b(i)=b(i)+t*a(i,k)
   60 continue
   50 continue
   20 continue
      return
      end
      subroutine psort (v,a,ii,jj)
c
c     puts into a the permutation vector which sorts v into
c     increasing order. the array v is not modified.
c     only elements from ii to jj are considered.
c     arrays iu(k) and il(k) permit sorting up to 2**(k+1)-1 elements
c
c     this is a modification of cacm algorithm #347 by r. c. singleton,
c     which is a modified hoare quicksort.
c
      dimension a(jj),v(jj),iu(20),il(20)
      integer t,tt
      integer a
      real v
      m=1
      i=ii
      j=jj
 10   if (i.ge.j) go to 80
 20   k=i
      ij=(j+i)/2
      t=a(ij)
      vt=v(t)
      if (v(a(i)).le.vt) go to 30
      a(ij)=a(i)
      a(i)=t
      t=a(ij)
      vt=v(t)
 30   l=j
      if (v(a(j)).ge.vt) go to 50
      a(ij)=a(j)
      a(j)=t
      t=a(ij)
      vt=v(t)
      if (v(a(i)).le.vt) go to 50
      a(ij)=a(i)
      a(i)=t
      t=a(ij)
      vt=v(t)
      go to 50
 40   a(l)=a(k)
      a(k)=tt
 50   l=l-1
      if (v(a(l)).gt.vt) go to 50
      tt=a(l)
      vtt=v(tt)
 60   k=k+1
      if (v(a(k)).lt.vt) go to 60
      if (k.le.l) go to 40
      if (l-i.le.j-k) go to 70
      il(m)=i
      iu(m)=l
      i=k
      m=m+1
      go to 90
 70   il(m)=k
      iu(m)=j
      j=l
      m=m+1
      go to 90
 80   m=m-1
      if (m.eq.0) return
      i=il(m)
      j=iu(m)
 90   if (j-i.gt.10) go to 20
      if (i.eq.ii) go to 10
      i=i-1
 100  i=i+1
      if (i.eq.j) go to 80
      t=a(i+1)
      vt=v(t)
      if (v(a(i)).le.vt) go to 100
      k=i
 110  a(k+1)=a(k)
      k=k-1
      if (vt.lt.v(a(k))) go to 110
      a(k+1)=t
      go to 100
      end
      subroutine stseed (iseed)
      real x(1)
      double precision u
      data i /987654321/
      i=iseed
      return
      entry rnms (x,n)
      do 1 j=1,n
      i=dmod(i*16807.d0,2147483647.d0)
      u=i
      u=u*.465661287d-9
      x(j)=u
    1 continue
      return
      end


