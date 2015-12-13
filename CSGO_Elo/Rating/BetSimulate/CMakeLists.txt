cmake_minimum_required (VERSION 2.6)
project (CSGOElo)

# executable
add_executable(Main Simulation.cpp MatchUpdate.cpp Hashtable.cpp Team.cpp TeamUpdate.cpp)
