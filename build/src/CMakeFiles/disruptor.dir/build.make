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
include src/CMakeFiles/disruptor.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/disruptor.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/disruptor.dir/flags.make

src/CMakeFiles/disruptor.dir/ring_buffer.cc.o: src/CMakeFiles/disruptor.dir/flags.make
src/CMakeFiles/disruptor.dir/ring_buffer.cc.o: ../src/ring_buffer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/disruptor.dir/ring_buffer.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor.dir/ring_buffer.cc.o -c /home/zgx/project/DisruptorForCpp/src/ring_buffer.cc

src/CMakeFiles/disruptor.dir/ring_buffer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor.dir/ring_buffer.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/src/ring_buffer.cc > CMakeFiles/disruptor.dir/ring_buffer.cc.i

src/CMakeFiles/disruptor.dir/ring_buffer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor.dir/ring_buffer.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/src/ring_buffer.cc -o CMakeFiles/disruptor.dir/ring_buffer.cc.s

src/CMakeFiles/disruptor.dir/ring_buffer.cc.o.requires:

.PHONY : src/CMakeFiles/disruptor.dir/ring_buffer.cc.o.requires

src/CMakeFiles/disruptor.dir/ring_buffer.cc.o.provides: src/CMakeFiles/disruptor.dir/ring_buffer.cc.o.requires
	$(MAKE) -f src/CMakeFiles/disruptor.dir/build.make src/CMakeFiles/disruptor.dir/ring_buffer.cc.o.provides.build
.PHONY : src/CMakeFiles/disruptor.dir/ring_buffer.cc.o.provides

src/CMakeFiles/disruptor.dir/ring_buffer.cc.o.provides.build: src/CMakeFiles/disruptor.dir/ring_buffer.cc.o


src/CMakeFiles/disruptor.dir/sequence.cc.o: src/CMakeFiles/disruptor.dir/flags.make
src/CMakeFiles/disruptor.dir/sequence.cc.o: ../src/sequence.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/disruptor.dir/sequence.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor.dir/sequence.cc.o -c /home/zgx/project/DisruptorForCpp/src/sequence.cc

src/CMakeFiles/disruptor.dir/sequence.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor.dir/sequence.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/src/sequence.cc > CMakeFiles/disruptor.dir/sequence.cc.i

src/CMakeFiles/disruptor.dir/sequence.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor.dir/sequence.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/src/sequence.cc -o CMakeFiles/disruptor.dir/sequence.cc.s

src/CMakeFiles/disruptor.dir/sequence.cc.o.requires:

.PHONY : src/CMakeFiles/disruptor.dir/sequence.cc.o.requires

src/CMakeFiles/disruptor.dir/sequence.cc.o.provides: src/CMakeFiles/disruptor.dir/sequence.cc.o.requires
	$(MAKE) -f src/CMakeFiles/disruptor.dir/build.make src/CMakeFiles/disruptor.dir/sequence.cc.o.provides.build
.PHONY : src/CMakeFiles/disruptor.dir/sequence.cc.o.provides

src/CMakeFiles/disruptor.dir/sequence.cc.o.provides.build: src/CMakeFiles/disruptor.dir/sequence.cc.o


src/CMakeFiles/disruptor.dir/wait_strategy.cc.o: src/CMakeFiles/disruptor.dir/flags.make
src/CMakeFiles/disruptor.dir/wait_strategy.cc.o: ../src/wait_strategy.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/disruptor.dir/wait_strategy.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor.dir/wait_strategy.cc.o -c /home/zgx/project/DisruptorForCpp/src/wait_strategy.cc

src/CMakeFiles/disruptor.dir/wait_strategy.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor.dir/wait_strategy.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/src/wait_strategy.cc > CMakeFiles/disruptor.dir/wait_strategy.cc.i

src/CMakeFiles/disruptor.dir/wait_strategy.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor.dir/wait_strategy.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/src/wait_strategy.cc -o CMakeFiles/disruptor.dir/wait_strategy.cc.s

