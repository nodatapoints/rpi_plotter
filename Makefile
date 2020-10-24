plot: plot.o plotter.o plt4822.o
	$(CXX) -o $@ $^

clean:
	rm -vf plot *.o

.PHONY: clean
