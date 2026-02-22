CC := gcc

TARGET := tinyshell
OBJ := main.o builtins.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

main.o: main.c builtins.h
	$(CC) -c $< -o $@

builtins.o: builtins.c builtins.h
	$(CC) -c $< -o $@

clean:
	rm $(TARGET) $(OBJ)