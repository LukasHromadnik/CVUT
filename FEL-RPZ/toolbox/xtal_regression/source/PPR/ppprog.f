c
c Projection Pursuit Regression (SMART).
c
c coded  and copywrite (c) 1984 by:
c
c                          Jerome H. Friedman
c                       Department of Statistics
c                                 and
c                  Stanford Linear Accelerator Center
c                          Stanford University
c
c all rights reserved.
c
c
c Methodology described in:
c
c Friedman, J. H. (1985) Classification and multiple regression through
c    projection pursuit. Deptartment of Statistics, Stanford University,
c    Technical Report LCS12.
c
c Documentation for this program described in:
c
c Friedman, J. H. (1984) SMART user's guide. Deptpartment of Statistics,
c    Stanford University, Technical Report LCS1.
c
c
      subroutine logo                                                       103
      common /parms/ ifl,lf,span,alpha,big                                  104
      if (ifl.gt.0) write (ifl,10)                                          105
      return                                                                106
 10   format('1                  **********************************'/  '    115
     1                   **********************************'/  '            115
     2           ******                      ******'/  '                    115
     3   *****         smart          *****'/  '                   *****    115
     4        modeling        *****'/  '                   *****    vers    115
     5ion 10/10/84    *****'/  '                   ******                   115
     6       ******'/  '                   *****************************    115
     7*****'/  '                   **********************************'/)    115
      end                                                                   116
      subroutine smartr (m,mu,p,q,n,w,x,y,ww,smod,nsmod,sp,nsp,dp,ndp)      117
      integer p,q                                                           118
      real x(p,n),y(q,n),w(n),ww(q),smod(nsmod),sp(nsp)                     119
      double precision dp(ndp)                                              120
      common /parms/ ifl,lf,span,alpha,big                                  121
      call logo                                                             122
      if (ifl.gt.0) write (ifl,10)                                          123
      call diagr (0,m,mu,p,q,n,w,ww,ierr)                                   123
      if (ierr.ne.0) return                                                 124
      call chkmem (nsmod,m*(p+q+2*n)+q+7,nsp,n*(q+15)+q+3*p,ndp,(p*(p+1)    126
     1)/2+6*p,ierr)                                                         126
      if (ierr.ne.0) return                                                 127
      smod(1)=m                                                             127
      smod(2)=p                                                             127
      smod(3)=q                                                             127
      smod(4)=n                                                             128
      call smart1 (0,m,mu,p,q,n,w,x,y,ww,smod(6),smod(q+6),smod(q+7),smo    132
     1d(q+7+p*m),smod(q+7+m*(p+q)),smod(q+7+m*(p+q+n)),smod(q+7+m*(p+q+2    132
     2*n)),sp,sp(q*n+1),sp(n*(q+15)+1),sp(n*(q+15)+q+1),dp,dum1,dum2,smo    132
     3d(5))                                                                 132
      return                                                                133
 10   format('0welcome to smart regression.'/)                              134
      end                                                                   135
      subroutine smartc (m,mu,p,q,n,w,x,y,pi,fls,smod,nsmod,sp,nsp,dp,nd    136
     1p)                                                                    136
      integer p,q                                                           137
      real x(p,n),y(n),w(n),pi(q),fls(q,q),smod(nsmod),sp(nsp)              138
      double precision dp(ndp)                                              139
      common /parms/ ifl,lf,span,alpha,big                                  140
      common /z00001/ maxit,conv,mitone,cutmin,fdel,cjeps,mitcj             141
      call logo                                                             142
      if (ifl.gt.0) write (ifl,100)                                         143
      call diagr (1,m,mu,p,q,n,w,sp,ierr)                                   143
      if (ierr.ne.0) return                                                 144
      call diagc (q,n,pi,fls,y,jerr)                                        145
      jsp=n*(q+15)+q+3*p                                                    145
      jmod=m*(p+q+2*n)+q+7                                                  146
      nmod=jmod                                                             146
      if (pi(1).lt.big) nmod=jmod+q                                         147
      call chkmem (nsmod,nmod,nsp,jsp+q,ndp,(p*(p+1))/2+6*p,ierr)           148
      if (ierr.ne.0.or.jerr.ne.0) return                                    149
      do 10 i=1,q                                                           149
      sp(jsp+i)=1.0                                                         149
 10   continue                                                              150
      if (pi(1).ge.big) go to 60                                            150
      do 20 i=1,q                                                           150
      smod(jmod+i)=0.0                                                      150
 20   continue                                                              151
      do 30 j=1,n                                                           151
      k=y(j)+.1                                                             151
      smod(jmod+k)=smod(jmod+k)+w(j)                                        151
 30   continue                                                              152
      sw=0.0                                                                152
      do 40 i=1,q                                                           152
      sw=sw+smod(jmod+i)                                                    152
 40   continue                                                              153
      do 50 i=1,q                                                           154
      if (smod(jmod+i).le.0.0) go to 50                                     154
      smod(jmod+i)=pi(i)*sw/smod(jmod+i)                                    155
      sp(jsp+i)=sp(jsp+i)*smod(jmod+i)                                      156
 50   continue                                                              158
 60   if (fls(1,1).ge.big) go to 90                                         160
      do 80 i=1,q                                                           160
      s=0.0                                                                 160
      do 70 j=1,q                                                           160
      s=s+fls(i,j)                                                          160
 70   continue                                                              161
      sp(jsp+i)=sp(jsp+i)*s                                                 162
 80   continue                                                              163
 90   smod(1)=m                                                             164
      smod(2)=p                                                             164
      smod(3)=q                                                             164
      smod(4)=n                                                             165
      call smart1 (1,m,mu,p,q,n,w,x,y,sp(jsp+1),smod(6),smod(q+6),smod(q    169
     1+7),smod(q+7+p*m),smod(q+7+m*(p+q)),smod(q+7+m*(p+q+n)),smod(q+7+m    169
     2*(p+q+2*n)),sp,sp(q*n+1),sp(n*(q+15)+1),sp(n*(q+15)+q+1),dp,pi,fls    169
     3,smod(5))                                                             169
      return                                                                170
 100  format('0welcome to smart classification.'/)                          171
      end                                                                   172
      subroutine smart1 (icl,m,mu,p,q,n,w,x,y,ww,yb,ys,a,b,f,t,asr,r,sc,    174
     1bt,g,dp,pi,fls,flm)                                                   174
      integer p,q                                                           176
      real w(n),x(p,n),y(1),ww(q),a(p,m),f(n,m),t(n,m),r(q,n)               177
      real sc(n,15),g(p,3),yb(q),b(q,m),bt(q),pi(q),fls(q,q)                178
      double precision dp(1)                                                179
      common /parms/ ifl,lf,span,alpha,big                                  180
      common /z00001/ maxit,conv,mitone,cutmin,fdel,cjeps,mitcj             181
      sw=0.0                                                                181
      do 10 j=1,n                                                           181
      sw=sw+w(j)                                                            181
 10   continue                                                              182
      if (icl.ne.0) go to 40                                                182
      do 30 j=1,n                                                           182
      do 20 i=1,q                                                           182
      r(i,j)=y(q*(j-1)+i)                                                   182
 20   continue                                                              182
 30   continue                                                              182
      go to 70                                                              183
 40   do 60 j=1,n                                                           183
      do 50 i=1,q                                                           183
      r(i,j)=0.0                                                            183
 50   continue                                                              183
      k=y(j)+.1                                                             183
      r(k,j)=1.0                                                            183
 60   continue                                                              183
 70   do 90 i=1,q                                                           184
      s=0.0                                                                 184
      do 80 j=1,n                                                           184
      s=s+w(j)*r(i,j)                                                       184
 80   continue                                                              184
      yb(i)=s/sw                                                            184
 90   continue                                                              185
      do 110 j=1,n                                                          185
      do 100 i=1,q                                                          185
      r(i,j)=r(i,j)-yb(i)                                                   185
 100  continue                                                              185
 110  continue                                                              186
      ys=0.                                                                 186
      do 130 i=1,q                                                          186
      s=0.                                                                  186
      do 120 j=1,n                                                          186
      s=s+w(j)*r(i,j)**2                                                    186
 120  continue                                                              186
      ys=ys+ww(i)*s/sw                                                      186
 130  continue                                                              187
      if (ys.gt.0.0) go to 140                                              187
      if (ifl.gt.0) write (ifl,440)                                         187
      return                                                                187
 140  ys=sqrt(ys)                                                           188
      s=1./ys                                                               188
      do 160 j=1,n                                                          188
      do 150 i=1,q                                                          188
      r(i,j)=r(i,j)*s                                                       188
 150  continue                                                              188
 160  continue                                                              189
      call subfit (m,p,q,n,w,sw,x,r,ww,lm,a,b,f,t,asr,sc,bt,g,dp)           190
      if (lf.le.0) go to 370                                                191
      if (ifl.gt.0) write (ifl,410) lm                                      192
      call fulfit (lm,lf,p,q,n,w,sw,x,r,ww,a,b,f,t,asr,sc,bt,g,dp)          193
 170  do 190 l=1,lm                                                         194
      sc(l,1)=l+.1                                                          194
      s=0.0                                                                 195
      do 180 i=1,q                                                          195
      s=s+ww(i)*abs(b(i,l))                                                 195
 180  continue                                                              195
      sc(l,2)=-s                                                            196
 190  continue                                                              197
      call sort (sc(1,2),sc,1,lm)                                           198
      if (ifl.le.0) go to 210                                               198
      write (ifl,390)                                                       199
      do 200 l=1,lm                                                         199
      k=sc(l,1)                                                             200
      s=abs(sc(l,2)/sc(1,2))                                                200
      write (ifl,400) l,k,s                                                 201
 200  continue                                                              202
 210  if (icl.ne.0) go to 240                                               203
      do 230 j=1,n                                                          203
      do 220 i=1,q                                                          203
      r(i,j)=y(q*(j-1)+i)                                                   203
 220  continue                                                              203
 230  continue                                                              203
      go to 270                                                             204
 240  do 260 j=1,n                                                          204
      do 250 i=1,q                                                          204
      r(i,j)=0.0                                                            204
 250  continue                                                              204
      k=y(j)+.1                                                             204
      r(k,j)=1.0                                                            204
 260  continue                                                              204
 270  if (ifl.gt.0.and.q.gt.1) write (ifl,420)                              206
      do 300 i=1,q                                                          206
      u=0.0                                                                 206
      v=u                                                                   207
      do 290 j=1,n                                                          207
      r(i,j)=r(i,j)-yb(i)                                                   207
      v=v+w(j)*r(i,j)**2                                                    208
      s=0.                                                                  208
      do 280 l=1,lm                                                         208
      s=s+b(i,l)*f(j,l)                                                     208
 280  continue                                                              209
      r(i,j)=r(i,j)/ys-s                                                    209
      u=u+w(j)*r(i,j)**2                                                    210
 290  continue                                                              211
      s=u*ys**2/v                                                           211
      if (ifl.gt.0.and.q.gt.1) write (ifl,430) i,s                          212
 300  continue                                                              213
      if (icl.eq.1.and.ifl.gt.0) call wrisk (lm,p,q,n,pi,fls,w,sw,x,y,ww    215
     1,yb,ys,a,b,f,sc(1,2))                                                 215
      if (lm.le.mu) go to 370                                               215
      l=sc(lm,1)                                                            216
      asr=0.0                                                               217
      do 320 j=1,n                                                          217
      do 310 i=1,q                                                          217
      r(i,j)=r(i,j)+b(i,l)*f(j,l)                                           218
      asr=asr+w(j)*ww(i)*r(i,j)**2                                          219
 310  continue                                                              219
 320  continue                                                              220
      asr=asr/sw                                                            221
      if (l.ge.lm) go to 360                                                222
      do 330 i=1,p                                                          222
      a(i,l)=a(i,lm)                                                        222
 330  continue                                                              222
      do 340 i=1,q                                                          222
      b(i,l)=b(i,lm)                                                        222
 340  continue                                                              223
      do 350 j=1,n                                                          223
      f(j,l)=f(j,lm)                                                        223
      t(j,l)=t(j,lm)                                                        223
 350  continue                                                              224
 360  lm=lm-1                                                               226
      if (ifl.gt.0) write (ifl,410) lm                                      227
      call fulfit (lm,lf,p,q,n,w,sw,x,r,ww,a,b,f,t,asr,sc,bt,g,dp)          228
      go to 170                                                             229
 370  if (ifl.gt.0) write (ifl,380)                                         231
      if (ifl.gt.0.and.p.gt.1) call import (m,lm,p,q,n,w,sw,x,ww,a,b,f,t    232
     1,sc,g,r)                                                              232
      call fsort (lm,n,f,t,r)                                               232
      flm=lm                                                                233
      return                                                                234
 380  format('0this is the smart solution.'/)                               235
 390  format('0relative term importance:'/'0 pos  term      importance')    236
 400  format(' 'i3,'  'i3,'       'g12.4)                                   237
 410  format('0'i2,' term solution:'/)                                      238
 420  format('0e**2 separately for each response:'/)                        239
 430  format(' e('i2,')**2 ='g12.4)                                         240
 440  format(' sorry, every response must not have zero variance.'/  ' u    243
     1nfortunately smart cannot continue with your problem.'/  ' please     243
     2check your input response values and try again.')                     243
      end                                                                   244
      subroutine subfit (m,p,q,n,w,sw,x,r,ww,lm,a,b,f,t,asr,sc,bt,g,dp)     245
      integer p,q                                                           246
      real w(n),x(p,n),ww(q),a(p,m),f(n,m),t(n,m),r(q,n)                    247
      real sc(n,15),g(p,3),b(q,m),bt(q)                                     248
      double precision dp(1)                                                249
      common /parms/ ifl,lf,span,alpha,big                                  250
      common /z00001/ maxit,conv,mitone,cutmin,fdel,cjeps,mitcj             251
      asr=big                                                               251
      lm=0                                                                  252
      do 50 l=1,m                                                           252
      lm=lm+1                                                               252
      asrold=asr                                                            252
      call newb (lm,q,ww,b)                                                 253
      call onetrm (0,p,q,n,w,sw,x,r,ww,a(1,lm),b(1,lm),f(1,lm),t(1,lm),a    255
     1sr,sc,g,dp)                                                           255
      do 20 j=1,n                                                           255
      do 10 i=1,q                                                           255
      r(i,j)=r(i,j)-b(i,lm)*f(j,lm)                                         255
 10   continue                                                              255
 20   continue                                                              256
      if (ifl.le.0.or.lf.gt.0) go to 30                                     256
      write (ifl,70) lm,asr                                                 257
      write (ifl,80) (a(i,lm),i=1,p)                                        258
      write (ifl,90) (b(i,lm),i=1,q)                                        259
 30   if (lm.eq.1) go to 50                                                 261
      if (lf.le.0) go to 40                                                 262
      if (lm.eq.m) go to 60                                                 262
      iflsv=ifl                                                             262
      ifl=0                                                                 263
      call fulfit (lm,1,p,q,n,w,sw,x,r,ww,a,b,f,t,asr,sc,bt,g,dp)           264
      ifl=iflsv                                                             265
 40   if (asr.le.0.0.or.(asrold-asr)/asrold.lt.conv) go to 60               266
 50   continue                                                              267
 60   return                                                                268
 70   format('0smart stage 'i2,' term:        e**2 ='g12.4)                 269
 80   format(' a ='5g12.4)                                                  269
 90   format(' b ='5g12.4)                                                  270
      end                                                                   271
      subroutine fulfit (lm,lbf,p,q,n,w,sw,x,r,ww,a,b,f,t,asri,sc,bt,g,d    272
     1p)                                                                    272
      integer p,q                                                           273
      real w(n),x(p,n),ww(q),a(p,lm),f(n,lm),t(n,lm),r(q,n)                 274
      real sc(n,15),g(p,3),b(q,lm),bt(q)                                    275
      double precision dp(1)                                                276
      common /parms/ ifl,lf,span,alpha,big                                  277
      common /z00001/ maxit,conv,mitone,cutmin,fdel,cjeps,mitcj             278
      if (lbf.le.0) return                                                  279
      iter=0                                                                280
      fsv=cutmin                                                            280
      isv=mitone                                                            281
      if (lbf.ge.3) go to 10                                                281
      cutmin=1.0                                                            281
      mitone=lbf-1                                                          281
 10   asrold=asri                                                           282
      iter=iter+1                                                           283
      do 130 lp=1,lm                                                        283
      do 20 i=1,q                                                           283
      bt(i)=b(i,lp)                                                         283
 20   continue                                                              283
      do 30 i=1,p                                                           283
      g(i,3)=a(i,lp)                                                        283
 30   continue                                                              284
      do 50 j=1,n                                                           284
      do 40 i=1,q                                                           284
      r(i,j)=r(i,j)+bt(i)*f(j,lp)                                           284
 40   continue                                                              284
 50   continue                                                              285
      call onetrm (1,p,q,n,w,sw,x,r,ww,g(1,3),bt,sc(1,14),sc(1,15),asri,    287
     1sc,g,dp)                                                              287
      if (asri.ge.asrold) go to 90                                          288
      do 60 i=1,q                                                           288
      b(i,lp)=bt(i)                                                         288
 60   continue                                                              288
      do 70 i=1,p                                                           288
      a(i,lp)=g(i,3)                                                        288
 70   continue                                                              289
      do 80 j=1,n                                                           289
      f(j,lp)=sc(j,14)                                                      289
      t(j,lp)=sc(j,15)                                                      289
 80   continue                                                              290
      go to 100                                                             291
 90   asri=asrold                                                           291
 100  do 120 j=1,n                                                          292
      do 110 i=1,q                                                          292
      r(i,j)=r(i,j)-b(i,lp)*f(j,lp)                                         292
 110  continue                                                              292
 120  continue                                                              293
 130  continue                                                              294
      if (iter.gt.maxit.or.asri.le.0.0.or.(asrold-asri)/asrold.lt.conv)     294
     1go to 140                                                             294
      go to 10                                                              295
 140  cutmin=fsv                                                            295
      mitone=isv                                                            296
      if (ifl.le.0) go to 160                                               296
      write (ifl,190) asri                                                  297
      do 150 lp=1,lm                                                        297
      write (ifl,200) lp                                                    298
      write (ifl,170) (a(i,lp),i=1,p)                                       299
      write (ifl,180) (b(i,lp),i=1,q)                                       300
 150  continue                                                              301
 160  return                                                                303
 170  format(' a ='5g12.4)                                                  303
 180  format(' b ='5g12.4)                                                  304
 190  format(' e**2 ='g12.4/)                                               305
 200  format(' term'i2,':')                                                 306
      end                                                                   307
      subroutine onetrm (jfl,p,q,n,w,sw,x,y,ww,a,b,f,t,asr,sc,g,dp)         308
      integer p,q                                                           309
      real w(n),x(p,n),y(q,n),ww(q),a(p),b(q),f(n),t(n),sc(n,13),g(p,2)     310
      double precision dp(1)                                                311
      common /parms/ ifl,lf,span,alpha,big                                  312
      common /z00001/ maxit,conv,mitone,cutmin,fdel,cjeps,mitcj             313
      iter=0                                                                313
      asr=big                                                               314
 10   iter=iter+1                                                           314
      asrold=asr                                                            315
      do 30 j=1,n                                                           315
      s=0.0                                                                 315
      do 20 i=1,q                                                           315
      s=s+ww(i)*b(i)*y(i,j)                                                 315
 20   continue                                                              315
      sc(j,13)=s                                                            315
 30   continue                                                              316
      call oneone (max0(jfl,iter-1),p,n,w,sw,sc(1,13),x,a,f,t,asr,sc,g,d    317
     1p)                                                                    317
      do 50 i=1,q                                                           317
      s=0.0                                                                 317
      do 40 j=1,n                                                           317
      s=s+w(j)*y(i,j)*f(j)                                                  317
 40   continue                                                              317
      b(i)=s/sw                                                             317
 50   continue                                                              318
      asr=0.0                                                               319
      do 70 i=1,q                                                           319
      s=0.0                                                                 319
      do 60 j=1,n                                                           319
      s=s+w(j)*(y(i,j)-b(i)*f(j))**2                                        319
 60   continue                                                              320
      asr=asr+ww(i)*s/sw                                                    321
 70   continue                                                              324
      if (q.eq.1.or.iter.gt.maxit.or.asr.le.0.0.or.(asrold-asr)/asrold.l    325
     1t.conv) go to 80                                                      325
      go to 10                                                              326
 80   return                                                                327
      end                                                                   328
      subroutine oneone (ist,p,n,w,sw,y,x,a,f,t,asr,sc,g,dp)                329
      integer p                                                             330
      real w(n),y(n),x(p,n),a(p),f(n),t(n),sc(n,12),g(p,2)                  331
      double precision dp(1)                                                332
      common /parms/ ifl,lf,span,alpha,big                                  333
      common /z00001/ maxit,conv,mitone,cutmin,fdel,cjeps,mitcj             334
      sml=1.0/big                                                           335
      if (ist.gt.0) go to 30                                                336
      if (p.gt.1) go to 10                                                  336
      a(1)=1.0                                                              336
      go to 30                                                              337
 10   do 20 j=1,n                                                           337
      sc(j,2)=1.0                                                           337
 20   continue                                                              338
      call dir (p,n,w,sw,y,x,sc(1,2),a,dp)                                  339
 30   s=0.0                                                                 341
      do 40 i=1,p                                                           341
      g(i,1)=0.0                                                            341
      s=s+a(i)**2                                                           341
 40   continue                                                              342
      s=1.0/sqrt(s)                                                         342
      do 50 i=1,p                                                           342
      a(i)=a(i)*s                                                           342
 50   continue                                                              343
      iter=0                                                                343
      asr=big                                                               343
      cut=1.0                                                               344
 60   iter=iter+1                                                           344
      asrold=asr                                                            345
 70   s=0.0                                                                 346
      do 80 i=1,p                                                           346
      g(i,2)=a(i)+g(i,1)                                                    346
      s=s+g(i,2)**2                                                         346
 80   continue                                                              347
      s=1./sqrt(s)                                                          347
      do 90 i=1,p                                                           347
      g(i,2)=g(i,2)*s                                                       347
 90   continue                                                              348
      do 110 j=1,n                                                          348
      sc(j,1)=j+.1                                                          348
      s=0.                                                                  349
      do 100 i=1,p                                                          349
      s=s+g(i,2)*x(i,j)                                                     349
 100  continue                                                              349
      sc(j,11)=s                                                            350
 110  continue                                                              351
      call sort (sc(1,11),sc,1,n)                                           352
      do 120 j=1,n                                                          352
      k=sc(j,1)                                                             352
      sc(j,2)=y(k)                                                          352
      sc(j,3)=amax1(w(k),sml)                                               352
 120  continue                                                              353
      call supsmu (n,sc(1,11),sc(1,2),sc(1,3),1,span,alpha,sc(1,12),sc(1    355
     1,4))                                                                  355
      s=0.0                                                                 355
      do 130 j=1,n                                                          355
      s=s+sc(j,3)*(sc(j,2)-sc(j,12))**2                                     355
 130  continue                                                              355
      s=s/sw                                                                358
      if (s.lt.asr) go to 150                                               358
      cut=cut*0.5                                                           358
      if (cut.lt.cutmin) go to 190                                          359
      do 140 i=1,p                                                          359
      g(i,1)=g(i,1)*cut                                                     359
 140  continue                                                              359
      go to 70                                                              360
 150  asr=s                                                                 361
      cut=1.0                                                               361
      do 160 i=1,p                                                          361
      a(i)=g(i,2)                                                           361
 160  continue                                                              362
      do 170 j=1,n                                                          362
      k=sc(j,1)                                                             362
      t(k)=sc(j,11)                                                         362
      f(k)=sc(j,12)                                                         362
 170  continue                                                              363
      if (asr.le.0.0.or.(asrold-asr)/asrold.lt.conv) go to 190              364
      if (iter.gt.mitone.or.p.le.1) go to 190                               365
      call der (n,sc(1,11),sc(1,12),sc(1,3),fdel,sc(1,4),sc(1,5))           366
      do 180 j=1,n                                                          366
      k=sc(j,1)                                                             366
      sc(j,5)=y(j)-f(j)                                                     366
      sc(k,6)=sc(j,4)                                                       366
 180  continue                                                              367
      call dir (p,n,w,sw,sc(1,5),x,sc(1,6),g,dp)                            368
      go to 60                                                              369
 190  s=0.0                                                                 369
      v=s                                                                   369
      do 200 j=1,n                                                          369
      s=s+w(j)*f(j)                                                         369
 200  continue                                                              370
      s=s/sw                                                                370
      do 210 j=1,n                                                          370
      f(j)=f(j)-s                                                           370
      v=v+w(j)*f(j)**2                                                      370
 210  continue                                                              371
      if (v.le.0.0) go to 230                                               371
      v=1.0/sqrt(v/sw)                                                      371
      do 220 j=1,n                                                          371
      f(j)=f(j)*v                                                           371
 220  continue                                                              371
 230  return                                                                373
      end                                                                   374
      subroutine dir (p,n,w,sw,r,x,d,e,g)                                   375
      integer p                                                             375
      real w(n),r(n),x(p,n),d(n),e(p)                                       376
      double precision s,g(1)                                               377
      common /z00001/ maxit,conv,mitone,cutmin,fdel,cjeps,mitcj             378
      do 20 i=1,p                                                           378
      s=0.0                                                                 378
      do 10 j=1,n                                                           378
      s=s+w(j)*d(j)*x(i,j)                                                  378
 10   continue                                                              378
      e(i)=s/sw                                                             378
 20   continue                                                              379
      k=0                                                                   379
      m1=p*(p+1)/2                                                          379
      m2=m1+p                                                               380
      do 60 j=1,p                                                           380
      s=0.0                                                                 380
      do 30 l=1,n                                                           380
      s=s+w(l)*r(l)*(d(l)*x(j,l)-e(j))                                      380
 30   continue                                                              380
      g(m1+j)=s/sw                                                          381
      do 50 i=1,j                                                           381
      s=0.0                                                                 382
      do 40 l=1,n                                                           382
      s=s+w(l)*(d(l)*x(i,l)-e(i))*(d(l)*x(j,l)-e(j))                        382
 40   continue                                                              383
      k=k+1                                                                 383
      g(k)=s/sw                                                             384
 50   continue                                                              385
 60   continue                                                              386
      call conjg1 (p,g,g(m1+1),g(m2+1),cjeps,mitcj,g(m2+p+1))               387
      do 70 i=1,p                                                           387
      e(i)=g(m2+i)                                                          387
 70   continue                                                              388
      return                                                                389
      end                                                                   390
      subroutine conjg1 (p,g,c,x,eps,maxit,sc)                              391
      integer p                                                             391
      double precision g(1),c(p),x(p),sc(p,4)                               392
      double precision beta,h,s,alpha,t                                     394
      do 10 i=1,p                                                           394
      x(i)=0.0                                                              394
      sc(i,2)=0.0                                                           394
 10   continue                                                              394
      nit=0                                                                 395
 20   nit=nit+1                                                             395
      h=0.0                                                                 395
      beta=0.0                                                              396
      do 90 i=1,p                                                           396
      sc(i,4)=x(i)                                                          396
      s=g(i*(i-1)/2+i)*x(i)                                                 396
      im1=i-1                                                               397
      j=1                                                                   397
      go to 40                                                              397
 30   j=j+1                                                                 397
 40   if ((j).gt.(im1)) go to 50                                            397
      s=s+g(i*(i-1)/2+j)*x(j)                                               397
      go to 30                                                              398
 50   j=i+1                                                                 398
      go to 70                                                              398
 60   j=j+1                                                                 398
 70   if ((j).gt.(p)) go to 80                                              398
      s=s+g(j*(j-1)/2+i)*x(j)                                               398
      go to 60                                                              399
 80   sc(i,1)=s-c(i)                                                        399
      h=h+sc(i,1)**2                                                        400
 90   continue                                                              401
      if (h.le.0.0) go to 220                                               402
      do 190 iter=1,p                                                       402
      do 100 i=1,p                                                          402
      sc(i,2)=beta*sc(i,2)-sc(i,1)                                          402
 100  continue                                                              402
      t=0.0                                                                 403
      do 170 i=1,p                                                          403
      s=g(i*(i-1)/2+i)*sc(i,2)                                              403
      im1=i-1                                                               404
      j=1                                                                   404
      go to 120                                                             404
 110  j=j+1                                                                 404
 120  if ((j).gt.(im1)) go to 130                                           404
      s=s+g(i*(i-1)/2+j)*sc(j,2)                                            404
      go to 110                                                             405
 130  j=i+1                                                                 405
      go to 150                                                             405
 140  j=j+1                                                                 405
 150  if ((j).gt.(p)) go to 160                                             405
      s=s+g(j*(j-1)/2+i)*sc(j,2)                                            405
      go to 140                                                             406
 160  sc(i,3)=s                                                             406
      t=t+s*sc(i,2)                                                         407
 170  continue                                                              408
      alpha=h/t                                                             408
      s=0.0                                                                 409
      do 180 i=1,p                                                          409
      x(i)=x(i)+alpha*sc(i,2)                                               410
      sc(i,1)=sc(i,1)+alpha*sc(i,3)                                         410
      s=s+sc(i,1)**2                                                        411
 180  continue                                                              412
      if (s.le.0.0) go to 200                                               412
      beta=s/h                                                              412
      h=s                                                                   413
 190  continue                                                              414
 200  s=0.0                                                                 414
      do 210 i=1,p                                                          414
      s=dmax1(s,dabs(x(i)-sc(i,4)))                                         414
 210  continue                                                              415
      if (s.lt.eps.or.nit.ge.maxit) go to 220                               415
      go to 20                                                              417
 220  return                                                                418
      end                                                                   419
      subroutine der (n,x,s,w,fdel,d,sc)                                    420
      real x(n),s(n),w(n),d(n),sc(n,3)                                      421
      integer bl,el,bc,ec,br,er                                             423
      if (x(n).gt.x(1)) go to 20                                            423
      do 10 j=1,n                                                           423
      d(j)=0.0                                                              423
 10   continue                                                              423
      return                                                                423
 20   i=n/4                                                                 424
      j=3*i                                                                 424
      scale=x(j)-x(i)                                                       425
 30   if (scale.gt.0.0) go to 40                                            425
      if (j.lt.n) j=j+1                                                     425
      if (i.gt.1) i=i-1                                                     426
      scale=x(j)-x(i)                                                       427
      go to 30                                                              428
 40   del=fdel*scale*2.0                                                    430
      do 50 j=1,n                                                           430
      sc(j,1)=x(j)                                                          430
      sc(j,2)=s(j)                                                          430
      sc(j,3)=w(j)                                                          430
 50   continue                                                              431
      call pool (n,sc,sc(1,2),sc(1,3),del)                                  433
      bc=0                                                                  433
      br=bc                                                                 433
      er=br                                                                 434
 60   br=er+1                                                               434
      er=br                                                                 435
 70   if (er.ge.n) go to 80                                                 435
      if (sc(br,1).ne.sc(er+1,1)) go to 80                                  435
      er=er+1                                                               435
      go to 70                                                              435
 80   if (br.ne.1) go to 90                                                 436
      bl=br                                                                 436
      el=er                                                                 436
      go to 60                                                              436
 90   if (bc.ne.0) go to 110                                                437
      bc=br                                                                 437
      ec=er                                                                 438
      do 100 j=bl,el                                                        438
      d(j)=(sc(bc,2)-sc(bl,2))/(sc(bc,1)-sc(bl,1))                          438
 100  continue                                                              439
      go to 60                                                              440
 110  do 120 j=bc,ec                                                        441
      d(j)=(sc(br,2)-sc(bl,2))/(sc(br,1)-sc(bl,1))                          441
 120  continue                                                              442
      if (er.ne.n) go to 140                                                443
      do 130 j=br,er                                                        443
      d(j)=(sc(br,2)-sc(bc,2))/(sc(br,1)-sc(bc,1))                          443
 130  continue                                                              444
      go to 150                                                             445
 140  bl=bc                                                                 446
      el=ec                                                                 446
      bc=br                                                                 446
      ec=er                                                                 447
      go to 60                                                              448
 150  return                                                                449
      end                                                                   450
      subroutine pool (n,x,y,w,del)                                         455
      real x(n),y(n),w(n)                                                   456
      integer bb,eb,br,er,bl,el                                             457
      bb=0                                                                  457
      eb=bb                                                                 458
 10   if (eb.ge.n) go to 110                                                458
      bb=eb+1                                                               458
      eb=bb                                                                 459
 20   if (eb.ge.n) go to 30                                                 459
      if (x(bb).ne.x(eb+1)) go to 30                                        459
      eb=eb+1                                                               459
      go to 20                                                              459
 30   if (eb.ge.n) go to 70                                                 461
      if (x(eb+1)-x(eb).ge.del) go to 70                                    461
      br=eb+1                                                               461
      er=br                                                                 462
 40   if (er.ge.n) go to 50                                                 462
      if (x(er+1).ne.x(br)) go to 50                                        463
      er=er+1                                                               463
      go to 40                                                              463
 50   if (x(er+1)-x(er).lt.x(eb+1)-x(eb)) go to 10                          466
      eb=er                                                                 466
      pw=w(bb)+w(eb)                                                        466
      px=(x(bb)*w(bb)+x(eb)*w(eb))/pw                                       466
      py=(y(bb)*w(bb)+y(eb)*w(eb))/pw                                       466
      do 60 i=bb,eb                                                         466
      x(i)=px                                                               466
      y(i)=py                                                               466
      w(i)=pw                                                               466
 60   continue                                                              467
 70   if (bb.le.1) go to 10                                                 471
      if (x(bb)-x(bb-1).ge.del) go to 10                                    472
      bl=bb-1                                                               472
      el=bl                                                                 473
 80   if (bl.le.1) go to 90                                                 473
      if (x(bl-1).ne.x(el)) go to 90                                        473
      bl=bl-1                                                               473
      go to 80                                                              473
 90   bb=bl                                                                 474
      pw=w(bb)+w(eb)                                                        474
      px=(x(bb)*w(bb)+x(eb)*w(eb))/pw                                       474
      py=(y(bb)*w(bb)+y(eb)*w(eb))/pw                                       474
      do 100 i=bb,eb                                                        474
      x(i)=px                                                               474
      y(i)=py                                                               474
      w(i)=pw                                                               474
 100  continue                                                              475
      go to 70                                                              476
 110  return                                                                478
      end                                                                   479
      subroutine newb (lm,q,ww,b)                                           480
      integer q                                                             480
      real ww(q),b(q,lm)                                                    481
      common /parms/ ifl,lf,span,alpha,big                                  482
      sml=1.0/big                                                           483
      if (q.ne.1) go to 10                                                  483
      b(1,lm)=1.0                                                           483
      return                                                                483
 10   if (lm.ne.1) go to 30                                                 484
      do 20 i=1,q                                                           484
      b(i,lm)=i                                                             484
 20   continue                                                              484
      return                                                                484
 30   lm1=lm-1                                                              485
      do 40 i=1,q                                                           485
      b(i,lm)=0.0                                                           485
 40   continue                                                              485
      t=0.0                                                                 486
      do 60 i=1,q                                                           486
      s=0.0                                                                 486
      do 50 l=1,lm1                                                         486
      s=s+abs(b(i,l))                                                       486
 50   continue                                                              486
      b(i,lm)=s                                                             486
      t=t+s                                                                 486
 60   continue                                                              487
      do 70 i=1,q                                                           487
      b(i,lm)=ww(i)*(t-b(i,lm))                                             487
 70   continue                                                              488
      l1=1                                                                  488
      if (lm.gt.q) l1=lm-q+1                                                489
      do 100 l=l1,lm1                                                       489
      s=0.0                                                                 489
      t=s                                                                   490
      do 80 i=1,q                                                           490
      s=s+ww(i)*b(i,lm)*b(i,l)                                              490
      t=t+ww(i)*b(i,l)**2                                                   490
 80   continue                                                              491
      s=s/sqrt(t)                                                           491
      do 90 i=1,q                                                           491
      b(i,lm)=b(i,lm)-s*b(i,l)                                              491
 90   continue                                                              492
 100  continue                                                              493
      do 110 i=2,q                                                          493
      if (abs(b(i-1,lm)-b(i,lm)).gt.sml) return                             493
 110  continue                                                              494
      do 120 i=1,q                                                          494
      b(i,lm)=i                                                             494
 120  continue                                                              495
      return                                                                496
      end                                                                   497
      subroutine import (m,mu,p,q,n,w,sw,x,ww,a,b,f,t,sc,h,r)               498
      integer p,q                                                           498
      real sc(n,8),h(p,2),r(q,n)                                            499
      real w(n),x(p,n),ww(q),a(p,m),b(q,m),f(n,m),t(n,m)                    500
      common /parms/ ifl,lf,span,alpha,big                                  501
      common /z00001/ maxit,conv,mitone,cutmin,fdel,cjeps,mitcj             502
      sml=1.0/big                                                           503
      do 110 i=1,p                                                          503
      s=0.0                                                                 503
      v=s                                                                   503
      do 10 j=1,n                                                           503
      s=s+w(j)*x(i,j)                                                       503
 10   continue                                                              503
      s=s/sw                                                                504
      do 30 j=1,n                                                           504
      v=v+w(j)*(x(i,j)-s)**2                                                504
      do 20 k=1,q                                                           504
      r(k,j)=0.0                                                            504
 20   continue                                                              504
 30   continue                                                              505
      do 80 l=1,mu                                                          505
      do 40 j=1,n                                                           505
      sc(j,2)=t(j,l)                                                        505
      sc(j,1)=j+.1                                                          505
 40   continue                                                              506
      call sort (sc(1,2),sc,1,n)                                            507
      do 50 j=1,n                                                           507
      k=sc(j,1)                                                             507
      sc(j,3)=f(k,l)                                                        507
      sc(j,4)=amax1(w(k),sml)                                               507
 50   continue                                                              508
      call der (n,sc(1,2),sc(1,3),sc(1,4),fdel,sc(1,5),sc(1,6))             509
      do 70 j=1,n                                                           509
      do 60 k=1,q                                                           509
      r(k,j)=r(k,j)+b(k,l)*a(i,l)*sc(j,5)                                   509
 60   continue                                                              509
 70   continue                                                              510
 80   continue                                                              511
      s=0.0                                                                 511
      do 100 j=1,n                                                          511
      do 90 k=1,q                                                           511
      s=s+w(j)*ww(k)*abs(r(k,j))                                            511
 90   continue                                                              511
 100  continue                                                              512
      h(i,1)=-s*sqrt(v)                                                     512
      h(i,2)=i+.1                                                           513
 110  continue                                                              514
      call sort (h,h(1,2),1,p)                                              515
      write (ifl,130)                                                       516
      do 120 i=1,p                                                          516
      k=h(i,2)                                                              516
      s=abs(h(i,1)/h(1,1))                                                  517
      write (ifl,140) i,k,s                                                 518
 120  continue                                                              519
      return                                                                520
 130  format('0relative predictor variable importance:'/  '0 pos  varbl     522
     1   importance')                                                       522
 140  format(' 'i3,'   'i3,'     'g12.4)                                    523
      end                                                                   524
      function gtfun (iterm,smod,f,t)                                       525
      real f(1),t(1),smod(1)                                                526
      gtfun=1.0                                                             527
      if ((iterm.ge.1).and.(float(iterm).le.smod(5))) go to 10              527
      gtfun=0.0                                                             527
      return                                                                527
 10   n=smod(4)+.1                                                          529
      jf=smod(3)+6.0+smod(1)*(smod(2)+smod(3))+n*(iterm-1)+.1               530
      jt=jf+smod(1)*n+.1                                                    531
      do 20 j=1,n                                                           531
      f(j)=smod(jf+j)                                                       531
      t(j)=smod(jt+j)                                                       531
 20   continue                                                              532
      return                                                                533
      end                                                                   534
      function gtprms (iterm,smod,a,b)                                      535
      integer p,q                                                           535
      real a(1),b(1),smod(1)                                                536
      gtprms=1.0                                                            537
      if ((iterm.ge.1).and.(float(iterm).le.smod(5))) go to 10              537
      gtprms=0.0                                                            537
      return                                                                537
 10   p=smod(2)+.1                                                          538
      q=smod(3)+.1                                                          538
      m=smod(1)+.1                                                          539
      ja=q+6+p*(iterm-1)                                                    539
      jb=q+6+p*m+q*(iterm-1)                                                540
      do 20 i=1,p                                                           540
      a(i)=smod(ja+i)                                                       540
 20   continue                                                              540
      do 30 i=1,q                                                           540
      b(i)=smod(jb+i)                                                       540
 30   continue                                                              541
      return                                                                542
      end                                                                   543
      function gof (smod,mu)                                                544
      real smod(1)                                                          545
      lgof=smod(3)+7.0+smod(1)*(smod(2)+smod(3)+2.0*smod(4))+.1             546
      gof=smod(lgof)                                                        546
      mu=smod(5)+.1                                                         547
      return                                                                548
      end                                                                   549
      subroutine fsort (mu,n,f,t,sp)                                        550
      real f(n,mu),t(n,mu),sp(n,2)                                          551
      do 30 l=1,mu                                                          552
      do 10 j=1,n                                                           552
      sp(j,1)=j+.1                                                          552
      sp(j,2)=f(j,l)                                                        552
 10   continue                                                              553
      call sort (t(1,l),sp,1,n)                                             554
      do 20 j=1,n                                                           554
      k=sp(j,1)                                                             554
      f(j,l)=sp(k,2)                                                        554
 20   continue                                                              555
 30   continue                                                              556
      return                                                                557
      end                                                                   558
      subroutine yhat (x,smod,y)                                            559
      integer p,q                                                           559
      real x(1),y(1),smod(1)                                                560
      integer place,low,high                                                561
      m=smod(1)+.1                                                          561
      p=smod(2)+.1                                                          561
      q=smod(3)+.1                                                          561
      n=smod(4)+.1                                                          562
      mu=smod(5)+.1                                                         562
      ys=smod(q+6)                                                          563
      ja=q+6                                                                563
      jb=ja+p*m                                                             563
      jf=jb+m*q                                                             563
      jt=jf+n*m                                                             564
      do 10 i=1,q                                                           564
      y(i)=0.0                                                              564
 10   continue                                                              565
      do 110 l=1,mu                                                         565
      s=0.0                                                                 565
      do 20 j=1,p                                                           565
      s=s+smod(ja+j)*x(j)                                                   565
 20   continue                                                              566
      if (s.gt.smod(jt+1)) go to 30                                         566
      place=1                                                               566
      go to 80                                                              566
 30   if (s.lt.smod(jt+n)) go to 40                                         567
      place=n                                                               567
      go to 80                                                              567
 40   low=0                                                                 568
      high=n+1                                                              569
 50   if (low+1.ge.high) go to 70                                           569
      place=(low+high)/2                                                    570
      t=smod(jt+place)                                                      570
      if (s.eq.t) go to 80                                                  571
      if (s.ge.t) go to 60                                                  571
      high=place                                                            571
      go to 50                                                              571
 60   low=place                                                             571
      go to 50                                                              573
 70   jfl=jf+low                                                            573
      jfh=jf+high                                                           573
      jtl=jt+low                                                            573
      jth=jt+high                                                           574
      t=smod(jfl)+(smod(jfh)-smod(jfl))*(s-smod(jtl))/(smod(jth)-smod(jt    576
     1l))                                                                   576
      go to 90                                                              576
 80   t=smod(jf+place)                                                      577
 90   do 100 i=1,q                                                          577
      y(i)=y(i)+smod(jb+i)*t                                                577
 100  continue                                                              578
      ja=ja+p                                                               578
      jb=jb+q                                                               578
      jf=jf+n                                                               578
      jt=jt+n                                                               579
 110  continue                                                              580
      do 120 i=1,q                                                          580
      y(i)=ys*y(i)+smod(i+5)                                                580
 120  continue                                                              581
      return                                                                582
      end                                                                   583
      subroutine clsfy (pi,fls,y,smod,icl,rsk)                              584
      integer q                                                             584
      real pi(1),fls(1),y(1),smod(1)                                        585
      common /parms/ ifl,lf,span,alpha,big                                  586
      q=smod(3)+.1                                                          587
      if (pi(1).ge.big) go to 20                                            587
      jmod=smod(1)*(smod(2)+q+2.0*smod(4))+q+7.1                            588
      do 10 i=1,q                                                           588
      y(i)=smod(jmod+i)*y(i)                                                588
 10   continue                                                              589
 20   ym=big                                                                590
      do 30 i=1,q                                                           590
      ym=amin1(ym,y(i))                                                     590
 30   continue                                                              591
      if (ym.ge.0.0) go to 50                                               591
      do 40 i=1,q                                                           591
      y(i)=y(i)-ym                                                          591
 40   continue                                                              591
 50   s=0.0                                                                 592
      do 60 i=1,q                                                           592
      s=s+y(i)                                                              592
 60   continue                                                              593
      if (s.gt.0.0) go to 70                                                593
      icl=1                                                                 593
      rsk=big                                                               593
      return                                                                593
 70   s=1./s                                                                593
      do 80 i=1,q                                                           593
      y(i)=y(i)*s                                                           593
 80   continue                                                              594
      if (fls(1).lt.big) go to 100                                          595
      s=0.0                                                                 595
      do 90 i=1,q                                                           595
      if (y(i).le.s) go to 90                                               595
      s=y(i)                                                                595
      icl=i                                                                 595
 90   continue                                                              596
      rsk=1.0-s                                                             596
      return                                                                597
 100  rsk=big                                                               599
      do 120 i=1,q                                                          599
      s=0.0                                                                 599
      do 110 j=1,q                                                          599
      s=s+y(j)*fls(q*(i-1)+j)                                               599
 110  continue                                                              600
      if (s.ge.rsk) go to 120                                               600
      rsk=s                                                                 600
      icl=i                                                                 601
 120  continue                                                              602
      return                                                                603
      end                                                                   604
      subroutine wrisk (lm,p,q,n,pi,fls,w,sw,x,c,ww,yb,ys,a,b,f,y)          605
      integer p,q                                                           605
      real pi(q),fls(q,q),c(n),a(p,lm),b(q,lm),f(n,lm)                      606
      real w(n),x(p,n),ww(q),yb(q),y(q,2)                                   607
      common /parms/ ifl,lf,span,alpha,big                                  608
      risk=0.0                                                              608
      erisk=risk                                                            609
      if (fls(1,1).ge.big) go to 30                                         610
      do 20 i=1,q                                                           610
      s=0.0                                                                 610
      do 10 k=1,q                                                           610
      s=s+fls(i,k)                                                          610
 10   continue                                                              610
      y(i,2)=ww(i)/s                                                        610
 20   continue                                                              611
      go to 50                                                              612
 30   do 40 i=1,q                                                           612
      y(i,2)=ww(i)                                                          612
 40   continue                                                              612
 50   do 210 j=1,n                                                          614
      do 70 i=1,q                                                           614
      s=0.0                                                                 614
      do 60 l=1,lm                                                          614
      s=s+b(i,l)*f(j,l)                                                     614
 60   continue                                                              614
      y(i,1)=ys*s+yb(i)                                                     614
 70   continue                                                              615
      if (pi(1).ge.big) go to 90                                            615
      do 80 i=1,q                                                           615
      y(i,1)=y(i,2)*y(i,1)                                                  615
 80   continue                                                              615
 90   ym=big                                                                616
      do 100 i=1,q                                                          616
      ym=amin1(ym,y(i,1))                                                   616
 100  continue                                                              617
      if (ym.ge.0.0) go to 120                                              617
      do 110 i=1,q                                                          617
      y(i,1)=y(i,1)-ym                                                      617
 110  continue                                                              617
 120  s=0.0                                                                 618
      do 130 i=1,q                                                          618
      s=s+y(i,1)                                                            618
 130  continue                                                              619
      if (s.gt.0.0) go to 140                                               619
      risk=big                                                              619
      erisk=risk                                                            619
      go to 220                                                             619
 140  s=1./s                                                                620
      do 150 i=1,q                                                          620
      y(i,1)=y(i,1)*s                                                       620
 150  continue                                                              621
      if (fls(1,1).lt.big) go to 170                                        622
      s=0.0                                                                 622
      do 160 i=1,q                                                          622
      if (y(i,1).le.s) go to 160                                            622
      s=y(i,1)                                                              622
      icl=i                                                                 622
 160  continue                                                              623
      rsk=1.0-s                                                             623
      go to 200                                                             624
 170  rsk=big                                                               626
      do 190 i=1,q                                                          626
      s=0.0                                                                 626
      do 180 k=1,q                                                          626
      s=s+y(k,1)*fls(k,i)                                                   626
 180  continue                                                              627
      if (s.ge.rsk) go to 190                                               627
      rsk=s                                                                 627
      icl=i                                                                 628
 190  continue                                                              629
 200  ic=c(j)+.1                                                            629
      wt=w(j)                                                               629
      if (pi(1).lt.big) wt=y(ic,2)*wt                                       630
      erisk=erisk+wt*rsk                                                    631
      if (icl.eq.ic) go to 210                                              631
      if (fls(1,1).lt.big) wt=wt*fls(ic,icl)                                631
      risk=risk+wt                                                          631
 210  continue                                                              633
      risk=risk/sw                                                          633
      erisk=erisk/sw                                                        634
 220  write (ifl,230) risk,erisk                                            635
      return                                                                636
 230  format('0misclassification risk ='g12.4/  ' calculated from probab    638
     1ility estimates ='g12.4)                                              638
      end                                                                   639
      subroutine chkmem (nsmod,m1,nsp,m2,ndp,m3,ierr)                       640
      common /parms/ ifl,lf,span,alpha,big                                  641
      ierr=0                                                                642
      if (nsmod.ge.m1) go to 10                                             642
      ierr=1                                                                642
      if (ifl.gt.0) write (ifl,50) m1                                       642
 10   if (nsp.ge.m2) go to 20                                               643
      ierr=1                                                                643
      if (ifl.gt.0) write (ifl,60) m2                                       643
 20   if (ndp.ge.m3) go to 30                                               644
      ierr=1                                                                644
      if (ifl.gt.0) write (ifl,70) m3                                       644
 30   if (ierr.eq.0) go to 40                                               645
      if (ifl.gt.0) write (ifl,80)                                          645
 40   return                                                                647
 50   format(' sorry, smod array must be dimensioned at least'i5,'.'/)      648
 60   format(' sorry, sp array must be dimensioned at least'i5,'.'/)        649
 70   format(' sorry, dp array must be dimensioned at least'i5,'.'/)        650
 80   format(' unfortunately smart cannot continue with your problem.'/     652
     1 ' please check your workspace dimensions and try again.')            652
      end                                                                   653
      subroutine diagr (icl,m,mu,p,q,n,w,ww,ierr)                           654
      integer p,q                                                           654
      real w(n),ww(q)                                                       655
      common /parms/ ifl,lf,span,alpha,big                                  656
      ierr=0                                                                657
      if (m.ge.1) go to 10                                                  657
      ierr=1                                                                657
      if (ifl.gt.0) write (ifl,150) m                                       657
 10   if ((mu.ge.1).and.(mu.le.m)) go to 20                                 658
      ierr=1                                                                658
      if (ifl.gt.0) write (ifl,160) mu,m                                    658
 20   if (p.ge.1) go to 30                                                  659
      ierr=1                                                                659
      if (ifl.gt.0) write (ifl,170) p                                       659
 30   if (q.ge.1) go to 40                                                  660
      ierr=1                                                                660
      if (ifl.gt.0) write (ifl,180) q                                       660
 40   if (n.ge.1) go to 50                                                  661
      ierr=1                                                                661
      if (ifl.gt.0) write (ifl,190) n                                       661
 50   if (span.ge.0.0) go to 60                                             662
      ierr=1                                                                662
      if (ifl.gt.0) write (ifl,210) span                                    662
 60   if ((alpha.ge.0.0).and.(alpha.le.10.0)) go to 70                      664
      ierr=1                                                                664
      if (ifl.gt.0) write (ifl,200) alpha                                   665
 70   if ((lf.ge.0).and.(lf.le.3)) go to 80                                 666
      ierr=1                                                                666
      if (ifl.gt.0) write (ifl,220) lf                                      666
 80   if (icl.ne.0.or.q.lt.1) go to 110                                     667
      s=0.0                                                                 668
      do 100 i=1,q                                                          669
      if (ww(i).ge.0.0) go to 90                                            669
      ierr=1                                                                669
      if (ifl.gt.0) write (ifl,230) i,ww(i)                                 669
 90   s=s+ww(i)                                                             671
 100  continue                                                              672
      if (s.gt.0.0) go to 110                                               672
      ierr=1                                                                672
      if (ifl.gt.0) write (ifl,240)                                         672
 110  if (n.lt.1) go to 140                                                 674
      s=0.0                                                                 675
      do 130 j=1,n                                                          676
      if (w(j).ge.0.0) go to 120                                            676
      ierr=1                                                                676
      if (ifl.gt.0) write (ifl,250) j,w(j)                                  676
 120  s=s+w(j)                                                              678
 130  continue                                                              679
      if (s.gt.0.0) go to 140                                               679
      ierr=1                                                                679
      if (ifl.gt.0) write (ifl,260)                                         679
 140  if (ierr.ne.0.and.ifl.gt.0) write (ifl,270)                           682
      return                                                                683
 150  format(' sorry, maximum number of terms m ='g12.4,  ' must be one     685
     1or greater.')                                                         685
 160  format(' sorry, specified number of terms for model mu ='g12.4/  '    687
     1 must be greater than 0 and less than or equal to m (='g12.4,')')     687
 170  format(' sorry, number of predictor variables p ='g12.4,  ' must b    689
     1e one or greater.')                                                   689
 180  format(' sorry, number of response variables q ='g12.4,  ' must be    691
     1 one or greater.')                                                    691
 190  format(' sorry, number of observations n ='g12.4,  ' must be one o    693
     1r greater.')                                                          693
 200  format(' sorry, smoother tone control alpha ='g12.4,  ' must be be    695
     1tween 1.0 and 10.0')                                                  695
 210  format(' sorry, smoother span, span ='g12.4,  ' must not be negati    697
     1ve')                                                                  697
 220  format(' sorry, fitting level lf ='g12.4,  ' must be between 0 and    699
     1 3')                                                                  699
 230  format(' sorry, response weight ww('i4,') =' g12.4,  ' must not be    701
     1 negative')                                                           701
 240  format(' sorry, sum of response weights must be positive.')           702
 250  format(' sorry, observation weight w('i4,') =' g12.4,  ' must not     704
     1be negative')                                                         704
 260  format(' sorry, sum of observation weights must be positive.')        705
 270  format(' unfortunately smart cannot continue with your problem.'/     707
     1 ' please check your input parameters and try again.')                707
      end                                                                   708
      subroutine diagc (q,n,pi,fls,y,ierr)                                  709
      integer q                                                             709
      real pi(q),fls(q,q),y(n)                                              710
      common /parms/ ifl,lf,span,alpha,big                                  711
      data eps /1.0e-4/                                                     712
      ierr=0                                                                713
      do 10 j=1,n                                                           713
      if (y(j).ge.1.0.and.y(j).le.float(q)) go to 10                        714
      if (ifl.gt.0) write (ifl,110) j,y(j),q                                714
      ierr=1                                                                715
 10   continue                                                              716
      if (pi(1).ge.big) go to 60                                            716
      s=0.0                                                                 717
      do 30 i=1,q                                                           718
      if (pi(i).ge.0.0) go to 20                                            718
      ierr=1                                                                718
      if (ifl.gt.0) write (ifl,120) i,pi(i)                                 718
 20   s=s+pi(i)                                                             720
 30   continue                                                              721
      if (s.gt.0.0) go to 40                                                721
      ierr=1                                                                721
      if (ifl.gt.0) write (ifl,160) s                                       721
 40   if ((s.ge.1.0-eps).and.(s.le.1.0+eps)) go to 60                       723
      if (ifl.ne.0.0) write (ifl,130) s                                     723
      do 50 i=1,q                                                           723
      pi(i)=pi(i)/s                                                         723
 50   continue                                                              724
 60   if (fls(1,1).ge.big) go to 100                                        727
      do 90 i=1,q                                                           727
      do 80 j=1,q                                                           728
      if (i.ne.j) go to 70                                                  729
      if (fls(i,j).eq.0.0) go to 80                                         729
      ierr=1                                                                730
      if (ifl.gt.0) write (ifl,140) i,j,fls(i,j)                            731
      go to 80                                                              733
 70   if (fls(i,j).ge.0.0) go to 80                                         733
      ierr=1                                                                734
      if (ifl.gt.0) write (ifl,150) i,j,fls(i,j)                            735
 80   continue                                                              736
 90   continue                                                              737
 100  if (ierr.ne.0.and.ifl.gt.0) write (ifl,170)                           739
      return                                                                740
 110  format(' sorry, observation class y('i4,') ='g12.4,  ' must be bet    742
     1ween 1.0 and 'i3,'.0')                                                742
 120  format(' sorry, class prior pi('i3,') ='g12.4,  ' must not be nega    744
     1tive.')                                                               744
 130  format(' warning, sum of class priors ='g12.4,' should be one.'/      746
     1' priors are being so scaled.')                                       746
 140  format(  ' sorry, correct classification loss fls('i2,','i2,') ='g    749
     112.4,  ' must be zero.')                                              749
 150  format(  ' sorry, misclassification loss fls('i2,','i2,') ='g12.4,    752
     1  ' must not be negative.')                                           752
 160  format(' sorry, sum of class priors ='g12.4,' must be positive')      753
 170  format(' unfortunately smart cannot continue with your problem.'/     755
     1 ' please check your classification parameters and try again.')       755
      end                                                                   756
      subroutine supsmu (n,x,y,w,iper,span,alpha,smo,sc)                    804
      dimension x(n),y(n),w(n),smo(n),sc(n,7)                               805
      common /spans/ spans(3) /consts/ big,sml,eps                          807
      if (x(n).gt.x(1)) go to 30                                            807
      sy=0.0                                                                807
      sw=sy                                                                 808
      do 10 j=1,n                                                           808
      sy=sy+w(j)*y(j)                                                       808
      sw=sw+w(j)                                                            808
 10   continue                                                              809
      a=0.0                                                                 809
      if (sw.gt.0.0) a=sy/sw                                                809
      do 20 j=1,n                                                           809
      smo(j)=a                                                              809
 20   continue                                                              809
      return                                                                810
 30   i=n/4                                                                 811
      j=3*i                                                                 811
      scale=x(j)-x(i)                                                       812
 40   if (scale.gt.0.0) go to 50                                            812
      if (j.lt.n) j=j+1                                                     812
      if (i.gt.1) i=i-1                                                     813
      scale=x(j)-x(i)                                                       814
      go to 40                                                              815
 50   vsmlsq=(eps*scale)**2                                                 816
      jper=iper                                                             816
      if (iper.eq.2.and.(x(1).lt.0.0.or.x(n).gt.1.0)) jper=1                817
      if (jper.lt.1.or.jper.gt.2) jper=1                                    818
      if (span.le.0.0) go to 60                                             819
      call smooth (n,x,y,w,span,jper,vsmlsq,smo,sc)                         819
      return                                                                820
 60   do 70 i=1,3                                                           823
      call smooth (n,x,y,w,spans(i),jper,vsmlsq,sc(1,2*i-1),sc(1,7))        825
      call smooth (n,x,sc(1,7),w,spans(2),-jper,vsmlsq,sc(1,2*i),h)         826
 70   continue                                                              828
      do 90 j=1,n                                                           828
      resmin=big                                                            829
      do 80 i=1,3                                                           830
      if (sc(j,2*i).ge.resmin) go to 80                                     830
      resmin=sc(j,2*i)                                                      830
      sc(j,7)=spans(i)                                                      830
 80   continue                                                              833
      if (alpha.gt.0.0.and.alpha.le.10.0.and.resmin.lt.sc(j,6).and.resmi    836
     1n.gt.0.0) sc(j,7)=sc(j,7)+(spans(3)-sc(j,7))*amax1(sml,resmin/sc(j    836
     2,6))**(10.0-alpha)                                                    836
 90   continue                                                              838
      call smooth (n,x,sc(1,7),w,spans(2),-jper,vsmlsq,sc(1,2),h)           840
      do 110 j=1,n                                                          841
      if (sc(j,2).le.spans(1)) sc(j,2)=spans(1)                             842
      if (sc(j,2).ge.spans(3)) sc(j,2)=spans(3)                             843
      f=sc(j,2)-spans(2)                                                    844
      if (f.ge.0.0) go to 100                                               844
      f=-f/(spans(2)-spans(1))                                              845
      sc(j,4)=(1.0-f)*sc(j,3)+f*sc(j,1)                                     846
      go to 110                                                             847
 100  f=f/(spans(3)-spans(2))                                               848
      sc(j,4)=(1.0-f)*sc(j,3)+f*sc(j,5)                                     849
 110  continue                                                              852
      call smooth (n,x,sc(1,4),w,spans(1),-jper,vsmlsq,smo,h)               853
      return                                                                854
      end                                                                   855
      subroutine smooth (n,x,y,w,span,iper,vsmlsq,smo,acvr)                 856
      dimension x(n),y(n),w(n),smo(n),acvr(n)                               857
      integer in,out                                                        858
      double precision wt,fbo,fbw,xm,ym,tmp,var,cvar,a,h,sy                 864
      xm=0.0                                                                871
      ym=xm                                                                 871
      var=ym                                                                871
      cvar=var                                                              871
      fbw=cvar                                                              871
      jper=iabs(iper)                                                       872
      ibw=0.5*span*n+0.5                                                    873
      if (ibw.lt.2) ibw=2                                                   875
      it=2*ibw+1                                                            876
      do 20 i=1,it                                                          876
      j=i                                                                   876
      if (jper.eq.2) j=i-ibw-1                                              876
      xti=x(j)                                                              877
      if (j.ge.1) go to 10                                                  877
      j=n+j                                                                 877
      xti=x(j)-1.0                                                          877
 10   wt=w(j)                                                               877
      fbo=fbw                                                               877
      fbw=fbw+wt                                                            877
      if (fbw.gt.0.0) xm=(fbo*xm+wt*xti)/fbw                                877
      if (fbw.gt.0.0) ym=(fbo*ym+wt*y(j))/fbw                               877
      tmp=0.0                                                               877
      if (fbo.gt.0.0) tmp=fbw*wt*(xti-xm)/fbo                               877
      var=var+tmp*(xti-xm)                                                  877
      cvar=cvar+tmp*(y(j)-ym)                                               878
 20   continue                                                              880
      do 80 j=1,n                                                           880
      out=j-ibw-1                                                           880
      in=j+ibw                                                              881
      if ((jper.ne.2).and.(out.lt.1.or.in.gt.n)) go to 60                   882
      if (out.ge.1) go to 30                                                882
      out=n+out                                                             882
      xto=x(out)-1.0                                                        882
      xti=x(in)                                                             882
      go to 50                                                              883
 30   if (in.le.n) go to 40                                                 883
      in=in-n                                                               883
      xti=x(in)+1.0                                                         883
      xto=x(out)                                                            883
      go to 50                                                              884
 40   xto=x(out)                                                            884
      xti=x(in)                                                             884
 50   wt=w(out)                                                             885
      fbo=fbw                                                               885
      fbw=fbw-wt                                                            885
      tmp=0.0                                                               885
      if (fbw.gt.0.0) tmp=fbo*wt*(xto-xm)/fbw                               885
      var=var-tmp*(xto-xm)                                                  885
      cvar=cvar-tmp*(y(out)-ym)                                             885
      if (fbw.gt.0.0) xm=(fbo*xm-wt*xto)/fbw                                885
      if (fbw.gt.0.0) ym=(fbo*ym-wt*y(out))/fbw                             885
      wt=w(in)                                                              885
      fbo=fbw                                                               885
      fbw=fbw+wt                                                            885
      if (fbw.gt.0.0) xm=(fbo*xm+wt*xti)/fbw                                885
      if (fbw.gt.0.0) ym=(fbo*ym+wt*y(in))/fbw                              885
      tmp=0.0                                                               885
      if (fbo.gt.0.0) tmp=fbw*wt*(xti-xm)/fbo                               885
      var=var+tmp*(xti-xm)                                                  885
      cvar=cvar+tmp*(y(in)-ym)                                              886
 60   a=0.0                                                                 887
      if (var.gt.vsmlsq) a=cvar/var                                         888
      smo(j)=a*(x(j)-xm)+ym                                                 889
      if (iper.le.0) go to 80                                               890
      h=0.0                                                                 890
      if (fbw.gt.0.0) h=1.0/fbw                                             891
      if (var.gt.vsmlsq) h=h+(x(j)-xm)**2/var                               892
      acvr(j)=0.0                                                           892
      a=1.0-w(j)*h                                                          893
      if (a.le.0.0) go to 70                                                893
      acvr(j)=abs(y(j)-smo(j))/a                                            893
      go to 80                                                              894
 70   if (j.le.1) go to 80                                                  894
      acvr(j)=acvr(j-1)                                                     894
 80   continue                                                              898
      j=1                                                                   899
 90   j0=j                                                                  899
      sy=smo(j)*w(j)                                                        899
      fbw=w(j)                                                              900
      if (j.ge.n) go to 110                                                 901
 100  if (x(j+1).gt.x(j)) go to 110                                         901
      j=j+1                                                                 902
      sy=sy+w(j)*smo(j)                                                     902
      fbw=fbw+w(j)                                                          903
      if (j.ge.n) go to 110                                                 903
      go to 100                                                             904
 110  if (j.le.j0) go to 130                                                905
      a=0.0                                                                 905
      if (fbw.gt.0.0) a=sy/fbw                                              905
      do 120 i=j0,j                                                         905
      smo(i)=a                                                              905
 120  continue                                                              905
 130  j=j+1                                                                 907
      if (j.gt.n) go to 140                                                 907
      go to 90                                                              908
 140  return                                                                909
      end                                                                   910
      block data                                                            911
      common /parms/ ifl,lf,span,alpha,big1                                 912
      common /z00001/ maxit,conv,mitone,cutmin,fdel,cjeps,mitcj             913
      common /spans/ spans(3) /consts/ big,sml,eps                          914
      data ifl,maxit,conv,mitone,cutmin,fdel,span,alpha,big1,cjeps,mitcj    916
     1,lf  /6,20,.005,20,0.1,0.02,0.0,0.0,1.0e20,0.001,1,2/                 916
      data spans,big,sml,eps /0.05,0.2,0.5,1.0e20,1.0e-7,1.0e-3/            917
      end                                                                   918
      subroutine sort (v,a,ii,jj)
c
c     puts into a the permutation vector which sorts v into
c     increasing order.  only elements from ii to jj are considered.
c     arrays iu(k) and il(k) permit sorting up to 2**(k+1)-1 elements
c
c     this is a modification of cacm algorithm #347 by r. c. singleton,
c     which is a modified hoare quicksort.
c
      dimension a(jj),v(1),iu(20),il(20)
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
      vt=v(ij)
      if (v(i).le.vt) go to 30
      a(ij)=a(i)
      a(i)=t
      t=a(ij)
      v(ij)=v(i)
      v(i)=vt
      vt=v(ij)
 30   l=j
      if (v(j).ge.vt) go to 50
      a(ij)=a(j)
      a(j)=t
      t=a(ij)
      v(ij)=v(j)
      v(j)=vt
      vt=v(ij)
      if (v(i).le.vt) go to 50
      a(ij)=a(i)
      a(i)=t
      t=a(ij)
      v(ij)=v(i)
      v(i)=vt
      vt=v(ij)
      go to 50
 40   a(l)=a(k)
      a(k)=tt
      v(l)=v(k)
      v(k)=vtt
 50   l=l-1
      if (v(l).gt.vt) go to 50
      tt=a(l)
      vtt=v(l)
 60   k=k+1
      if (v(k).lt.vt) go to 60
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
      vt=v(i+1)
      if (v(i).le.vt) go to 100
      k=i
 110  a(k+1)=a(k)
      v(k+1)=v(k)
      k=k-1
      if (vt.lt.v(k)) go to 110
      a(k+1)=t
      v(k+1)=vt
      go to 100
      end

