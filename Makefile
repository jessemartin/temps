GCC     = g++
PREFIX = /usr
EXEC   = temps

build : $(EXEC)

clean :
	rm $(EXEC)

$(EXEC) : temps.cpp
	$(GCC) -v -std=c++11 -o $@ $?

install : $(EXEC)
	install -v $(EXEC) $(PREFIX)/bin/$(EXEC)
