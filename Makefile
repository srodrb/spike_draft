CC      = icc
CFLAGS  = -O2 -std=c99
DEFINES = -DDEBUG 
CLIBS   = 
CINCS   = 

TARGET=spike

all:$(TARGET)

spike: spike_main.o spike_matrix.o spike_common.o
	$(CC) $(CFLAGS) $(CINCS) $(DEFINES) $+ -o $@ $(CLIBS)

spike_main.o: spike_main.c
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


