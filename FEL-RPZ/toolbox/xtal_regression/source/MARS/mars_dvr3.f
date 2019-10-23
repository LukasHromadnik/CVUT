c	this program is a driver for mars
c
c	maximum ranges
	parameter (nmax=20000, npmax=11,nkmax=100)
c
	 real x(nmax*npmax),y(nmax),w(nmax),fm(50000),sp(2000000)
	integer lx(npmax),im(50000),mm(2000000)
	double precision dp(100000)
	real ts(nmax*npmax), sp2(nmax,2), ty(nmax), fn(nmax), sum,df
	integer n,p,i,j,nk,model,mi,np,dfi
	character ftrain*50, ftest*50,fout*50,fstat*50,fproc*50
c
c set predictor variable flags to indicate all are unrestricted
c ordinal variables, and set observation weights:
c
c	read in driver file
	write(*,*) '1. Enter training set filename:'
	read(5,100)ftrain
	write(*,*) '2. Enter number of samples in training set:'
	read(5,110)n
	write(*,*) '3. Enter the number of predictor (X) variables in data:'
	read(5,110)np
	write(*,*) '4. Enter the test set filename:'
	read(5,100)ftest
	write(*,*) '5. Enter the number of samples in the test set:'
	read(5,110)p
	write(*,*) '6. Enter the number of Max. Basis Functs to use'
	write(*,*) 'Must be greater than 0 and less than 100:'
	read(5,110)nk
	write(*,*) '7. Enter the number of degrees of freedom'
	write(*,*) 'Must be a integer from 0-9, 9 most smoothing:'
	read(5,110)dfi
	write(*,*) '8. Maximum number of interactions to allow'
	write(*,*) 'Default value is = number of predictor vars:'
	read(5,110)mi
	write(*,*) '9.	Type of spline to use'
	write(*,*) '1=linear (standard)'
	write(*,*) '2=cubic '
	read(5,110)model
	write(*,*) '10. Output filename for runtime messages'
	write(*,*) 'use the name "none" if you do not want this file:'
	read(5,100)fproc
	write(*,*) '11. Output filename for predictions made on test set'
	write(*,*) 'use the name "none" if you do not want this file:'
	read(5,100)fout
	write(*,*) '12. Output filename for rms error results'
	write(*,*) 'use the name "none" if you do not want this file:'
	read(5,100)fstat
c	set predictor variable flags to indicate all are unrestricted
c 	ordinal variables, and set observation weights:
	data lx,w /npmax*1,nmax*1.0/
c
c	set degrees of freedom
c
	df=2.0+dfi*2.0/9.0
	write(*,*) 'degree of freedom given to MARS =',df
	call setdf(df)
c	get training set
c	get test set
	open(9,file=ftest)
	do 2 i=1,p
		read(9,*) (ts(p*(j-1)+i),j=1,np),ty(i)
2	 continue
	close(9)
c	set up all output files
	If (fproc .eq. 'none') go to 4
		open(9,file=fproc)
		call print(9)
		write(9,'(/,''  MARS 3.5 run file'')')
4	If(fout .eq. 'none') go to 5
		open(10,file=fout)
5	If(fstat .eq. 'none') go to 6
		open(11,file=fstat)
6	open(12,file=ftrain)
c	get training set
	do 1 i=1,n
		read(12,*)(x(n*(j-1)+i),j=1,np),y(i)
1	continue
		If (fproc .eq. 'none' ) go to 7
			write(9,120)nk
7		call mars (n,np,x,y,w,nk,mi,lx,fm,im,sp,dp,mm)
c		evaluate model
		call fmod(model,p,ts,fm,im,fn,sp2)
		sum=0
		do 80 i=1,p
			If(fout .eq. 'none') go to 8
				write(10,140) (ts(p*(j-1)+i),j=1,np),fn(i)
8			sum=sum+(fn(i)-ty(i))**2
80		continue
		If(fstat .eq. 'none' ) go to 9
			write(11,130)nk,(sum/p)**.5
9		If(fproc .eq. 'none') go to 90
			write(9,130)nk,(sum/p)**.5
90	close(12)
	close(9)
	close(10)
	close(11)
	stop
100		format(1a50)
110		format(i5)
120		format(1x,'max num of basis functions:',i3)
130		format(1x,' basis fns:',i3,'  RMS test error=',f10.7)
140		format(99f20.10)
	end
