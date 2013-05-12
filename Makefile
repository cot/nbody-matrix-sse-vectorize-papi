CC     = gcc
CFLAGS = -lm -W -Wall -ansi -pedantic  
LFLAGS = -fPIC -shared

SRC    = src
BIN    = bin
INC    = -I/inc
LIB    = lib

all: nbody

optim: CC += -O3
optim: nbody

sse: CC += -msse4  -O2 -ftree-vectorize -ftree-vectorizer-verbose=5 -funroll-loops
sse: nbody

papi: CC += -DPAPI -lpapi
papi: nbody

matrix: CC += -DMATRIX -O2 -funroll-loops
matrix: nbody

debug: CC += -DDEBUG -DMATRIX -g
debug: nbody

profile: CC += -DPROFILE -g -pg
profile: nbody

report: CC += -O2 -ftree-vectorize -msse4 -ftree-vectorizer-verbose=5 -funroll-loops
report: nbody

nbody: $(SRC)/nbody.c libproto
	$(CC) $(INC) $(SRC)/nbody.c $(LIB)/* -o $(BIN)/nbody $(CFLAGS)

libproto: $(SRC)/proto.c
	$(CC) $(INC) $(SRC)/proto.c $(LFLAGS) -o $(LIB)/libproto.so

clean:
	rm -rf *.o $(BIN)/nbody 

veryclean :
	rm -rf $(BIN)/nbody $(LIB)/*.so 
