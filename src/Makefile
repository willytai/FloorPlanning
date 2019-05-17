CSRCS     = $(wildcard *.cpp) $(wildcard *.c)
CHDRS     = $(wildcard *.h)
COBJS     = $(addsuffix .o, $(basename $(CSRCS)))

CXX       = g++
ECHO      = /bin/echo

CFLAGS    = -g -Wall -std=c++11
CFLAGS    = -O3 -Wall -std=c++11

EXEC      = fp
BIN       = ../bin
TARGET    = $(BIN)/$(EXEC)

.PHONY: depend

all: $(COBJS)
	@echo "> building $(EXEC)..."
	@if [ ! -d $(BIN) ]; then \
		mkdir $(BIN); \
		echo "Creating directory \"$(BIN)\"..."; \
	fi;
	@$(CXX) $(CFLAGS) $(COBJS) -o $(TARGET)

%.o : %.cpp
	@$(ECHO) "> compiling: $<"
	@$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f $(COBJS) $(TARGET)

depend: .depend.mak
.depend.mak: $(CSRCS) $(CHDRS)
	@echo Making dependencies ...
	@$(CXX) -MM -I. $(CSRCS) > $@

include .depend.mak
