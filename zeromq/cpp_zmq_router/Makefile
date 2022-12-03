.PHONY : dummy clean

CFLAGS    = -Wall
LDFLAGS    = -lzmq -lpthread

CC        = gcc -g
CXX        = g++ -g
MAKEF    = make -f Makefile
CPA        = cp -a
MAKE    = $(CC)

subdir-list         = $(patsubst %,_subdir_%,$(SUB_DIRS))
subdir-clean-list     = $(patsubst %,_subdir_clean_%,$(SUB_DIRS))

%.o: %.c
    $(MAKE) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
    $(MAKE) -o $@ -c $< $(CFLAGS)

%.os: %.c
    $(MAKE) -fPIC -c $< -o $@ $(CFLAGS) 

%.os: %.cpp
    $(MAKE) -fPIC -c $< -o $@ $(CFLAGS) 

ALL_FILES    = recv send router

all : $(ALL_FILES)

recv : recv.o comm.o
    $(MAKE) -o $@ $(LDFLAGS) $^

send : send.o comm.o
    $(MAKE) -o $@ $(LDFLAGS) $^

router : router.o comm.o
    $(MAKE) -o $@ $(LDFLAGS) $^

clean : 
    rm -rf *.o
    rm -rf $(ALL_FILES)