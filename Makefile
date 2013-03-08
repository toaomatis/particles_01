SOURCES = main.c mainwindow.c particle.c helper.c
OBJS = ${SOURCES:.c=.o}

DEBUG_CFLAGS = -O0 -g
BASE_CFLAGS += -I. -D_GNU_SOURCE
BASE_CFLAGS += $(DEBUG_CFLAGS)
CFLAGS +=  -Wall -std=c99 ${BASE_CFLAGS}
LIBS =  -lglut -lGLEW

EXE_NAME = particles_01

all: $(EXE_NAME)


$(EXE_NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(OBJS) $(LDFLAGS) $(LIBS)

.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(EXE_NAME)
	rm -f testcairo
