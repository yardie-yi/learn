# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/yardie/Desktop/learn/openGL/project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yardie/Desktop/learn/openGL/project/build

# Include any dependencies generated for this target.
include CMakeFiles/myOpengl.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/myOpengl.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/myOpengl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myOpengl.dir/flags.make

CMakeFiles/myOpengl.dir/src/application.cpp.o: CMakeFiles/myOpengl.dir/flags.make
CMakeFiles/myOpengl.dir/src/application.cpp.o: /home/yardie/Desktop/learn/openGL/project/src/application.cpp
CMakeFiles/myOpengl.dir/src/application.cpp.o: CMakeFiles/myOpengl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/yardie/Desktop/learn/openGL/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myOpengl.dir/src/application.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/myOpengl.dir/src/application.cpp.o -MF CMakeFiles/myOpengl.dir/src/application.cpp.o.d -o CMakeFiles/myOpengl.dir/src/application.cpp.o -c /home/yardie/Desktop/learn/openGL/project/src/application.cpp

CMakeFiles/myOpengl.dir/src/application.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/myOpengl.dir/src/application.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yardie/Desktop/learn/openGL/project/src/application.cpp > CMakeFiles/myOpengl.dir/src/application.cpp.i

CMakeFiles/myOpengl.dir/src/application.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/myOpengl.dir/src/application.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yardie/Desktop/learn/openGL/project/src/application.cpp -o CMakeFiles/myOpengl.dir/src/application.cpp.s

CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.o: CMakeFiles/myOpengl.dir/flags.make
CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.o: /home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c
CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.o: CMakeFiles/myOpengl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/yardie/Desktop/learn/openGL/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.o -MF CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.o.d -o CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.o -c /home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c

CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c > CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.i

CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c -o CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.s

# Object files for target myOpengl
myOpengl_OBJECTS = \
"CMakeFiles/myOpengl.dir/src/application.cpp.o" \
"CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.o"

# External object files for target myOpengl
myOpengl_EXTERNAL_OBJECTS =

myOpengl: CMakeFiles/myOpengl.dir/src/application.cpp.o
myOpengl: CMakeFiles/myOpengl.dir/home/yardie/Desktop/learn/openGL/3th/glad/src/glad.c.o
myOpengl: CMakeFiles/myOpengl.dir/build.make
myOpengl: CMakeFiles/myOpengl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/yardie/Desktop/learn/openGL/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable myOpengl"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myOpengl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myOpengl.dir/build: myOpengl
.PHONY : CMakeFiles/myOpengl.dir/build

CMakeFiles/myOpengl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myOpengl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myOpengl.dir/clean

CMakeFiles/myOpengl.dir/depend:
	cd /home/yardie/Desktop/learn/openGL/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yardie/Desktop/learn/openGL/project /home/yardie/Desktop/learn/openGL/project /home/yardie/Desktop/learn/openGL/project/build /home/yardie/Desktop/learn/openGL/project/build /home/yardie/Desktop/learn/openGL/project/build/CMakeFiles/myOpengl.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/myOpengl.dir/depend

