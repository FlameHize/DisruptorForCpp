# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/zgx/project/DisruptorForCpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zgx/project/DisruptorForCpp/build

# Include any dependencies generated for this target.
include test/CMakeFiles/disruptor_test.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/disruptor_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/disruptor_test.dir/flags.make

test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o: test/CMakeFiles/disruptor_test.dir/flags.make
test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o: ../test/claim_strategy_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o -c /home/zgx/project/DisruptorForCpp/test/claim_strategy_test.cc

test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/test/claim_strategy_test.cc > CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.i

test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/test/claim_strategy_test.cc -o CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.s

test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o.requires:

.PHONY : test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o.requires

test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o.provides: test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o.requires
	$(MAKE) -f test/CMakeFiles/disruptor_test.dir/build.make test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o.provides.build
.PHONY : test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o.provides

test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o.provides.build: test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o


test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o: test/CMakeFiles/disruptor_test.dir/flags.make
test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o: ../test/consumer_barrier_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o -c /home/zgx/project/DisruptorForCpp/test/consumer_barrier_test.cc

test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/test/consumer_barrier_test.cc > CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.i

test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/test/consumer_barrier_test.cc -o CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.s

test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o.requires:

.PHONY : test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o.requires

test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o.provides: test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o.requires
	$(MAKE) -f test/CMakeFiles/disruptor_test.dir/build.make test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o.provides.build
.PHONY : test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o.provides

test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o.provides.build: test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o


test/CMakeFiles/disruptor_test.dir/main.cc.o: test/CMakeFiles/disruptor_test.dir/flags.make
test/CMakeFiles/disruptor_test.dir/main.cc.o: ../test/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object test/CMakeFiles/disruptor_test.dir/main.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor_test.dir/main.cc.o -c /home/zgx/project/DisruptorForCpp/test/main.cc

test/CMakeFiles/disruptor_test.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor_test.dir/main.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/test/main.cc > CMakeFiles/disruptor_test.dir/main.cc.i

test/CMakeFiles/disruptor_test.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor_test.dir/main.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/test/main.cc -o CMakeFiles/disruptor_test.dir/main.cc.s

test/CMakeFiles/disruptor_test.dir/main.cc.o.requires:

.PHONY : test/CMakeFiles/disruptor_test.dir/main.cc.o.requires

test/CMakeFiles/disruptor_test.dir/main.cc.o.provides: test/CMakeFiles/disruptor_test.dir/main.cc.o.requires
	$(MAKE) -f test/CMakeFiles/disruptor_test.dir/build.make test/CMakeFiles/disruptor_test.dir/main.cc.o.provides.build
.PHONY : test/CMakeFiles/disruptor_test.dir/main.cc.o.provides

test/CMakeFiles/disruptor_test.dir/main.cc.o.provides.build: test/CMakeFiles/disruptor_test.dir/main.cc.o


test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o: test/CMakeFiles/disruptor_test.dir/flags.make
test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o: ../test/ring_buffer_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o -c /home/zgx/project/DisruptorForCpp/test/ring_buffer_test.cc

test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/test/ring_buffer_test.cc > CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.i

test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/test/ring_buffer_test.cc -o CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.s

test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o.requires:

.PHONY : test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o.requires

test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o.provides: test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o.requires
	$(MAKE) -f test/CMakeFiles/disruptor_test.dir/build.make test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o.provides.build
.PHONY : test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o.provides

test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o.provides.build: test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o


test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o: test/CMakeFiles/disruptor_test.dir/flags.make
test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o: ../test/sequence_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor_test.dir/sequence_test.cc.o -c /home/zgx/project/DisruptorForCpp/test/sequence_test.cc

test/CMakeFiles/disruptor_test.dir/sequence_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor_test.dir/sequence_test.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/test/sequence_test.cc > CMakeFiles/disruptor_test.dir/sequence_test.cc.i

test/CMakeFiles/disruptor_test.dir/sequence_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor_test.dir/sequence_test.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/test/sequence_test.cc -o CMakeFiles/disruptor_test.dir/sequence_test.cc.s

test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o.requires:

.PHONY : test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o.requires

test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o.provides: test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o.requires
	$(MAKE) -f test/CMakeFiles/disruptor_test.dir/build.make test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o.provides.build
.PHONY : test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o.provides

test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o.provides.build: test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o


