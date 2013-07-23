# =========================================================================
# Freetype GL - A C OpenGL Freetype engine
# Platform:    Any
# WWW:         http://code.google.com/p/freetype-gl/
# -------------------------------------------------------------------------
# Copyright 2011,2012,2013 Nicolas P. Rougier. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY NICOLAS P. ROUGIER ''AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL NICOLAS P. ROUGIER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are
# those of the authors and should not be interpreted as representing official
# policies, either expressed or implied, of Nicolas P. Rougier.
# =========================================================================
PLATFORM		= $(shell uname)
CC				= gcc
CFLAGS			= -Wall `freetype-config --cflags` -I/usr/X11/include -g -O0
LIBS			= -lGLEW -lGL -lglut -lGLU -lm \
	              `freetype-config --libs`
ifeq ($(PLATFORM), Darwin)
	LIBS		= -framework OpenGL -framework GLUT -lGLEW -lm \
	               `freetype-config --libs` -L /usr/X11/lib
endif

DEMOS     := $(patsubst %.c,%,$(wildcard demo-*.c))
DEMOS_ATB := demo-atb-agg
DEMOS_MKP  := demo-markup
DEMOS_MKP2 := demo-markup-2
DEMOS     := $(filter-out $(DEMOS_ATB), $(DEMOS))
DEMOS     := $(filter-out $(DEMOS_MKP), $(DEMOS))
DEMOS     := $(filter-out $(DEMOS_MKP2), $(DEMOS))

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = "/Applications/CMake 2.8-5.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-5.app/Contents/bin/cmake" -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-5.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/pjohalloran/work/svn/freetype-gl-read-only

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/pjohalloran/work/svn/freetype-gl-read-only

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	"/Applications/CMake 2.8-5.app/Contents/bin/ccmake" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	"/Applications/CMake 2.8-5.app/Contents/bin/cmake" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/pjohalloran/work/svn/freetype-gl-read-only/CMakeFiles /Users/pjohalloran/work/svn/freetype-gl-read-only/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/pjohalloran/work/svn/freetype-gl-read-only/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named demo-ansi

# Build rule for target.
demo-ansi: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-ansi
.PHONY : demo-ansi

# fast build rule for target.
demo-ansi/fast:
	$(MAKE) -f CMakeFiles/demo-ansi.dir/build.make CMakeFiles/demo-ansi.dir/build
.PHONY : demo-ansi/fast

#=============================================================================
# Target rules for targets named demo-benchmark

# Build rule for target.
demo-benchmark: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-benchmark
.PHONY : demo-benchmark

# fast build rule for target.
demo-benchmark/fast:
	$(MAKE) -f CMakeFiles/demo-benchmark.dir/build.make CMakeFiles/demo-benchmark.dir/build
.PHONY : demo-benchmark/fast

#=============================================================================
# Target rules for targets named demo-cartoon

# Build rule for target.
demo-cartoon: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-cartoon
.PHONY : demo-cartoon

# fast build rule for target.
demo-cartoon/fast:
	$(MAKE) -f CMakeFiles/demo-cartoon.dir/build.make CMakeFiles/demo-cartoon.dir/build
.PHONY : demo-cartoon/fast

#=============================================================================
# Target rules for targets named demo-console

# Build rule for target.
demo-console: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-console
.PHONY : demo-console

# fast build rule for target.
demo-console/fast:
	$(MAKE) -f CMakeFiles/demo-console.dir/build.make CMakeFiles/demo-console.dir/build
.PHONY : demo-console/fast

#=============================================================================
# Target rules for targets named demo-cube

# Build rule for target.
demo-cube: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-cube
.PHONY : demo-cube

# fast build rule for target.
demo-cube/fast:
	$(MAKE) -f CMakeFiles/demo-cube.dir/build.make CMakeFiles/demo-cube.dir/build
.PHONY : demo-cube/fast

#=============================================================================
# Target rules for targets named demo-display-list

# Build rule for target.
demo-display-list: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-display-list
.PHONY : demo-display-list

# fast build rule for target.
demo-display-list/fast:
	$(MAKE) -f CMakeFiles/demo-display-list.dir/build.make CMakeFiles/demo-display-list.dir/build
.PHONY : demo-display-list/fast

#=============================================================================
# Target rules for targets named demo-distance-field

# Build rule for target.
demo-distance-field: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-distance-field
.PHONY : demo-distance-field

# fast build rule for target.
demo-distance-field/fast:
	$(MAKE) -f CMakeFiles/demo-distance-field.dir/build.make CMakeFiles/demo-distance-field.dir/build
.PHONY : demo-distance-field/fast

