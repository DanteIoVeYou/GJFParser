CC=g++
SRC=main.cpp
BIN=gen
STATIC=-static
STD=-std=c++11
$(BIN):$(SRC)
	$(CC) -o $@ $^ $(STATIC) $(STD) 
.PHONY:clean
clean:
	rm -f $(BIN)
