all: driver.o dtree.o utree.o
	g++ -Wall driver.o dtree.o utree.o -o output.out
utree.o: utree.cpp
	g++ -Wall -c utree.cpp
dtree.o: dtree.cpp
	g++ -Wall -c dtree.cpp
driver.o: driver.cpp
	g++ -Wall -c driver.cpp
run: 
	./output.out
clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#[