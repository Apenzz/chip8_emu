CC := clang 
CFLAGS := -Wall -Wextra -g -O2
SRC_DIR := ./src
BUILD_DIR := ./build
TARGET_EX := chip8_emu

# source files
SRCS := $(shell find $(SRC_DIR) -name '*.c')
# object files
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# final build
$(BUILD_DIR)/$(TARGET_EX): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# build object files from C sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)	
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: print
print:
	echo $(SRCS)
	echo $(OBJS)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
