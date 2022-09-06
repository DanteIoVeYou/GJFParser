CC=g++
SRC=main.cpp
BIN=gen
STATIC=-static
$(BIN):$(SRC)
	$(CC) -o $@ $^ $(STATIC) -std=c++11
.PHONY:clean
clean:
	rm -f $(BIN)
