# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/build"

# Include any dependencies generated for this target.
include CMakeFiles/lab_2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/lab_2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/lab_2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab_2.dir/flags.make

CMakeFiles/lab_2.dir/lab_2.cpp.o: CMakeFiles/lab_2.dir/flags.make
CMakeFiles/lab_2.dir/lab_2.cpp.o: ../lab_2.cpp
CMakeFiles/lab_2.dir/lab_2.cpp.o: CMakeFiles/lab_2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lab_2.dir/lab_2.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/lab_2.dir/lab_2.cpp.o -MF CMakeFiles/lab_2.dir/lab_2.cpp.o.d -o CMakeFiles/lab_2.dir/lab_2.cpp.o -c "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/lab_2.cpp"

CMakeFiles/lab_2.dir/lab_2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab_2.dir/lab_2.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/lab_2.cpp" > CMakeFiles/lab_2.dir/lab_2.cpp.i

CMakeFiles/lab_2.dir/lab_2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab_2.dir/lab_2.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/lab_2.cpp" -o CMakeFiles/lab_2.dir/lab_2.cpp.s

CMakeFiles/lab_2.dir/text.cpp.o: CMakeFiles/lab_2.dir/flags.make
CMakeFiles/lab_2.dir/text.cpp.o: ../text.cpp
CMakeFiles/lab_2.dir/text.cpp.o: CMakeFiles/lab_2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/lab_2.dir/text.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/lab_2.dir/text.cpp.o -MF CMakeFiles/lab_2.dir/text.cpp.o.d -o CMakeFiles/lab_2.dir/text.cpp.o -c "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/text.cpp"

CMakeFiles/lab_2.dir/text.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab_2.dir/text.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/text.cpp" > CMakeFiles/lab_2.dir/text.cpp.i

CMakeFiles/lab_2.dir/text.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab_2.dir/text.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/text.cpp" -o CMakeFiles/lab_2.dir/text.cpp.s

CMakeFiles/lab_2.dir/StrM.cpp.o: CMakeFiles/lab_2.dir/flags.make
CMakeFiles/lab_2.dir/StrM.cpp.o: ../StrM.cpp
CMakeFiles/lab_2.dir/StrM.cpp.o: CMakeFiles/lab_2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/lab_2.dir/StrM.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/lab_2.dir/StrM.cpp.o -MF CMakeFiles/lab_2.dir/StrM.cpp.o.d -o CMakeFiles/lab_2.dir/StrM.cpp.o -c "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/StrM.cpp"

CMakeFiles/lab_2.dir/StrM.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab_2.dir/StrM.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/StrM.cpp" > CMakeFiles/lab_2.dir/StrM.cpp.i

CMakeFiles/lab_2.dir/StrM.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab_2.dir/StrM.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/StrM.cpp" -o CMakeFiles/lab_2.dir/StrM.cpp.s

# Object files for target lab_2
lab_2_OBJECTS = \
"CMakeFiles/lab_2.dir/lab_2.cpp.o" \
"CMakeFiles/lab_2.dir/text.cpp.o" \
"CMakeFiles/lab_2.dir/StrM.cpp.o"

# External object files for target lab_2
lab_2_EXTERNAL_OBJECTS =

lab_2: CMakeFiles/lab_2.dir/lab_2.cpp.o
lab_2: CMakeFiles/lab_2.dir/text.cpp.o
lab_2: CMakeFiles/lab_2.dir/StrM.cpp.o
lab_2: CMakeFiles/lab_2.dir/build.make
lab_2: CMakeFiles/lab_2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable lab_2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lab_2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab_2.dir/build: lab_2
.PHONY : CMakeFiles/lab_2.dir/build

CMakeFiles/lab_2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lab_2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lab_2.dir/clean

CMakeFiles/lab_2.dir/depend:
	cd "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2" "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2" "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/build" "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/build" "/home/kalujny/work/leti_labs/2 SEMESTR/lab_2/build/CMakeFiles/lab_2.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/lab_2.dir/depend

