ODELIB = .

# ROOT configuration
ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs)
ROOTGLIBS  = $(shell root-config --glibs)

# Compiler and flags
CXXFLAGS  += $(ROOTCFLAGS) -I$(ODELIB) -Wall -O3
LDFLAGS    = $(ROOTLIBS) $(ROOTGLIBS) -Wl,-rpath,$(ODELIB) -L$(ODELIB)
GSLFLAGS   = -lgsl -lgslcblas
CXX        = g++

# List of executables
EXES = baseball1 baseball2 RKnDemo RKnStep RKnTest projGSL vterm vterm_vs_mass

# Default target
all: $(EXES)

# Build rules
RKnDemo: RKnDemo.cpp RKn.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

RKnStep: RKnStep.cpp RKn.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

RKnTest: RKnTest.cpp RKn.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

baseball1: baseball1.cpp RKn.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

baseball2: baseball2.cpp RKn.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

projGSL: projGSL.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(GSLFLAGS)

# vterm uses RKn.cpp
vterm: vterm.cpp RKn.cpp RKn.hpp
	$(CXX) $(CXXFLAGS) -o $@ vterm.cpp RKn.cpp $(LDFLAGS)

# vterm_vs_mass also uses RKn.cpp
vterm_vs_mass: vterm_vs_mass.cpp RKn.cpp RKn.hpp
	$(CXX) $(CXXFLAGS) -o $@ vterm_vs_mass.cpp RKn.cpp $(LDFLAGS)

# Clean
clean:
	rm -f *.o *.so *.d *~ $(EXES)

