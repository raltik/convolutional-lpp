CC       := mpicc
CFLAGS   := -fopenmp -O0
LDFLAGS  := -L/usr/lib -lstdc++ -lm
OBJ_DIR  := ./obj
APP_DIR  := .
TARGET   := conv
INCLUDE  := -Iinclude/ 
SRC      := $(wildcard src/*.c)

OBJECTS  := $(SRC:%.c=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CFLAGS += -DDEBUG -g
debug: all

release: CFLAGS += -O3
release: all

clean:
	-@rm -rvf $(OBJ_DIR)
	-@rm -rvf $(TARGET)
