# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /apps/sylar/bin/cmake

# The command to remove a file.
RM = /apps/sylar/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hg/helens

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hg/helens/build

# Include any dependencies generated for this target.
include CMakeFiles/test_lru.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_lru.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_lru.dir/flags.make

CMakeFiles/test_lru.dir/tests/test_lru.cc.o: CMakeFiles/test_lru.dir/flags.make
CMakeFiles/test_lru.dir/tests/test_lru.cc.o: ../tests/test_lru.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hg/helens/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_lru.dir/tests/test_lru.cc.o"
	/apps/sylar/bin/g++  $(CXX_DEFINES) -D__FILE__=\"tests/test_lru.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_lru.dir/tests/test_lru.cc.o -c /home/hg/helens/tests/test_lru.cc

CMakeFiles/test_lru.dir/tests/test_lru.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_lru.dir/tests/test_lru.cc.i"
	/apps/sylar/bin/g++ $(CXX_DEFINES) -D__FILE__=\"tests/test_lru.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hg/helens/tests/test_lru.cc > CMakeFiles/test_lru.dir/tests/test_lru.cc.i

CMakeFiles/test_lru.dir/tests/test_lru.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_lru.dir/tests/test_lru.cc.s"
	/apps/sylar/bin/g++ $(CXX_DEFINES) -D__FILE__=\"tests/test_lru.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hg/helens/tests/test_lru.cc -o CMakeFiles/test_lru.dir/tests/test_lru.cc.s

# Object files for target test_lru
test_lru_OBJECTS = \
"CMakeFiles/test_lru.dir/tests/test_lru.cc.o"

# External object files for target test_lru
test_lru_EXTERNAL_OBJECTS =

../bin/test_lru: CMakeFiles/test_lru.dir/tests/test_lru.cc.o
../bin/test_lru: CMakeFiles/test_lru.dir/build.make
../bin/test_lru: ../lib/libhelens.so
../bin/test_lru: /usr/lib64/libz.so
../bin/test_lru: /usr/lib64/libssl.so
../bin/test_lru: /usr/lib64/libcrypto.so
../bin/test_lru: /apps/sylar/lib/libprotobuf.so
../bin/test_lru: CMakeFiles/test_lru.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hg/helens/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_lru"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_lru.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_lru.dir/build: ../bin/test_lru

.PHONY : CMakeFiles/test_lru.dir/build

CMakeFiles/test_lru.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_lru.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_lru.dir/clean

CMakeFiles/test_lru.dir/depend:
	cd /home/hg/helens/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hg/helens /home/hg/helens /home/hg/helens/build /home/hg/helens/build /home/hg/helens/build/CMakeFiles/test_lru.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_lru.dir/depend

