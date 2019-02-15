.PHONY: clean
clean:
	@rm -f *.out
	@rm -f *.o

.PHONY: run
run:
	@gcc src/helpers.c src/special_moves.c src/moves.c src/chess.c -o chess.o
	@./chess.o

.PHONY: install
install:
	@gcc src/helpers.c src/special_moves.c src/moves.c src/chess.c -o chess.o
