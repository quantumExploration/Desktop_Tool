
MACHINE= $(shell uname -s)

INC= -I../install/include/ 
LIB= -L../install/lib/ -lfftw3 -lm

CXX=g++ 

COMPILER_FLAGS= -g

INCLUDE= -I../install/include/ 
LIBS= $(LIB)

TARGETS =hybridImage 

SRCS=hybridImage.cpp 
OBJECTS=hybridImage.o 


default: $(TARGETS)

OBJS = $(SRCS:.cpp=.o)
.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) -c -Wno-deprecated  $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

hybridImage: hybridImage.o
	$(CXX) $(COMPILER_FLAGS) $(LIBS) hybridImage.o -o hybridImage -lfftw3 -lm
clean:
	rm -f $(OBJS) $(TARGETS) *~ \.*.swo \.*.swp

#rm ./exe/qdot*

