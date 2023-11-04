# Compiler settings
MPICXX = mpicxx.mpich
CXXFLAGS = -Wall

# Targets
all: FloodMax OptFloodMax

FloodmakeMax: FloodMax.cpp LeaderElectionAlgorithm.cpp
	$(MPICXX) $(CXXFLAGS) $^ -o $@

OptFloodMax: OptFloodMax.cpp LeaderElectionAlgorithm.cpp
	$(MPICXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f FloodMax OptFloodMax

.PHONY: all clean
