# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2

# Include any dependencies generated for this target.
include CMakeFiles/vector_convex_hull_2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/vector_convex_hull_2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vector_convex_hull_2.dir/flags.make

CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o: CMakeFiles/vector_convex_hull_2.dir/flags.make
CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o: vector_convex_hull_2.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o -c /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2/vector_convex_hull_2.cpp

CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2/vector_convex_hull_2.cpp > CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.i

CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2/vector_convex_hull_2.cpp -o CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.s

CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o.requires:
.PHONY : CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o.requires

CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o.provides: CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o.requires
	$(MAKE) -f CMakeFiles/vector_convex_hull_2.dir/build.make CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o.provides.build
.PHONY : CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o.provides

CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o.provides.build: CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o

# Object files for target vector_convex_hull_2
vector_convex_hull_2_OBJECTS = \
"CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o"

# External object files for target vector_convex_hull_2
vector_convex_hull_2_EXTERNAL_OBJECTS =

vector_convex_hull_2: CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o
vector_convex_hull_2: CMakeFiles/vector_convex_hull_2.dir/build.make
vector_convex_hull_2: /usr/lib64/libmpfr.so
vector_convex_hull_2: /usr/lib64/libgmp.so
vector_convex_hull_2: /usr/lib64/libCGAL.so
vector_convex_hull_2: /usr/lib64/libboost_thread-mt.so
vector_convex_hull_2: /usr/lib64/libboost_system-mt.so
vector_convex_hull_2: /usr/lib64/libCGAL.so
vector_convex_hull_2: /usr/lib64/libboost_thread-mt.so
vector_convex_hull_2: /usr/lib64/libboost_system-mt.so
vector_convex_hull_2: CMakeFiles/vector_convex_hull_2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable vector_convex_hull_2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vector_convex_hull_2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/vector_convex_hull_2.dir/build: vector_convex_hull_2
.PHONY : CMakeFiles/vector_convex_hull_2.dir/build

CMakeFiles/vector_convex_hull_2.dir/requires: CMakeFiles/vector_convex_hull_2.dir/vector_convex_hull_2.cpp.o.requires
.PHONY : CMakeFiles/vector_convex_hull_2.dir/requires

CMakeFiles/vector_convex_hull_2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vector_convex_hull_2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vector_convex_hull_2.dir/clean

CMakeFiles/vector_convex_hull_2.dir/depend:
	cd /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2 /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2 /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2 /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2 /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/preprocessing/generateVTK/Convex_hull_2/CMakeFiles/vector_convex_hull_2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vector_convex_hull_2.dir/depend