src/CMakeFiles/disruptor.dir/wait_strategy.cc.o.requires:

.PHONY : src/CMakeFiles/disruptor.dir/wait_strategy.cc.o.requires

src/CMakeFiles/disruptor.dir/wait_strategy.cc.o.provides: src/CMakeFiles/disruptor.dir/wait_strategy.cc.o.requires
	$(MAKE) -f src/CMakeFiles/disruptor.dir/build.make src/CMakeFiles/disruptor.dir/wait_strategy.cc.o.provides.build
.PHONY : src/CMakeFiles/disruptor.dir/wait_strategy.cc.o.provides

src/CMakeFiles/disruptor.dir/wait_strategy.cc.o.provides.build: src/CMakeFiles/disruptor.dir/wait_strategy.cc.o


src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o: src/CMakeFiles/disruptor.dir/flags.make
src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o: ../src/consumer_barrier.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor.dir/consumer_barrier.cc.o -c /home/zgx/project/DisruptorForCpp/src/consumer_barrier.cc

src/CMakeFiles/disruptor.dir/consumer_barrier.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor.dir/consumer_barrier.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/src/consumer_barrier.cc > CMakeFiles/disruptor.dir/consumer_barrier.cc.i

src/CMakeFiles/disruptor.dir/consumer_barrier.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor.dir/consumer_barrier.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/src/consumer_barrier.cc -o CMakeFiles/disruptor.dir/consumer_barrier.cc.s

src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o.requires:

.PHONY : src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o.requires

src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o.provides: src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o.requires
	$(MAKE) -f src/CMakeFiles/disruptor.dir/build.make src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o.provides.build
.PHONY : src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o.provides

src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o.provides.build: src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o


src/CMakeFiles/disruptor.dir/claim_strategy.cc.o: src/CMakeFiles/disruptor.dir/flags.make
src/CMakeFiles/disruptor.dir/claim_strategy.cc.o: ../src/claim_strategy.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/disruptor.dir/claim_strategy.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor.dir/claim_strategy.cc.o -c /home/zgx/project/DisruptorForCpp/src/claim_strategy.cc

src/CMakeFiles/disruptor.dir/claim_strategy.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor.dir/claim_strategy.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/src/claim_strategy.cc > CMakeFiles/disruptor.dir/claim_strategy.cc.i

src/CMakeFiles/disruptor.dir/claim_strategy.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor.dir/claim_strategy.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/src/claim_strategy.cc -o CMakeFiles/disruptor.dir/claim_strategy.cc.s

src/CMakeFiles/disruptor.dir/claim_strategy.cc.o.requires:

.PHONY : src/CMakeFiles/disruptor.dir/claim_strategy.cc.o.requires

src/CMakeFiles/disruptor.dir/claim_strategy.cc.o.provides: src/CMakeFiles/disruptor.dir/claim_strategy.cc.o.requires
	$(MAKE) -f src/CMakeFiles/disruptor.dir/build.make src/CMakeFiles/disruptor.dir/claim_strategy.cc.o.provides.build
.PHONY : src/CMakeFiles/disruptor.dir/claim_strategy.cc.o.provides

src/CMakeFiles/disruptor.dir/claim_strategy.cc.o.provides.build: src/CMakeFiles/disruptor.dir/claim_strategy.cc.o


src/CMakeFiles/disruptor.dir/sequencer.cc.o: src/CMakeFiles/disruptor.dir/flags.make
src/CMakeFiles/disruptor.dir/sequencer.cc.o: ../src/sequencer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/disruptor.dir/sequencer.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor.dir/sequencer.cc.o -c /home/zgx/project/DisruptorForCpp/src/sequencer.cc

src/CMakeFiles/disruptor.dir/sequencer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor.dir/sequencer.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/src/sequencer.cc > CMakeFiles/disruptor.dir/sequencer.cc.i

