
GTEST_DIR=.
USER_DIR=.

RAYLIB_VERSION     ?= 4.2.0
RAYLIB_PATH        ?= C:/raylib/raylib
RAYLIB_RELEASE_PATH 	?= $(RAYLIB_PATH)/src
RAYLIB_LIBTYPE     ?= STATIC
TESTS = AllTests
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
MAKE = mingw32-make
CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra
all : $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external -Iinclude -ID:/DOOM_web/DOOM_web/"Rendering Raylib"/include
LDFLAGS = -L. -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)/src
LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm 

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

AllTests.o : $(USER_DIR)/AllTests.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -c $(USER_DIR)/AllTests.cc

AllTests : AllTests.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDE_PATHS) $^ -o $@