#=============================================================================
# Target rules for targets named demo-font

# Build rule for target.
demo-font: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-font
.PHONY : demo-font

# fast build rule for target.
demo-font/fast:
	$(MAKE) -f CMakeFiles/demo-font.dir/build.make CMakeFiles/demo-font.dir/build
.PHONY : demo-font/fast

#=============================================================================
# Target rules for targets named demo-gamma

# Build rule for target.
demo-gamma: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-gamma
.PHONY : demo-gamma

# fast build rule for target.
demo-gamma/fast:
	$(MAKE) -f CMakeFiles/demo-gamma.dir/build.make CMakeFiles/demo-gamma.dir/build
.PHONY : demo-gamma/fast

#=============================================================================
# Target rules for targets named demo-glyph

# Build rule for target.
demo-glyph: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-glyph
.PHONY : demo-glyph

# fast build rule for target.
demo-glyph/fast:
	$(MAKE) -f CMakeFiles/demo-glyph.dir/build.make CMakeFiles/demo-glyph.dir/build
.PHONY : demo-glyph/fast

#=============================================================================
# Target rules for targets named demo-lcd

# Build rule for target.
demo-lcd: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-lcd
.PHONY : demo-lcd

# fast build rule for target.
demo-lcd/fast:
	$(MAKE) -f CMakeFiles/demo-lcd.dir/build.make CMakeFiles/demo-lcd.dir/build
.PHONY : demo-lcd/fast

#=============================================================================
# Target rules for targets named demo-line-aa

# Build rule for target.
demo-line-aa: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-line-aa
.PHONY : demo-line-aa

# fast build rule for target.
demo-line-aa/fast:
	$(MAKE) -f CMakeFiles/demo-line-aa.dir/build.make CMakeFiles/demo-line-aa.dir/build
.PHONY : demo-line-aa/fast

#=============================================================================
# Target rules for targets named demo-make

# Build rule for target.
demo-make: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-make
.PHONY : demo-make

# fast build rule for target.
demo-make/fast:
	$(MAKE) -f CMakeFiles/demo-make.dir/build.make CMakeFiles/demo-make.dir/build
.PHONY : demo-make/fast

#=============================================================================
# Target rules for targets named demo-markup

# Build rule for target.
demo-markup: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-markup
.PHONY : demo-markup

# fast build rule for target.
demo-markup/fast:
	$(MAKE) -f CMakeFiles/demo-markup.dir/build.make CMakeFiles/demo-markup.dir/build
.PHONY : demo-markup/fast

#=============================================================================
# Target rules for targets named demo-outline

# Build rule for target.
demo-outline: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-outline
.PHONY : demo-outline

# fast build rule for target.
demo-outline/fast:
	$(MAKE) -f CMakeFiles/demo-outline.dir/build.make CMakeFiles/demo-outline.dir/build
.PHONY : demo-outline/fast

#=============================================================================
# Target rules for targets named demo-subpixel

# Build rule for target.
demo-subpixel: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-subpixel
.PHONY : demo-subpixel

# fast build rule for target.
demo-subpixel/fast:
	$(MAKE) -f CMakeFiles/demo-subpixel.dir/build.make CMakeFiles/demo-subpixel.dir/build
.PHONY : demo-subpixel/fast

#=============================================================================
# Target rules for targets named demo-texture

# Build rule for target.
demo-texture: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 demo-texture
.PHONY : demo-texture

# fast build rule for target.
demo-texture/fast:
	$(MAKE) -f CMakeFiles/demo-texture.dir/build.make CMakeFiles/demo-texture.dir/build
.PHONY : demo-texture/fast

#=============================================================================
# Target rules for targets named freetype-gl

# Build rule for target.
freetype-gl: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 freetype-gl
.PHONY : freetype-gl

# fast build rule for target.
freetype-gl/fast:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/build
.PHONY : freetype-gl/fast

#=============================================================================
# Target rules for targets named makefont

# Build rule for target.
makefont: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 makefont
.PHONY : makefont

# fast build rule for target.
makefont/fast:
	$(MAKE) -f CMakeFiles/makefont.dir/build.make CMakeFiles/makefont.dir/build
.PHONY : makefont/fast

demo-ansi.o: demo-ansi.c.o
.PHONY : demo-ansi.o

# target to build an object file
demo-ansi.c.o:
	$(MAKE) -f CMakeFiles/demo-ansi.dir/build.make CMakeFiles/demo-ansi.dir/demo-ansi.c.o
.PHONY : demo-ansi.c.o

demo-ansi.i: demo-ansi.c.i
.PHONY : demo-ansi.i

