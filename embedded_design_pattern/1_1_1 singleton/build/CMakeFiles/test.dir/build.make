# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\build"

# Include any dependencies generated for this target.
include CMakeFiles/test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test.dir/flags.make

CMakeFiles/test.dir/main.c.obj: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/main.c.obj: C:/Users/zhang/Desktop/embedded\ design\ pattern/1_1_1\ singleton/main.c
CMakeFiles/test.dir/main.c.obj: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test.dir/main.c.obj"
	C:\Envs\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test.dir/main.c.obj -MF CMakeFiles\test.dir\main.c.obj.d -o CMakeFiles\test.dir\main.c.obj -c "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\main.c"

CMakeFiles/test.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test.dir/main.c.i"
	C:\Envs\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\main.c" > CMakeFiles\test.dir\main.c.i

CMakeFiles/test.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test.dir/main.c.s"
	C:\Envs\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\main.c" -o CMakeFiles\test.dir\main.c.s

CMakeFiles/test.dir/Sensor.c.obj: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Sensor.c.obj: C:/Users/zhang/Desktop/embedded\ design\ pattern/1_1_1\ singleton/Sensor.c
CMakeFiles/test.dir/Sensor.c.obj: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/test.dir/Sensor.c.obj"
	C:\Envs\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test.dir/Sensor.c.obj -MF CMakeFiles\test.dir\Sensor.c.obj.d -o CMakeFiles\test.dir\Sensor.c.obj -c "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\Sensor.c"

CMakeFiles/test.dir/Sensor.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test.dir/Sensor.c.i"
	C:\Envs\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\Sensor.c" > CMakeFiles\test.dir\Sensor.c.i

CMakeFiles/test.dir/Sensor.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test.dir/Sensor.c.s"
	C:\Envs\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\Sensor.c" -o CMakeFiles\test.dir\Sensor.c.s

# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/main.c.obj" \
"CMakeFiles/test.dir/Sensor.c.obj"

# External object files for target test
test_EXTERNAL_OBJECTS =

test.exe: CMakeFiles/test.dir/main.c.obj
test.exe: CMakeFiles/test.dir/Sensor.c.obj
test.exe: CMakeFiles/test.dir/build.make
test.exe: CMakeFiles/test.dir/linkLibs.rsp
test.exe: CMakeFiles/test.dir/objects1.rsp
test.exe: CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable test.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\test.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test.dir/build: test.exe
.PHONY : CMakeFiles/test.dir/build

CMakeFiles/test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\test.dir\cmake_clean.cmake
.PHONY : CMakeFiles/test.dir/clean

CMakeFiles/test.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton" "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton" "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\build" "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\build" "C:\Users\zhang\Desktop\embedded design pattern\1_1_1 singleton\build\CMakeFiles\test.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/test.dir/depend
