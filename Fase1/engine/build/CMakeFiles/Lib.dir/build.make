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
CMAKE_SOURCE_DIR = /home/jpmag/Transferências/CG/engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jpmag/Transferências/CG/engine/build

# Include any dependencies generated for this target.
include CMakeFiles/Lib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Lib.dir/flags.make

CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.o: CMakeFiles/Lib.dir/flags.make
CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.o: ../tinyxml/tinystr.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jpmag/Transferências/CG/engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.o -c /home/jpmag/Transferências/CG/engine/tinyxml/tinystr.cpp

CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jpmag/Transferências/CG/engine/tinyxml/tinystr.cpp > CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.i

CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jpmag/Transferências/CG/engine/tinyxml/tinystr.cpp -o CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.s

CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.o: CMakeFiles/Lib.dir/flags.make
CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.o: ../tinyxml/tinyxml.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jpmag/Transferências/CG/engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.o -c /home/jpmag/Transferências/CG/engine/tinyxml/tinyxml.cpp

CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jpmag/Transferências/CG/engine/tinyxml/tinyxml.cpp > CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.i

CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jpmag/Transferências/CG/engine/tinyxml/tinyxml.cpp -o CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.s

CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.o: CMakeFiles/Lib.dir/flags.make
CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.o: ../tinyxml/tinyxmlerror.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jpmag/Transferências/CG/engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.o -c /home/jpmag/Transferências/CG/engine/tinyxml/tinyxmlerror.cpp

CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jpmag/Transferências/CG/engine/tinyxml/tinyxmlerror.cpp > CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.i

CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jpmag/Transferências/CG/engine/tinyxml/tinyxmlerror.cpp -o CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.s

CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.o: CMakeFiles/Lib.dir/flags.make
CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.o: ../tinyxml/tinyxmlparser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jpmag/Transferências/CG/engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.o -c /home/jpmag/Transferências/CG/engine/tinyxml/tinyxmlparser.cpp

CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jpmag/Transferências/CG/engine/tinyxml/tinyxmlparser.cpp > CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.i

CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jpmag/Transferências/CG/engine/tinyxml/tinyxmlparser.cpp -o CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.s

# Object files for target Lib
Lib_OBJECTS = \
"CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.o" \
"CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.o" \
"CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.o" \
"CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.o"

# External object files for target Lib
Lib_EXTERNAL_OBJECTS =

libLib.a: CMakeFiles/Lib.dir/tinyxml/tinystr.cpp.o
libLib.a: CMakeFiles/Lib.dir/tinyxml/tinyxml.cpp.o
libLib.a: CMakeFiles/Lib.dir/tinyxml/tinyxmlerror.cpp.o
libLib.a: CMakeFiles/Lib.dir/tinyxml/tinyxmlparser.cpp.o
libLib.a: CMakeFiles/Lib.dir/build.make
libLib.a: CMakeFiles/Lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jpmag/Transferências/CG/engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libLib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/Lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Lib.dir/build: libLib.a

.PHONY : CMakeFiles/Lib.dir/build

CMakeFiles/Lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Lib.dir/clean

CMakeFiles/Lib.dir/depend:
	cd /home/jpmag/Transferências/CG/engine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jpmag/Transferências/CG/engine /home/jpmag/Transferências/CG/engine /home/jpmag/Transferências/CG/engine/build /home/jpmag/Transferências/CG/engine/build /home/jpmag/Transferências/CG/engine/build/CMakeFiles/Lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Lib.dir/depend

