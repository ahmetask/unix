# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /home/ahmet/Downloads/clion-2017.3.4/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/ahmet/Downloads/clion-2017.3.4/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ahmet/CLionProjects/unixproject1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ahmet/CLionProjects/unixproject1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/unixproject1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/unixproject1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/unixproject1.dir/flags.make

CMakeFiles/unixproject1.dir/main.c.o: CMakeFiles/unixproject1.dir/flags.make
CMakeFiles/unixproject1.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmet/CLionProjects/unixproject1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/unixproject1.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/unixproject1.dir/main.c.o   -c /home/ahmet/CLionProjects/unixproject1/main.c

CMakeFiles/unixproject1.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/unixproject1.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ahmet/CLionProjects/unixproject1/main.c > CMakeFiles/unixproject1.dir/main.c.i

CMakeFiles/unixproject1.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/unixproject1.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ahmet/CLionProjects/unixproject1/main.c -o CMakeFiles/unixproject1.dir/main.c.s

CMakeFiles/unixproject1.dir/main.c.o.requires:

.PHONY : CMakeFiles/unixproject1.dir/main.c.o.requires

CMakeFiles/unixproject1.dir/main.c.o.provides: CMakeFiles/unixproject1.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/unixproject1.dir/build.make CMakeFiles/unixproject1.dir/main.c.o.provides.build
.PHONY : CMakeFiles/unixproject1.dir/main.c.o.provides

CMakeFiles/unixproject1.dir/main.c.o.provides.build: CMakeFiles/unixproject1.dir/main.c.o


CMakeFiles/unixproject1.dir/log.c.o: CMakeFiles/unixproject1.dir/flags.make
CMakeFiles/unixproject1.dir/log.c.o: ../log.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmet/CLionProjects/unixproject1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/unixproject1.dir/log.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/unixproject1.dir/log.c.o   -c /home/ahmet/CLionProjects/unixproject1/log.c

CMakeFiles/unixproject1.dir/log.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/unixproject1.dir/log.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ahmet/CLionProjects/unixproject1/log.c > CMakeFiles/unixproject1.dir/log.c.i

CMakeFiles/unixproject1.dir/log.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/unixproject1.dir/log.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ahmet/CLionProjects/unixproject1/log.c -o CMakeFiles/unixproject1.dir/log.c.s

CMakeFiles/unixproject1.dir/log.c.o.requires:

.PHONY : CMakeFiles/unixproject1.dir/log.c.o.requires

CMakeFiles/unixproject1.dir/log.c.o.provides: CMakeFiles/unixproject1.dir/log.c.o.requires
	$(MAKE) -f CMakeFiles/unixproject1.dir/build.make CMakeFiles/unixproject1.dir/log.c.o.provides.build
.PHONY : CMakeFiles/unixproject1.dir/log.c.o.provides

CMakeFiles/unixproject1.dir/log.c.o.provides.build: CMakeFiles/unixproject1.dir/log.c.o


# Object files for target unixproject1
unixproject1_OBJECTS = \
"CMakeFiles/unixproject1.dir/main.c.o" \
"CMakeFiles/unixproject1.dir/log.c.o"

# External object files for target unixproject1
unixproject1_EXTERNAL_OBJECTS =

unixproject1: CMakeFiles/unixproject1.dir/main.c.o
unixproject1: CMakeFiles/unixproject1.dir/log.c.o
unixproject1: CMakeFiles/unixproject1.dir/build.make
unixproject1: CMakeFiles/unixproject1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ahmet/CLionProjects/unixproject1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable unixproject1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/unixproject1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/unixproject1.dir/build: unixproject1

.PHONY : CMakeFiles/unixproject1.dir/build

CMakeFiles/unixproject1.dir/requires: CMakeFiles/unixproject1.dir/main.c.o.requires
CMakeFiles/unixproject1.dir/requires: CMakeFiles/unixproject1.dir/log.c.o.requires

.PHONY : CMakeFiles/unixproject1.dir/requires

CMakeFiles/unixproject1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/unixproject1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/unixproject1.dir/clean

CMakeFiles/unixproject1.dir/depend:
	cd /home/ahmet/CLionProjects/unixproject1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ahmet/CLionProjects/unixproject1 /home/ahmet/CLionProjects/unixproject1 /home/ahmet/CLionProjects/unixproject1/cmake-build-debug /home/ahmet/CLionProjects/unixproject1/cmake-build-debug /home/ahmet/CLionProjects/unixproject1/cmake-build-debug/CMakeFiles/unixproject1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/unixproject1.dir/depend

