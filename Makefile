CC := gcc

TARGET := tinyshell
OBJ := main.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm $(TARGET) $(OBJ)