src/CMakeFiles/disruptor.dir/sequencer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor.dir/sequencer.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/src/sequencer.cc -o CMakeFiles/disruptor.dir/sequencer.cc.s

src/CMakeFiles/disruptor.dir/sequencer.cc.o.requires:

.PHONY : src/CMakeFiles/disruptor.dir/sequencer.cc.o.requires

src/CMakeFiles/disruptor.dir/sequencer.cc.o.provides: src/CMakeFiles/disruptor.dir/sequencer.cc.o.requires
	$(MAKE) -f src/CMakeFiles/disruptor.dir/build.make src/CMakeFiles/disruptor.dir/sequencer.cc.o.provides.build
.PHONY : src/CMakeFiles/disruptor.dir/sequencer.cc.o.provides

src/CMakeFiles/disruptor.dir/sequencer.cc.o.provides.build: src/CMakeFiles/disruptor.dir/sequencer.cc.o


src/CMakeFiles/disruptor.dir/event/event_interface.cc.o: src/CMakeFiles/disruptor.dir/flags.make
src/CMakeFiles/disruptor.dir/event/event_interface.cc.o: ../src/event/event_interface.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/disruptor.dir/event/event_interface.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor.dir/event/event_interface.cc.o -c /home/zgx/project/DisruptorForCpp/src/event/event_interface.cc

src/CMakeFiles/disruptor.dir/event/event_interface.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor.dir/event/event_interface.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/src/event/event_interface.cc > CMakeFiles/disruptor.dir/event/event_interface.cc.i

src/CMakeFiles/disruptor.dir/event/event_interface.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor.dir/event/event_interface.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/src/event/event_interface.cc -o CMakeFiles/disruptor.dir/event/event_interface.cc.s

src/CMakeFiles/disruptor.dir/event/event_interface.cc.o.requires:

.PHONY : src/CMakeFiles/disruptor.dir/event/event_interface.cc.o.requires

src/CMakeFiles/disruptor.dir/event/event_interface.cc.o.provides: src/CMakeFiles/disruptor.dir/event/event_interface.cc.o.requires
	$(MAKE) -f src/CMakeFiles/disruptor.dir/build.make src/CMakeFiles/disruptor.dir/event/event_interface.cc.o.provides.build
.PHONY : src/CMakeFiles/disruptor.dir/event/event_interface.cc.o.provides

src/CMakeFiles/disruptor.dir/event/event_interface.cc.o.provides.build: src/CMakeFiles/disruptor.dir/event/event_interface.cc.o


src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o: src/CMakeFiles/disruptor.dir/flags.make
src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o: ../src/event/event_publisher.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor.dir/event/event_publisher.cc.o -c /home/zgx/project/DisruptorForCpp/src/event/event_publisher.cc

src/CMakeFiles/disruptor.dir/event/event_publisher.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor.dir/event/event_publisher.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/src/event/event_publisher.cc > CMakeFiles/disruptor.dir/event/event_publisher.cc.i

src/CMakeFiles/disruptor.dir/event/event_publisher.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor.dir/event/event_publisher.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/src/event/event_publisher.cc -o CMakeFiles/disruptor.dir/event/event_publisher.cc.s

src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o.requires:

.PHONY : src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o.requires

src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o.provides: src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o.requires
	$(MAKE) -f src/CMakeFiles/disruptor.dir/build.make src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o.provides.build
.PHONY : src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o.provides

src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o.provides.build: src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o


src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o: src/CMakeFiles/disruptor.dir/flags.make
src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o: ../src/event/event_consumer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/disruptor.dir/event/event_consumer.cc.o -c /home/zgx/project/DisruptorForCpp/src/event/event_consumer.cc

src/CMakeFiles/disruptor.dir/event/event_consumer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/disruptor.dir/event/event_consumer.cc.i"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zgx/project/DisruptorForCpp/src/event/event_consumer.cc > CMakeFiles/disruptor.dir/event/event_consumer.cc.i

