.PHONY: clean
clean:
	@rm -f *.out
	@rm -f *.o
	@rm -f *.chss

.PHONY: run
run:
	@gcc src/helpers.c src/check.c src/special_moves.c src/moves.c src/chess_example.c -o chess.o
	@./chess.o

.PHONY: install
install:
	@gcc src/helpers.c src/check.c src/special_moves.c src/moves.c src/chess_example.c -o chess.o
