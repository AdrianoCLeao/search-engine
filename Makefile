CC = gcc
CFLAGS = -Wall -Iengine/include -fPIC -m64 -D_WIN64
SRC_DIR = engine/src
BUILD_DIR = engine/bin
INCLUDE_DIR = engine/include
GUI_DIR = gui
RUST_TARGET := $(GUI_DIR)/target/debug/gui.exe

SRCS := $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
EXECUTABLE := $(BUILD_DIR)/main.exe
STATIC_LIB := $(BUILD_DIR)/libtfidf.a
SHARED_LIB := $(BUILD_DIR)/libtfidf.so

ifeq ($(OS),Windows_NT)
    MKDIR_P = if not exist "$(subst /,\\,$(strip $(1)))" mkdir "$(subst /,\\,$(strip $(1)))"
    RMDIR = del /s /q "$(subst /,\\,$(strip $(1)))" & rmdir /s /q "$(subst /,\\,$(strip $(1)))"
    RUN_RUST = $(RUST_TARGET)
else
    MKDIR_P = mkdir -p $(1)
    RMDIR = rm -rf $(1)
    RUN_RUST = ./$(RUST_TARGET)
endif

all: $(STATIC_LIB) $(SHARED_LIB) $(EXECUTABLE) $(RUST_TARGET)

$(STATIC_LIB): $(OBJS)
	$(call MKDIR_P, $(BUILD_DIR))
	ar rcs $@ $^

$(SHARED_LIB): $(OBJS)
	$(call MKDIR_P, $(BUILD_DIR))
	$(CC) -shared -o $@ $^

$(EXECUTABLE): $(OBJS)
	$(call MKDIR_P, $(BUILD_DIR))
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(call MKDIR_P, $(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

$(RUST_TARGET):
	@cd $(GUI_DIR) && cargo build

clean:
	$(call RMDIR, $(BUILD_DIR))
	@cd $(GUI_DIR) && cargo clean

run: 
	@cd $(GUI_DIR) && cargo run

run-c: $(EXECUTABLE)
	$(EXECUTABLE)

.PHONY: all clean run run-c
