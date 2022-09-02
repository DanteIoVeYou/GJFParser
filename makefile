CC=g++
SRC=main.cpp
BIN=gen
$(BIN):$(SRC)
	$(CC) -o $@ $^
.PHONY:clean
clean:
	rm -f $(BIN)
