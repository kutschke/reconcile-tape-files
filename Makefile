
HDRS = $(wildcard src/*.hh)
SRCS = $(wildcard src/*.cc)
OBJS = $(subst .cc,.o, $(SRCS))

CCFLAGS = --std=c++17 -Wall -Wno-unused-local-typedefs -g -Werror -gdwarf-2 -Werror=return-type -Winit-self -Woverloaded-virtual -O3 -fno-omit-frame-pointer -DNDEBUG -I$(BOOST_INC)
LFLAGS =  $(CCFLAGS) -fPIC -L$(BOOST_LIB) -l boost_program_options

all: bin/reconcile-tape-files

# Pretend that any change to any header requires a full recompilation.
%.o: src/%.cc $(HDRS)
	g++ $(CCFLAGS) -c $< -o $@

bin/reconcile-tape-files: $(OBJS)
	g++ -o bin/reconcile-tape-files $(LFLAGS) $(OBJS)

clean:
	rm $(OBJS) bin/reconcile-tape-files


dump:
	echo $(OBJS)
