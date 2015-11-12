CC      = gcc
CFLAGS  = -O2 -std=c99
DEFINES = -DDEBUG 
CLIBS   = 
CINCS   = 

TARGET=spike

OBJS  = spike_matrix.o spike_common.o spike_core.o spike_main.o

all:$(TARGET)

spike: $(OBJS) 
	$(CC) $(CFLAGS) $(CINCS) $(DEFINES) $+ -o $@ $(CLIBS)

spike_main.o: spike_main.c
	$(CC) $(CFLAGS) $(CINCS) $(DEFINES) $+ -c

spike_core.o: spike_core.c
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