# target to preprocess a source file
demo-ansi.c.i:
	$(MAKE) -f CMakeFiles/demo-ansi.dir/build.make CMakeFiles/demo-ansi.dir/demo-ansi.c.i
.PHONY : demo-ansi.c.i

demo-ansi.s: demo-ansi.c.s
.PHONY : demo-ansi.s

# target to generate assembly for a file
demo-ansi.c.s:
	$(MAKE) -f CMakeFiles/demo-ansi.dir/build.make CMakeFiles/demo-ansi.dir/demo-ansi.c.s
.PHONY : demo-ansi.c.s

demo-benchmark.o: demo-benchmark.c.o
.PHONY : demo-benchmark.o

# target to build an object file
demo-benchmark.c.o:
	$(MAKE) -f CMakeFiles/demo-benchmark.dir/build.make CMakeFiles/demo-benchmark.dir/demo-benchmark.c.o
.PHONY : demo-benchmark.c.o

demo-benchmark.i: demo-benchmark.c.i
.PHONY : demo-benchmark.i

# target to preprocess a source file
demo-benchmark.c.i:
	$(MAKE) -f CMakeFiles/demo-benchmark.dir/build.make CMakeFiles/demo-benchmark.dir/demo-benchmark.c.i
.PHONY : demo-benchmark.c.i

demo-benchmark.s: demo-benchmark.c.s
.PHONY : demo-benchmark.s

# target to generate assembly for a file
demo-benchmark.c.s:
	$(MAKE) -f CMakeFiles/demo-benchmark.dir/build.make CMakeFiles/demo-benchmark.dir/demo-benchmark.c.s
.PHONY : demo-benchmark.c.s

demo-cartoon.o: demo-cartoon.c.o
.PHONY : demo-cartoon.o

# target to build an object file
demo-cartoon.c.o:
	$(MAKE) -f CMakeFiles/demo-cartoon.dir/build.make CMakeFiles/demo-cartoon.dir/demo-cartoon.c.o
.PHONY : demo-cartoon.c.o

demo-cartoon.i: demo-cartoon.c.i
.PHONY : demo-cartoon.i

# target to preprocess a source file
demo-cartoon.c.i:
	$(MAKE) -f CMakeFiles/demo-cartoon.dir/build.make CMakeFiles/demo-cartoon.dir/demo-cartoon.c.i
.PHONY : demo-cartoon.c.i

demo-cartoon.s: demo-cartoon.c.s
.PHONY : demo-cartoon.s

# target to generate assembly for a file
demo-cartoon.c.s:
	$(MAKE) -f CMakeFiles/demo-cartoon.dir/build.make CMakeFiles/demo-cartoon.dir/demo-cartoon.c.s
.PHONY : demo-cartoon.c.s

demo-console.o: demo-console.c.o
.PHONY : demo-console.o

# target to build an object file
demo-console.c.o:
	$(MAKE) -f CMakeFiles/demo-console.dir/build.make CMakeFiles/demo-console.dir/demo-console.c.o
.PHONY : demo-console.c.o

demo-console.i: demo-console.c.i
.PHONY : demo-console.i

# target to preprocess a source file
demo-console.c.i:
	$(MAKE) -f CMakeFiles/demo-console.dir/build.make CMakeFiles/demo-console.dir/demo-console.c.i
.PHONY : demo-console.c.i

demo-console.s: demo-console.c.s
.PHONY : demo-console.s

# target to generate assembly for a file
demo-console.c.s:
	$(MAKE) -f CMakeFiles/demo-console.dir/build.make CMakeFiles/demo-console.dir/demo-console.c.s
.PHONY : demo-console.c.s

demo-cube.o: demo-cube.c.o
.PHONY : demo-cube.o

# target to build an object file
demo-cube.c.o:
	$(MAKE) -f CMakeFiles/demo-cube.dir/build.make CMakeFiles/demo-cube.dir/demo-cube.c.o
.PHONY : demo-cube.c.o

demo-cube.i: demo-cube.c.i
.PHONY : demo-cube.i

# target to preprocess a source file
demo-cube.c.i:
	$(MAKE) -f CMakeFiles/demo-cube.dir/build.make CMakeFiles/demo-cube.dir/demo-cube.c.i
.PHONY : demo-cube.c.i

demo-cube.s: demo-cube.c.s
.PHONY : demo-cube.s

# target to generate assembly for a file
demo-cube.c.s:
	$(MAKE) -f CMakeFiles/demo-cube.dir/build.make CMakeFiles/demo-cube.dir/demo-cube.c.s
.PHONY : demo-cube.c.s

