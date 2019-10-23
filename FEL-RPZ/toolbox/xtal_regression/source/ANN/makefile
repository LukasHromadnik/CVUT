#makefile for neural
CC=g++
CFLAGS=-g -O 
LIBS=-lm
NN_OBJS=act_func.o activity.o anneal.o confuse.o \
        conjgrad.o  control.o  direcmin.o \
        dotprod.o execute.o gen_init.o \
        kohnet.o layernet.o longrand.o \
        mem.o messages.o network.o neural.o \
        parsdubl.o regress.o shake.o svdcmp.o \
        train.o veclen.o wt_save.o 

#NN_OBJS=act_func activity anneal confuse \
        conjgrad  control  direcmin \
        dotprod execute gen_init \
        kohnet layernet longrand \
        mem messages network neural \
        parsdubl regress shake svdcmp \
        train veclen wt_save 

NN_CC = act_func.c activity.c anneal.c \
        confuse.c  conjgrad.c control.c \
        direcmin.c dotprod.c  execute.c \
        gen_init.c kohnet.c   layernet.c \
        longrand.c mem.c      messages.c \
        network.c  neural.c   parsdubl.c \
        regress.c  shake.c    svdcmp.c \
        train.c    veclen.c   wt_save.c

HEADERS=classes.h const.h funcdefs.h

all : neuron 

$(NN_OBJS) : $(HEADERS) $(NN_CC)
	${CC} $(CFLAGS) $*.c -o $@ -c

neuron : $(NN_OBJS)
	${CC} $(CFLAGS) -o neural $(NN_OBJS) $(LIBS)



