.default: all

all: qwirkle

clean:
	rm -rf qwirkle *.o *.dSYM test/*_actual.save

qwirkle: Tile.o Node.o LinkedList.o Board.o Player.o Game.o qwirkle.o
	g++ -Wall -Werror -std=c++14 -g -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -c $^

test: $(addsuffix \ -\ Test, $(basename $(wildcard test/*.input)))
	@echo "Success: All tests passed."

%\ -\ Test: %.input qwirkle
	@(./qwirkle <$< || echo "ERROR") | diff -w $*.output - >/dev/null || (echo "$@ failed." && exit 1)
	@if [ -e $*.expsave ]; then \
		diff -w -y $*.expsave $*_actual.save >/dev/null || (echo "$@ failed." && exit 1); \
	fi
