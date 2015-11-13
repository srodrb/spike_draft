CC      = icc
CFLAGS  = -g -O2 -std=c99
DEFINES = -DDEBUG 

# MKL libraries
CLIBS   = -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_core -lmkl_intel_thread -lpthread -lm
CINCS   = -qopenmp -I${MKLROOT}/include

# SuperLU libraries
CLIBS   += -L${SUPERLU_HOME}/lib -lsuperlu_mt_openmp
CINCS   += -I${SUPERLU_HOME}/include


TARGET=spike

OBJS  = spike_matrix.o spike_common.o spike_core.o spike_main.o spike_algebra.o

all:$(TARGET)

spike: $(OBJS) 
	$(CC) $(CFLAGS) $(CINCS) $(DEFINES) $+ -o $@ $(CLIBS)

spike_main.o: spike_main.c
	$(CC) $(CFLAGS) $(CINCS) $(DEFINES) $+ -c

spike_core.o: spike_core.c
	$(CC) $(CFLAGS) $(CINCS) $(DEFINES) $+ -c

spike_algebra.o: spike_algebra.c
	$(CC) $(CFLAGS) $(CINCS) $(DEFINES) $+ -c

spike_matrix.o: spike_matrix.c
	$(CC) $(CFLAGS) $(CINCS) $(DEFINES) $+ -c

spike_common.o: spike_common.c
	$(CC) $(CFLAGS) $(CINCS) $(DEFINES) $+ -c

clean:
	rm -f *.o $(TARGET)

run:
	./spike

.PHONY: clean all run