test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o: test/CMakeFiles/disruptor_test.dir/flags.make
test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o: ../test/sequencer_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor_test.dir/sequencer_test.cc.o -c /home/zgx/project/DisruptorForCpp/test/sequencer_test.cc

test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor_test.dir/sequencer_test.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/test/sequencer_test.cc > CMakeFiles/disruptor_test.dir/sequencer_test.cc.i

test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor_test.dir/sequencer_test.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/test/sequencer_test.cc -o CMakeFiles/disruptor_test.dir/sequencer_test.cc.s

test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o.requires:

.PHONY : test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o.requires

test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o.provides: test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o.requires
	$(MAKE) -f test/CMakeFiles/disruptor_test.dir/build.make test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o.provides.build
.PHONY : test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o.provides

test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o.provides.build: test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o


test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o: test/CMakeFiles/disruptor_test.dir/flags.make
test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o: ../test/wait_strategy_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o -c /home/zgx/project/DisruptorForCpp/test/wait_strategy_test.cc

test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/test/wait_strategy_test.cc > CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.i

test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/test/wait_strategy_test.cc -o CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.s

test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o.requires:

.PHONY : test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o.requires

test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o.provides: test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o.requires
	$(MAKE) -f test/CMakeFiles/disruptor_test.dir/build.make test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o.provides.build
.PHONY : test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o.provides

test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o.provides.build: test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o


# Object files for target disruptor_test
disruptor_test_OBJECTS = \
"CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o" \
"CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o" \
"CMakeFiles/disruptor_test.dir/main.cc.o" \
"CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o" \
"CMakeFiles/disruptor_test.dir/sequence_test.cc.o" \
"CMakeFiles/disruptor_test.dir/sequencer_test.cc.o" \
"CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o"

# External object files for target disruptor_test
disruptor_test_EXTERNAL_OBJECTS =

../bin/disruptor_test: test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o
../bin/disruptor_test: test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o
../bin/disruptor_test: test/CMakeFiles/disruptor_test.dir/main.cc.o
../bin/disruptor_test: test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o
../bin/disruptor_test: test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o
../bin/disruptor_test: test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o
../bin/disruptor_test: test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o
../bin/disruptor_test: test/CMakeFiles/disruptor_test.dir/build.make
../bin/disruptor_test: ../lib/libdisruptor.so
../bin/disruptor_test: /usr/lib/libgtest.a
../bin/disruptor_test: test/CMakeFiles/disruptor_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable ../../bin/disruptor_test"
	cd /home/zgx/project/DisruptorForCpp/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/disruptor_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/disruptor_test.dir/build: ../bin/disruptor_test

.PHONY : test/CMakeFiles/disruptor_test.dir/build

test/CMakeFiles/disruptor_test.dir/requires: test/CMakeFiles/disruptor_test.dir/claim_strategy_test.cc.o.requires
test/CMakeFiles/disruptor_test.dir/requires: test/CMakeFiles/disruptor_test.dir/consumer_barrier_test.cc.o.requires
test/CMakeFiles/disruptor_test.dir/requires: test/CMakeFiles/disruptor_test.dir/main.cc.o.requires
test/CMakeFiles/disruptor_test.dir/requires: test/CMakeFiles/disruptor_test.dir/ring_buffer_test.cc.o.requires
test/CMakeFiles/disruptor_test.dir/requires: test/CMakeFiles/disruptor_test.dir/sequence_test.cc.o.requires
test/CMakeFiles/disruptor_test.dir/requires: test/CMakeFiles/disruptor_test.dir/sequencer_test.cc.o.requires
test/CMakeFiles/disruptor_test.dir/requires: test/CMakeFiles/disruptor_test.dir/wait_strategy_test.cc.o.requires

.PHONY : test/CMakeFiles/disruptor_test.dir/requires

test/CMakeFiles/disruptor_test.dir/clean:
	cd /home/zgx/project/DisruptorForCpp/build/test && $(CMAKE_COMMAND) -P CMakeFiles/disruptor_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/disruptor_test.dir/clean

test/CMakeFiles/disruptor_test.dir/depend:
	cd /home/zgx/project/DisruptorForCpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zgx/project/DisruptorForCpp /home/zgx/project/DisruptorForCpp/test /home/zgx/project/DisruptorForCpp/build /home/zgx/project/DisruptorForCpp/build/test /home/zgx/project/DisruptorForCpp/build/test/CMakeFiles/disruptor_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/disruptor_test.dir/depend

