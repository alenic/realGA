# Options
DEBUG = 0

# Compiler
CPP=g++
CPPFLAGS=-fPIC -std=c++11

ifeq ($(DEBUG), 1) 
CPPFLAGS+= -g
else
CPPFLAGS+=-O3
endif

# obj
LOCAL_OBJ_PATH=./obj
LIB_OBJECTS=realgenotype.o realgen.o stat.o realgenmultithread.o
LOCAL_OBJECT_FILES=$(addprefix $(LOCAL_OBJ_PATH)/,$(LIB_OBJECTS))

# lib
LOCAL_LIB_PATH=./lib
LIB_VERSION=1.0.0
LIB_A_TARGET=librealgen.a
LIB_SO_TARGET=librealgen.so
L_LIBFLAGS=-L$(LOCAL_LIB_PATH)

# tests
LOCAL_TEST_PATH=./test
TEST_EXEC=test_main.bin

# tests
LOCAL_EXAMPLES_PATH=./examples
EXAMPLES=multithread.bin quadratic.bin readme.bin
EXAMPLES_EXEC= $(addprefix ./examples/,$(EXAMPLES))

# Includes
LOCAL_INCLUDE_PATH=./include

default: all

all: dir lib_a lib_so test examples

examples: $(EXAMPLES_EXEC)

# Create obj and lib dir
dir:
	(test -d $(LOCAL_LIB_PATH) || mkdir -p $(LOCAL_LIB_PATH))
	(test -d $(LOCAL_OBJ_PATH) || mkdir -p $(LOCAL_OBJ_PATH))

# Compiling
obj/%.o: ./src/%.cpp ./include/*.h
	$(CPP) $(CPPFLAGS) -I$(LOCAL_INCLUDE_PATH) -o $@ -c $<


# Building static Library
lib_a: $(LOCAL_LIB_PATH)/$(LIB_A_TARGET)

$(LOCAL_LIB_PATH)/$(LIB_A_TARGET): $(LOCAL_OBJECT_FILES)
	rm -f $(LOCAL_LIB_PATH)/$(LIB_A_TARGET)
	ar -rv $(LOCAL_LIB_PATH)/$(LIB_A_TARGET) $(LOCAL_OBJECT_FILES)
	ranlib $(LOCAL_LIB_PATH)/$(LIB_A_TARGET)
	@echo "****************** Static lib: $(LIB_A_TARGET) [OK] ******************"

# Building dynamic library
lib_so: $(LOCAL_LIB_PATH)/$(LIB_SO_TARGET)

$(LOCAL_LIB_PATH)/$(LIB_SO_TARGET): $(LOCAL_OBJECT_FILES)
	$(CPP) -shared -o $(LOCAL_LIB_PATH)/$(LIB_SO_TARGET) $(LOCAL_OBJECT_FILES)
	@echo "****************** Dynamic lib: $(LIB_SO_TARGET) [OK] ******************"

# Building tests
test: $(LOCAL_TEST_PATH)/$(TEST_EXEC)

test/$(TEST_EXEC): $(LOCAL_TEST_PATH)/*.cpp $(LOCAL_LIB_PATH)/$(LIB_A_TARGET) ./include/*.h
	$(CPP) $(CPPFLAGS) -o $(LOCAL_TEST_PATH)/$(TEST_EXEC) $(LOCAL_TEST_PATH)/*.cpp -I$(LOCAL_INCLUDE_PATH) $(LOCAL_LIB_PATH)/$(LIB_A_TARGET)
	@echo "****************** Test: $(TEST_EXEC) [OK] ******************"


# Building examples
./examples/%.bin: ./examples/%.cpp
	$(CPP) $(CPPFLAGS) -I$(LOCAL_INCLUDE_PATH) -o $@ $< $(LOCAL_LIB_PATH)/$(LIB_A_TARGET)
	@echo "****************** Examples [OK] ******************"


# Clean
clean:
ifneq ($(LOCAL_OBJ_PATH),/)
	(test -d $(LOCAL_OBJ_PATH) && rm -fr $(LOCAL_OBJ_PATH) || true)
endif
ifneq ($(LOCAL_LIB_PATH),/)
	(test -d $(LOCAL_LIB_PATH) && rm -fr $(LOCAL_LIB_PATH) || true)
endif
	(test $(LOCAL_TEST_PATH)/$(TEST_EXEC) && rm $(LOCAL_TEST_PATH)/$(TEST_EXEC) || true)
	(rm examples/*.bin || true)
	@echo "****************** Clean [OK] ******************"
	

.PHONY: clean
