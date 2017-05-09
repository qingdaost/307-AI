# Make the files in the "bpnn" directory

CC = gcc
CFLAGS = -g  -O
EXE1=nntrain
EXE2=nntest
all: $(EXE1) $(EXE2)

$(EXE1) : 
	$(CC) $(CFLAGS) -o $(EXE1) nntrain.c backprop.c globals.c network.c patterns.c weights.c -lm -lc

$(EXE2):
	$(CC) $(CFLAGS) -o $(EXE2) nntest.c backprop.c globals.c network.c patterns.c weights.c -lm -lc

