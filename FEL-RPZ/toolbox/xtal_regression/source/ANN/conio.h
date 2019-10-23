/*
	conio.h		console I/O
*/

#ifndef	__CONIO_H
#define	__CONIO_H


int	kbhit(void);
char	getch(void);
char	getche(void);
char	putch(char);
int	ungetch(char);
void	cputs(char *);
char	*cgets(char *);
void	sensebrk(void);


#endif
