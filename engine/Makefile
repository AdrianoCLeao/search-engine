CC = gcc
CFLAGS = -Wall -Iinclude -fPIC -m64 -D_WIN64
SRC_DIR = src
BUILD_DIR = bin
INCLUDE_DIR = include

SRCS := $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
STATIC_LIB := $(BUILD_DIR)/libtfidf.a
SHARED_LIB := $(BUILD_DIR)/libtfidf.so
EXECUTABLE := $(BUILD_DIR)/main.exe

ifeq ($(OS),Windows_NT)
    MKDIR_P = if not exist "$(subst /,\\,$(strip $(1)))" mkdir "$(subst /,\\,$(strip $(1)))"
    RMDIR = rmdir /s /q
else
    MKDIR_P = mkdir -p $(1)
    RMDIR = rm -rf
endif

all: $(STATIC_LIB) $(SHARED_LIB)

$(STATIC_LIB): $(OBJS)
	$(call MKDIR_P, $(BUILD_DIR))
	ar rcs $@ $^

$(SHARED_LIB): $(OBJS)
	$(call MKDIR_P, $(BUILD_DIR))
	$(CC) -shared -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(call MKDIR_P, $(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJS)
	$(call MKDIR_P, $(BUILD_DIR))
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RMDIR) $(BUILD_DIR)

run: $(EXECUTABLE)
	$(EXECUTABLE)

.PHONY: all clean run
