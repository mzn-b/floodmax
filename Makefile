# Compiler settings
MPICXX = mpic++
CXXFLAGS = -Wall -std=c++11

# Targets
all: FloodMax OptFloodMax

FloodMax: FloodMax.cpp
	$(MPICXX) $(CXXFLAGS) $< -o $@

OptFloodMax: OptFloodMax.cpp
	$(MPICXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f FloodMax OptFloodMax

.PHONY: all clean
