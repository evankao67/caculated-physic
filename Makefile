dijkstra.out: supportLib.o pbPlots.o
	g++ -o dijkstra.out FinalDijkstra.cpp supportLib.o pbPlots.o -lm -std=c++11
supportLib.o:
	g++ supportLib.cpp -o supportLib.o -std=c++98 -O3 -march=native -c 
pbPlots.o:
	g++ pbPlots.cpp -o pbPlots.o -std=c++98 -O3 -march=native -c 

clean:
	rm -f *.o dijkstra.out
