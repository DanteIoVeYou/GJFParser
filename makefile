CC=g++
SRC=main.cpp
BIN=gen
STATIC=-static
$(BIN):$(SRC)
	$(CC) -o $@ $^ $(STATIC)
.PHONY:clean
clean:
	rm -f $(BIN)
