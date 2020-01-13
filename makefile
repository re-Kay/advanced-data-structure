cc := g++

test: 
	$(cc) $@.cpp -o $@.exe

clean:
	del *.o
	del *.exe