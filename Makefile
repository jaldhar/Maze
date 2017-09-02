CXX=c++
CXXFLAGS= -std=c++14 -O2 -g -Wall -Wextra

.POSIX:


.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $^

testmaze: testmaze.o
	$(CXX) $(LDFLAGS) -o $@ $<

clean:
	-rm testmaze *.o
