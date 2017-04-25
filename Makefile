# Options
DEBUG = 0
NAME = realgen


# Compiler
CPP=g++
CPPFLAGS=-fPIC

ifeq ($(DEBUG), 1) 
CPPFLAGS+= -g
else
CPPFLAGS+=-O3
endif

# obj
LOCAL_OBJ_PATH=./obj
LIB_OBJECTS=realgenotype.o realgen.o stat.o
LOCAL_OBJECT_FILES=$(addprefix $(LOCAL_OBJ_PATH)/,$(LIB_OBJECTS))

# lib
LOCAL_LIB_PATH=./lib
LIB_VERSION=1.0.0
LIB_A_TARGET=lib$(NAME).a
LIB_SO_TARGET=lib$(NAME).so
L_LIBFLAGS=-L$(LOCAL_LIB_PATH)

# tests
LOCAL_TEST_PATH=./test
EXECUTABLE=main.bin

# Includes
LOCAL_INCLUDE_PATH=./include
LOCAL_INCLUDE_FILES = $(LOCAL_INCLUDE_PATH)/realgen.h $(LOCAL_INCLUDE_PATH)/stat.h

default: all

all: dir lib_a lib_so test

# Create obj and lib dir
dir:
	(test -d $(LOCAL_LIB_PATH) || mkdir -p $(LOCAL_LIB_PATH))
	(test -d $(LOCAL_OBJ_PATH) || mkdir -p $(LOCAL_OBJ_PATH))

# Compiling
$(LOCAL_OBJ_PATH)/%.o: ./src/%.cpp $(LOCAL_INCLUDE_FILES)
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
test: $(LOCAL_TEST_PATH)/$(EXECUTABLE)

test/$(EXECUTABLE): $(LOCAL_TEST_PATH)/*.cpp $(LOCAL_LIB_PATH)/$(LIB_A_TARGET) $(LOCAL_INCLUDE_FILES)
	$(CPP) $(CPPFLAGS) -o $(LOCAL_TEST_PATH)/$(EXECUTABLE) $(LOCAL_TEST_PATH)/*.cpp -I$(LOCAL_INCLUDE_PATH) $(LOCAL_LIB_PATH)/$(LIB_A_TARGET)
	@echo "****************** Test: $(EXECUTABLE) [OK] ******************"


# Clean
clean:
ifneq ($(LOCAL_OBJ_PATH),/)
	(test -d $(LOCAL_OBJ_PATH) && rm -fr $(LOCAL_OBJ_PATH) || true)
endif
ifneq ($(LOCAL_LIB_PATH),/)
	(test -d $(LOCAL_LIB_PATH) && rm -fr $(LOCAL_LIB_PATH) || true)
endif
	(test $(LOCAL_TEST_PATH)/$(EXECUTABLE) && rm $(LOCAL_TEST_PATH)/$(EXECUTABLE) || true)
	@echo "****************** Clean [OK] ******************"
	

.PHONY: clean