demo-display-list.o: demo-display-list.c.o
.PHONY : demo-display-list.o

# target to build an object file
demo-display-list.c.o:
	$(MAKE) -f CMakeFiles/demo-display-list.dir/build.make CMakeFiles/demo-display-list.dir/demo-display-list.c.o
.PHONY : demo-display-list.c.o

demo-display-list.i: demo-display-list.c.i
.PHONY : demo-display-list.i

# target to preprocess a source file
demo-display-list.c.i:
	$(MAKE) -f CMakeFiles/demo-display-list.dir/build.make CMakeFiles/demo-display-list.dir/demo-display-list.c.i
.PHONY : demo-display-list.c.i

demo-display-list.s: demo-display-list.c.s
.PHONY : demo-display-list.s

# target to generate assembly for a file
demo-display-list.c.s:
	$(MAKE) -f CMakeFiles/demo-display-list.dir/build.make CMakeFiles/demo-display-list.dir/demo-display-list.c.s
.PHONY : demo-display-list.c.s

demo-distance-field.o: demo-distance-field.c.o
.PHONY : demo-distance-field.o

# target to build an object file
demo-distance-field.c.o:
	$(MAKE) -f CMakeFiles/demo-distance-field.dir/build.make CMakeFiles/demo-distance-field.dir/demo-distance-field.c.o
.PHONY : demo-distance-field.c.o

demo-distance-field.i: demo-distance-field.c.i
.PHONY : demo-distance-field.i

# target to preprocess a source file
demo-distance-field.c.i:
	$(MAKE) -f CMakeFiles/demo-distance-field.dir/build.make CMakeFiles/demo-distance-field.dir/demo-distance-field.c.i
.PHONY : demo-distance-field.c.i

demo-distance-field.s: demo-distance-field.c.s
.PHONY : demo-distance-field.s

# target to generate assembly for a file
demo-distance-field.c.s:
	$(MAKE) -f CMakeFiles/demo-distance-field.dir/build.make CMakeFiles/demo-distance-field.dir/demo-distance-field.c.s
.PHONY : demo-distance-field.c.s

demo-font.o: demo-font.c.o
.PHONY : demo-font.o

# target to build an object file
demo-font.c.o:
	$(MAKE) -f CMakeFiles/demo-font.dir/build.make CMakeFiles/demo-font.dir/demo-font.c.o
.PHONY : demo-font.c.o

demo-font.i: demo-font.c.i
.PHONY : demo-font.i

# target to preprocess a source file
demo-font.c.i:
	$(MAKE) -f CMakeFiles/demo-font.dir/build.make CMakeFiles/demo-font.dir/demo-font.c.i
.PHONY : demo-font.c.i

demo-font.s: demo-font.c.s
.PHONY : demo-font.s

# target to generate assembly for a file
demo-font.c.s:
	$(MAKE) -f CMakeFiles/demo-font.dir/build.make CMakeFiles/demo-font.dir/demo-font.c.s
.PHONY : demo-font.c.s

demo-gamma.o: demo-gamma.c.o
.PHONY : demo-gamma.o

# target to build an object file
demo-gamma.c.o:
	$(MAKE) -f CMakeFiles/demo-gamma.dir/build.make CMakeFiles/demo-gamma.dir/demo-gamma.c.o
.PHONY : demo-gamma.c.o

demo-gamma.i: demo-gamma.c.i
.PHONY : demo-gamma.i

# target to preprocess a source file
demo-gamma.c.i:
	$(MAKE) -f CMakeFiles/demo-gamma.dir/build.make CMakeFiles/demo-gamma.dir/demo-gamma.c.i
.PHONY : demo-gamma.c.i

demo-gamma.s: demo-gamma.c.s
.PHONY : demo-gamma.s

# target to generate assembly for a file
demo-gamma.c.s:
	$(MAKE) -f CMakeFiles/demo-gamma.dir/build.make CMakeFiles/demo-gamma.dir/demo-gamma.c.s
.PHONY : demo-gamma.c.s

demo-glyph.o: demo-glyph.c.o
.PHONY : demo-glyph.o

# target to build an object file
demo-glyph.c.o:
	$(MAKE) -f CMakeFiles/demo-glyph.dir/build.make CMakeFiles/demo-glyph.dir/demo-glyph.c.o
.PHONY : demo-glyph.c.o

demo-glyph.i: demo-glyph.c.i
.PHONY : demo-glyph.i

# target to preprocess a source file
demo-glyph.c.i:
	$(MAKE) -f CMakeFiles/demo-glyph.dir/build.make CMakeFiles/demo-glyph.dir/demo-glyph.c.i
.PHONY : demo-glyph.c.i

