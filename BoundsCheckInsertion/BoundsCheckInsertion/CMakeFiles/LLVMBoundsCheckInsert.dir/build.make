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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion

# Include any dependencies generated for this target.
include BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/depend.make

# Include the progress variables for this target.
include BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/progress.make

# Include the compile flags for this target's objects.
include BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/flags.make

BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o: BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/flags.make
BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o: BoundsCheckInsertion/BoundsCheckInsert.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o"
	cd /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o -c /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion/BoundsCheckInsert.cpp

BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.i"
	cd /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion/BoundsCheckInsert.cpp > CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.i

BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.s"
	cd /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion/BoundsCheckInsert.cpp -o CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.s

BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o.requires:
.PHONY : BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o.requires

BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o.provides: BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o.requires
	$(MAKE) -f BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/build.make BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o.provides.build
.PHONY : BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o.provides

BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o.provides.build: BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o

# Object files for target LLVMBoundsCheckInsert
LLVMBoundsCheckInsert_OBJECTS = \
"CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o"

# External object files for target LLVMBoundsCheckInsert
LLVMBoundsCheckInsert_EXTERNAL_OBJECTS =

BoundsCheckInsertion/libLLVMBoundsCheckInsert.so: BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o
BoundsCheckInsertion/libLLVMBoundsCheckInsert.so: BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/build.make
BoundsCheckInsertion/libLLVMBoundsCheckInsert.so: BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libLLVMBoundsCheckInsert.so"
	cd /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LLVMBoundsCheckInsert.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/build: BoundsCheckInsertion/libLLVMBoundsCheckInsert.so
.PHONY : BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/build

BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/requires: BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/BoundsCheckInsert.o.requires
.PHONY : BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/requires

BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/clean:
	cd /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion && $(CMAKE_COMMAND) -P CMakeFiles/LLVMBoundsCheckInsert.dir/cmake_clean.cmake
.PHONY : BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/clean

BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/depend:
	cd /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion /home/cipher1729/Desktop/compilers_project_ph1/BoundsCheckInsertion/BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : BoundsCheckInsertion/CMakeFiles/LLVMBoundsCheckInsert.dir/depend

