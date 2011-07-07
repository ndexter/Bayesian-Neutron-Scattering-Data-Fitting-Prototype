CC = g++
CPPFLAGS = -g -I/opt/local/include/ 
LDFLAGS = -L/opt/local/lib
LIBS    = -lgsl -lgslcblas 
SRC = pocbayes.cpp
OBJS = pocbayes.o
EXE = bayes


.SUFFIXES: .cpp

.cpp.o: $*.cpp 
	$(CC) $(CPPFLAGS) -c $*.cpp

$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LDFLAGS) $(LIBS)

all: $(EXE)

clean:
	rm -rf a.out *.o $(EXE)