demo-glyph.s: demo-glyph.c.s
.PHONY : demo-glyph.s

# target to generate assembly for a file
demo-glyph.c.s:
	$(MAKE) -f CMakeFiles/demo-glyph.dir/build.make CMakeFiles/demo-glyph.dir/demo-glyph.c.s
.PHONY : demo-glyph.c.s

demo-lcd.o: demo-lcd.c.o
.PHONY : demo-lcd.o

# target to build an object file
demo-lcd.c.o:
	$(MAKE) -f CMakeFiles/demo-lcd.dir/build.make CMakeFiles/demo-lcd.dir/demo-lcd.c.o
.PHONY : demo-lcd.c.o

demo-lcd.i: demo-lcd.c.i
.PHONY : demo-lcd.i

# target to preprocess a source file
demo-lcd.c.i:
	$(MAKE) -f CMakeFiles/demo-lcd.dir/build.make CMakeFiles/demo-lcd.dir/demo-lcd.c.i
.PHONY : demo-lcd.c.i

demo-lcd.s: demo-lcd.c.s
.PHONY : demo-lcd.s

# target to generate assembly for a file
demo-lcd.c.s:
	$(MAKE) -f CMakeFiles/demo-lcd.dir/build.make CMakeFiles/demo-lcd.dir/demo-lcd.c.s
.PHONY : demo-lcd.c.s

demo-line-aa.o: demo-line-aa.c.o
.PHONY : demo-line-aa.o

# target to build an object file
demo-line-aa.c.o:
	$(MAKE) -f CMakeFiles/demo-line-aa.dir/build.make CMakeFiles/demo-line-aa.dir/demo-line-aa.c.o
.PHONY : demo-line-aa.c.o

demo-line-aa.i: demo-line-aa.c.i
.PHONY : demo-line-aa.i

# target to preprocess a source file
demo-line-aa.c.i:
	$(MAKE) -f CMakeFiles/demo-line-aa.dir/build.make CMakeFiles/demo-line-aa.dir/demo-line-aa.c.i
.PHONY : demo-line-aa.c.i

demo-line-aa.s: demo-line-aa.c.s
.PHONY : demo-line-aa.s

# target to generate assembly for a file
demo-line-aa.c.s:
	$(MAKE) -f CMakeFiles/demo-line-aa.dir/build.make CMakeFiles/demo-line-aa.dir/demo-line-aa.c.s
.PHONY : demo-line-aa.c.s

demo-makefont.o: demo-makefont.c.o
.PHONY : demo-makefont.o

# target to build an object file
demo-makefont.c.o:
	$(MAKE) -f CMakeFiles/demo-make.dir/build.make CMakeFiles/demo-make.dir/demo-makefont.c.o
.PHONY : demo-makefont.c.o

demo-makefont.i: demo-makefont.c.i
.PHONY : demo-makefont.i

# target to preprocess a source file
demo-makefont.c.i:
	$(MAKE) -f CMakeFiles/demo-make.dir/build.make CMakeFiles/demo-make.dir/demo-makefont.c.i
.PHONY : demo-makefont.c.i

demo-makefont.s: demo-makefont.c.s
.PHONY : demo-makefont.s

# target to generate assembly for a file
demo-makefont.c.s:
	$(MAKE) -f CMakeFiles/demo-make.dir/build.make CMakeFiles/demo-make.dir/demo-makefont.c.s
.PHONY : demo-makefont.c.s

demo-markup.o: demo-markup.c.o
.PHONY : demo-markup.o

# target to build an object file
demo-markup.c.o:
	$(MAKE) -f CMakeFiles/demo-markup.dir/build.make CMakeFiles/demo-markup.dir/demo-markup.c.o
.PHONY : demo-markup.c.o

demo-markup.i: demo-markup.c.i
.PHONY : demo-markup.i

# target to preprocess a source file
demo-markup.c.i:
	$(MAKE) -f CMakeFiles/demo-markup.dir/build.make CMakeFiles/demo-markup.dir/demo-markup.c.i
.PHONY : demo-markup.c.i

demo-markup.s: demo-markup.c.s
.PHONY : demo-markup.s

# target to generate assembly for a file
demo-markup.c.s:
	$(MAKE) -f CMakeFiles/demo-markup.dir/build.make CMakeFiles/demo-markup.dir/demo-markup.c.s
.PHONY : demo-markup.c.s

demo-outline.o: demo-outline.c.o
.PHONY : demo-outline.o

# target to build an object file
demo-outline.c.o:
	$(MAKE) -f CMakeFiles/demo-outline.dir/build.make CMakeFiles/demo-outline.dir/demo-outline.c.o