src/CMakeFiles/disruptor.dir/event/event_consumer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/disruptor.dir/event/event_consumer.cc.s"
	cd /home/zgx/project/DisruptorForCpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zgx/project/DisruptorForCpp/src/event/event_consumer.cc -o CMakeFiles/disruptor.dir/event/event_consumer.cc.s

src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o.requires:

.PHONY : src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o.requires

src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o.provides: src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o.requires
	$(MAKE) -f src/CMakeFiles/disruptor.dir/build.make src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o.provides.build
.PHONY : src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o.provides

src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o.provides.build: src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o


# Object files for target disruptor
disruptor_OBJECTS = \
"CMakeFiles/disruptor.dir/ring_buffer.cc.o" \
"CMakeFiles/disruptor.dir/sequence.cc.o" \
"CMakeFiles/disruptor.dir/wait_strategy.cc.o" \
"CMakeFiles/disruptor.dir/consumer_barrier.cc.o" \
"CMakeFiles/disruptor.dir/claim_strategy.cc.o" \
"CMakeFiles/disruptor.dir/sequencer.cc.o" \
"CMakeFiles/disruptor.dir/event/event_interface.cc.o" \
"CMakeFiles/disruptor.dir/event/event_publisher.cc.o" \
"CMakeFiles/disruptor.dir/event/event_consumer.cc.o"

# External object files for target disruptor
disruptor_EXTERNAL_OBJECTS =

../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/ring_buffer.cc.o
../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/sequence.cc.o
../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/wait_strategy.cc.o
../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o
../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/claim_strategy.cc.o
../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/sequencer.cc.o
../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/event/event_interface.cc.o
../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o
../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o
../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/build.make
../lib/libdisruptor.so: src/CMakeFiles/disruptor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zgx/project/DisruptorForCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX shared library ../../lib/libdisruptor.so"
	cd /home/zgx/project/DisruptorForCpp/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/disruptor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/disruptor.dir/build: ../lib/libdisruptor.so

.PHONY : src/CMakeFiles/disruptor.dir/build

src/CMakeFiles/disruptor.dir/requires: src/CMakeFiles/disruptor.dir/ring_buffer.cc.o.requires
src/CMakeFiles/disruptor.dir/requires: src/CMakeFiles/disruptor.dir/sequence.cc.o.requires
src/CMakeFiles/disruptor.dir/requires: src/CMakeFiles/disruptor.dir/wait_strategy.cc.o.requires
src/CMakeFiles/disruptor.dir/requires: src/CMakeFiles/disruptor.dir/consumer_barrier.cc.o.requires
src/CMakeFiles/disruptor.dir/requires: src/CMakeFiles/disruptor.dir/claim_strategy.cc.o.requires
src/CMakeFiles/disruptor.dir/requires: src/CMakeFiles/disruptor.dir/sequencer.cc.o.requires
src/CMakeFiles/disruptor.dir/requires: src/CMakeFiles/disruptor.dir/event/event_interface.cc.o.requires
src/CMakeFiles/disruptor.dir/requires: src/CMakeFiles/disruptor.dir/event/event_publisher.cc.o.requires
src/CMakeFiles/disruptor.dir/requires: src/CMakeFiles/disruptor.dir/event/event_consumer.cc.o.requires

.PHONY : src/CMakeFiles/disruptor.dir/requires

src/CMakeFiles/disruptor.dir/clean:
	cd /home/zgx/project/DisruptorForCpp/build/src && $(CMAKE_COMMAND) -P CMakeFiles/disruptor.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/disruptor.dir/clean

src/CMakeFiles/disruptor.dir/depend:
	cd /home/zgx/project/DisruptorForCpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zgx/project/DisruptorForCpp /home/zgx/project/DisruptorForCpp/src /home/zgx/project/DisruptorForCpp/build /home/zgx/project/DisruptorForCpp/build/src /home/zgx/project/DisruptorForCpp/build/src/CMakeFiles/disruptor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/disruptor.dir/depend

