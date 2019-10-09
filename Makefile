graph:
	g++ code/graph*.cpp -o graph.out -O3 -I ./headers/

sat:
	g++ code/sat*.cpp -o sat.out -O3 -I ./headers/

clean:
	rm *.out