.PHONY : demo-outline.c.o

demo-outline.i: demo-outline.c.i
.PHONY : demo-outline.i

# target to preprocess a source file
demo-outline.c.i:
	$(MAKE) -f CMakeFiles/demo-outline.dir/build.make CMakeFiles/demo-outline.dir/demo-outline.c.i
.PHONY : demo-outline.c.i

demo-outline.s: demo-outline.c.s
.PHONY : demo-outline.s

# target to generate assembly for a file
demo-outline.c.s:
	$(MAKE) -f CMakeFiles/demo-outline.dir/build.make CMakeFiles/demo-outline.dir/demo-outline.c.s
.PHONY : demo-outline.c.s

demo-subpixel.o: demo-subpixel.c.o
.PHONY : demo-subpixel.o

# target to build an object file
demo-subpixel.c.o:
	$(MAKE) -f CMakeFiles/demo-subpixel.dir/build.make CMakeFiles/demo-subpixel.dir/demo-subpixel.c.o
.PHONY : demo-subpixel.c.o

demo-subpixel.i: demo-subpixel.c.i
.PHONY : demo-subpixel.i

# target to preprocess a source file
demo-subpixel.c.i:
	$(MAKE) -f CMakeFiles/demo-subpixel.dir/build.make CMakeFiles/demo-subpixel.dir/demo-subpixel.c.i
.PHONY : demo-subpixel.c.i

demo-subpixel.s: demo-subpixel.c.s
.PHONY : demo-subpixel.s

# target to generate assembly for a file
demo-subpixel.c.s:
	$(MAKE) -f CMakeFiles/demo-subpixel.dir/build.make CMakeFiles/demo-subpixel.dir/demo-subpixel.c.s
.PHONY : demo-subpixel.c.s

demo-texture.o: demo-texture.c.o
.PHONY : demo-texture.o

# target to build an object file
demo-texture.c.o:
	$(MAKE) -f CMakeFiles/demo-texture.dir/build.make CMakeFiles/demo-texture.dir/demo-texture.c.o
.PHONY : demo-texture.c.o

demo-texture.i: demo-texture.c.i
.PHONY : demo-texture.i

# target to preprocess a source file
demo-texture.c.i:
	$(MAKE) -f CMakeFiles/demo-texture.dir/build.make CMakeFiles/demo-texture.dir/demo-texture.c.i
.PHONY : demo-texture.c.i

demo-texture.s: demo-texture.c.s
.PHONY : demo-texture.s

# target to generate assembly for a file
demo-texture.c.s:
	$(MAKE) -f CMakeFiles/demo-texture.dir/build.make CMakeFiles/demo-texture.dir/demo-texture.c.s
.PHONY : demo-texture.c.s

edtaa3func.o: edtaa3func.c.o
.PHONY : edtaa3func.o

# target to build an object file
edtaa3func.c.o:
	$(MAKE) -f CMakeFiles/demo-distance-field.dir/build.make CMakeFiles/demo-distance-field.dir/edtaa3func.c.o
.PHONY : edtaa3func.c.o

edtaa3func.i: edtaa3func.c.i
.PHONY : edtaa3func.i

# target to preprocess a source file
edtaa3func.c.i:
	$(MAKE) -f CMakeFiles/demo-distance-field.dir/build.make CMakeFiles/demo-distance-field.dir/edtaa3func.c.i
.PHONY : edtaa3func.c.i

edtaa3func.s: edtaa3func.c.s
.PHONY : edtaa3func.s

# target to generate assembly for a file
edtaa3func.c.s:
	$(MAKE) -f CMakeFiles/demo-distance-field.dir/build.make CMakeFiles/demo-distance-field.dir/edtaa3func.c.s
.PHONY : edtaa3func.c.s

font-manager.o: font-manager.c.o
.PHONY : font-manager.o

# target to build an object file
font-manager.c.o:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/font-manager.c.o
.PHONY : font-manager.c.o

font-manager.i: font-manager.c.i
.PHONY : font-manager.i

# target to preprocess a source file
font-manager.c.i:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/font-manager.c.i
.PHONY : font-manager.c.i

font-manager.s: font-manager.c.s
.PHONY : font-manager.s

# target to generate assembly for a file
font-manager.c.s:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/font-manager.c.s
.PHONY : font-manager.c.s

makefont.o: makefont.c.o
.PHONY : makefont.o

# target to build an object file
makefont.c.o:
	$(MAKE) -f CMakeFiles/makefont.dir/build.make CMakeFiles/makefont.dir/makefont.c.o
.PHONY : makefont.c.o

