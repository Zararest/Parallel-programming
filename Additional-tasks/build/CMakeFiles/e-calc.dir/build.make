# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/uwu/Desktop/Subjects/ParProga/Additional-tasks

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/uwu/Desktop/Subjects/ParProga/Additional-tasks/build

# Include any dependencies generated for this target.
include CMakeFiles/e-calc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/e-calc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/e-calc.dir/flags.make

CMakeFiles/e-calc.dir/e-task.cpp.o: CMakeFiles/e-calc.dir/flags.make
CMakeFiles/e-calc.dir/e-task.cpp.o: ../e-task.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/uwu/Desktop/Subjects/ParProga/Additional-tasks/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/e-calc.dir/e-task.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/e-calc.dir/e-task.cpp.o -c /home/uwu/Desktop/Subjects/ParProga/Additional-tasks/e-task.cpp

CMakeFiles/e-calc.dir/e-task.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/e-calc.dir/e-task.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/uwu/Desktop/Subjects/ParProga/Additional-tasks/e-task.cpp > CMakeFiles/e-calc.dir/e-task.cpp.i

CMakeFiles/e-calc.dir/e-task.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/e-calc.dir/e-task.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/uwu/Desktop/Subjects/ParProga/Additional-tasks/e-task.cpp -o CMakeFiles/e-calc.dir/e-task.cpp.s

# Object files for target e-calc
e__calc_OBJECTS = \
"CMakeFiles/e-calc.dir/e-task.cpp.o"

# External object files for target e-calc
e__calc_EXTERNAL_OBJECTS =

e-calc: CMakeFiles/e-calc.dir/e-task.cpp.o
e-calc: CMakeFiles/e-calc.dir/build.make
e-calc: /usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi.so
e-calc: CMakeFiles/e-calc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/uwu/Desktop/Subjects/ParProga/Additional-tasks/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable e-calc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/e-calc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/e-calc.dir/build: e-calc

.PHONY : CMakeFiles/e-calc.dir/build

CMakeFiles/e-calc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/e-calc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/e-calc.dir/clean

CMakeFiles/e-calc.dir/depend:
	cd /home/uwu/Desktop/Subjects/ParProga/Additional-tasks/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/uwu/Desktop/Subjects/ParProga/Additional-tasks /home/uwu/Desktop/Subjects/ParProga/Additional-tasks /home/uwu/Desktop/Subjects/ParProga/Additional-tasks/build /home/uwu/Desktop/Subjects/ParProga/Additional-tasks/build /home/uwu/Desktop/Subjects/ParProga/Additional-tasks/build/CMakeFiles/e-calc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/e-calc.dir/depend

