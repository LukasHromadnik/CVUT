c	this program is a driver for projection pursuit SMART
c
c	maximum ranges
	parameter (nmax=10000, pmax=11, mlmax=100, nsmod=500000,nsp=300000,ndp=10000)
c
	real x(2000000),y(nmax),w(nmax),ww(1),smod(nsmod),sp(nsp)
	real xt(2000000),yt(nmax),xv(pmax),yv(1)
	double precision dp(ndp)
	real sum
	integer n,p,nt,ml,mu,i,j
	character ftrain*100, ftest*100,fout*100
	common /parms/ ifl,lf,span,alpha,big
c
c set observation weights and response weight for one response
c
	data w /nmax*1.0/
	ww(1)=1
c
c	read in the configuration file
c
	open(9,file='pp.config')
	rewind(9)
	read(9,100)ftrain
	read(9,110)n
	read(9,110)p
	read(9,100)ftest
	read(9,110)nt
	read(9,110)ml
	read(9,110)mu
	read(9,100)fout
	close(9)
	write(*,*)'n=',n
	write(*,*)'p=',p
	write(*,*)'nt=',nt
	write(*,*)'ml=',ml
	write(*,*)'mu=',mu
c
c	get training set
c
	open(9,file=ftrain)
	rewind(9)
	do 1 i=1,n
		read(9,*) (x(p*(i-1)+j),j=1,p),y(i)
1	continue
	close(9)
c
c	get test set
c
	open(9,file=ftest)
	rewind(9)
	do 2 i=1,nt
		read(9,*) (xt(p*(i-1)+j),j=1,p),yt(i)
2	continue
	close(9)
c
c	reset default to thorough optimization
c
	lf=3
c
c	run smart
c
	call smartr (ml,mu,p,1,n,w,x,y,ww,smod,nsmod,sp,nsp,dp,ndp)
c
c	evaluate model and output results
c
	sum=0
	open(10,file=fout)
	do 3 i=1,nt
		do 4 j=1,p
			xv(j)=xt(p*(i-1)+j)
4		continue
		call yhat(xv,smod,yv)
		write(10,140) yt(i),yv(1)
		sum=sum+(yt(i)-yv(1))**2
3	continue
	write(6,130)(sum/nt)**.5
	close(10)
100	format(1a50)
110	format(i5)
130	format(1x,'RMS test error=',f10.7)
140	format(99f20.10)
	end
	