makefont.i: makefont.c.i
.PHONY : makefont.i

# target to preprocess a source file
makefont.c.i:
	$(MAKE) -f CMakeFiles/makefont.dir/build.make CMakeFiles/makefont.dir/makefont.c.i
.PHONY : makefont.c.i

makefont.s: makefont.c.s
.PHONY : makefont.s

# target to generate assembly for a file
makefont.c.s:
	$(MAKE) -f CMakeFiles/makefont.dir/build.make CMakeFiles/makefont.dir/makefont.c.s
.PHONY : makefont.c.s

platform.o: platform.c.o
.PHONY : platform.o

# target to build an object file
platform.c.o:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/platform.c.o
.PHONY : platform.c.o

platform.i: platform.c.i
.PHONY : platform.i

# target to preprocess a source file
platform.c.i:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/platform.c.i
.PHONY : platform.c.i

platform.s: platform.c.s
.PHONY : platform.s

# target to generate assembly for a file
platform.c.s:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/platform.c.s
.PHONY : platform.c.s

shader.o: shader.c.o
.PHONY : shader.o

# target to build an object file
shader.c.o:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/shader.c.o
.PHONY : shader.c.o

shader.i: shader.c.i
.PHONY : shader.i

# target to preprocess a source file
shader.c.i:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/shader.c.i
.PHONY : shader.c.i

shader.s: shader.c.s
.PHONY : shader.s

# target to generate assembly for a file
shader.c.s:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/shader.c.s
.PHONY : shader.c.s

text-buffer.o: text-buffer.c.o
.PHONY : text-buffer.o

# target to build an object file
text-buffer.c.o:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/text-buffer.c.o
.PHONY : text-buffer.c.o

text-buffer.i: text-buffer.c.i
.PHONY : text-buffer.i

# target to preprocess a source file
text-buffer.c.i:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/text-buffer.c.i
.PHONY : text-buffer.c.i

text-buffer.s: text-buffer.c.s
.PHONY : text-buffer.s

# target to generate assembly for a file
text-buffer.c.s:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/text-buffer.c.s
.PHONY : text-buffer.c.s

texture-atlas.o: texture-atlas.c.o
.PHONY : texture-atlas.o

# target to build an object file
texture-atlas.c.o:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/texture-atlas.c.o
.PHONY : texture-atlas.c.o

texture-atlas.i: texture-atlas.c.i
.PHONY : texture-atlas.i

# target to preprocess a source file
texture-atlas.c.i:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/texture-atlas.c.i
.PHONY : texture-atlas.c.i

texture-atlas.s: texture-atlas.c.s
.PHONY : texture-atlas.s

# target to generate assembly for a file
texture-atlas.c.s:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/texture-atlas.c.s
.PHONY : texture-atlas.c.s

texture-font.o: texture-font.c.o
.PHONY : texture-font.o

# target to build an object file
texture-font.c.o:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/texture-font.c.o
.PHONY : texture-font.c.o

texture-font.i: texture-font.c.i
.PHONY : texture-font.i

# target to preprocess a source file
texture-font.c.i:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/texture-font.c.i
.PHONY : texture-font.c.i

texture-font.s: texture-font.c.s
.PHONY : texture-font.s

# target to generate assembly for a file
texture-font.c.s:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/texture-font.c.s
.PHONY : texture-font.c.s

vector.o: vector.c.o
.PHONY : vector.o

# target to build an object file
vector.c.o:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/vector.c.o
.PHONY : vector.c.o

vector.i: vector.c.i
.PHONY : vector.i

# target to preprocess a source file
vector.c.i:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/vector.c.i
.PHONY : vector.c.i

vector.s: vector.c.s
.PHONY : vector.s

# target to generate assembly for a file
vector.c.s:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/vector.c.s
.PHONY : vector.c.s

vertex-attribute.o: vertex-attribute.c.o
.PHONY : vertex-attribute.o

# target to build an object file
vertex-attribute.c.o:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/vertex-attribute.c.o
.PHONY : vertex-attribute.c.o

vertex-attribute.i: vertex-attribute.c.i
.PHONY : vertex-attribute.i

# target to preprocess a source file
vertex-attribute.c.i:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/vertex-attribute.c.i
.PHONY : vertex-attribute.c.i

vertex-attribute.s: vertex-attribute.c.s
.PHONY : vertex-attribute.s

# target to generate assembly for a file
vertex-attribute.c.s:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/vertex-attribute.c.s
.PHONY : vertex-attribute.c.s

vertex-buffer.o: vertex-buffer.c.o
.PHONY : vertex-buffer.o

