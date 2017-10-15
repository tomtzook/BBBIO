INC := bbbio/include
SRC := bbbio/src
CC := gcc
FLAGS := -I$(INC) -fPIC -shared

TARGET := bbbio.so
SOURCES := $(wildcard $(SRC)/*.c)
HEADERS := $(wildcard $(INC)/*.h)
OBJECTS := $(SOURCES:.c=.o)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(FLAGS)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) -o $(TARGET) $(OBJECTS)
	-rm bbbio/src/*.o

clean:
	