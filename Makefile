glbl: glbl.cpp
	g++ -O3 -Wall -Werror $< -o $@

clean:
	rm -f glbl