# target to build an object file
vertex-buffer.c.o:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/vertex-buffer.c.o
.PHONY : vertex-buffer.c.o

vertex-buffer.i: vertex-buffer.c.i
.PHONY : vertex-buffer.i

# target to preprocess a source file
vertex-buffer.c.i:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/vertex-buffer.c.i
.PHONY : vertex-buffer.c.i

vertex-buffer.s: vertex-buffer.c.s
.PHONY : vertex-buffer.s

# target to generate assembly for a file
vertex-buffer.c.s:
	$(MAKE) -f CMakeFiles/freetype-gl.dir/build.make CMakeFiles/freetype-gl.dir/vertex-buffer.c.s
.PHONY : vertex-buffer.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... demo-ansi"
	@echo "... demo-benchmark"
	@echo "... demo-cartoon"
	@echo "... demo-console"
	@echo "... demo-cube"
	@echo "... demo-display-list"
	@echo "... demo-distance-field"
	@echo "... demo-font"
	@echo "... demo-gamma"
	@echo "... demo-glyph"
	@echo "... demo-lcd"
	@echo "... demo-line-aa"
	@echo "... demo-make"
	@echo "... demo-markup"
	@echo "... demo-outline"
	@echo "... demo-subpixel"
	@echo "... demo-texture"
	@echo "... edit_cache"
	@echo "... freetype-gl"
	@echo "... makefont"
	@echo "... rebuild_cache"
	@echo "... demo-ansi.o"
	@echo "... demo-ansi.i"
	@echo "... demo-ansi.s"
	@echo "... demo-benchmark.o"
	@echo "... demo-benchmark.i"
	@echo "... demo-benchmark.s"
	@echo "... demo-cartoon.o"
	@echo "... demo-cartoon.i"
	@echo "... demo-cartoon.s"
	@echo "... demo-console.o"
	@echo "... demo-console.i"
	@echo "... demo-console.s"
	@echo "... demo-cube.o"
	@echo "... demo-cube.i"
	@echo "... demo-cube.s"
	@echo "... demo-display-list.o"
	@echo "... demo-display-list.i"
	@echo "... demo-display-list.s"
	@echo "... demo-distance-field.o"
	@echo "... demo-distance-field.i"
	@echo "... demo-distance-field.s"
	@echo "... demo-font.o"
	@echo "... demo-font.i"
	@echo "... demo-font.s"
	@echo "... demo-gamma.o"
	@echo "... demo-gamma.i"
	@echo "... demo-gamma.s"
	@echo "... demo-glyph.o"
	@echo "... demo-glyph.i"
	@echo "... demo-glyph.s"
	@echo "... demo-lcd.o"
	@echo "... demo-lcd.i"
	@echo "... demo-lcd.s"
	@echo "... demo-line-aa.o"
	@echo "... demo-line-aa.i"
	@echo "... demo-line-aa.s"
	@echo "... demo-makefont.o"
	@echo "... demo-makefont.i"
	@echo "... demo-makefont.s"
	@echo "... demo-markup.o"
	@echo "... demo-markup.i"
	@echo "... demo-markup.s"
	@echo "... demo-outline.o"
	@echo "... demo-outline.i"
	@echo "... demo-outline.s"
	@echo "... demo-subpixel.o"
	@echo "... demo-subpixel.i"
	@echo "... demo-subpixel.s"
	@echo "... demo-texture.o"
	@echo "... demo-texture.i"
	@echo "... demo-texture.s"
	@echo "... edtaa3func.o"
	@echo "... edtaa3func.i"
	@echo "... edtaa3func.s"
	@echo "... font-manager.o"
	@echo "... font-manager.i"
	@echo "... font-manager.s"
	@echo "... makefont.o"
	@echo "... makefont.i"
	@echo "... makefont.s"
	@echo "... platform.o"
	@echo "... platform.i"
	@echo "... platform.s"
	@echo "... shader.o"
	@echo "... shader.i"
	@echo "... shader.s"
	@echo "... text-buffer.o"
	@echo "... text-buffer.i"
	@echo "... text-buffer.s"
	@echo "... texture-atlas.o"
	@echo "... texture-atlas.i"
	@echo "... texture-atlas.s"
	@echo "... texture-font.o"
	@echo "... texture-font.i"
	@echo "... texture-font.s"
	@echo "... vector.o"
	@echo "... vector.i"
	@echo "... vector.s"
	@echo "... vertex-attribute.o"
	@echo "... vertex-attribute.i"
	@echo "... vertex-attribute.s"
	@echo "... vertex-buffer.o"
	@echo "... vertex-buffer.i"
	@echo "... vertex-buffer.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

