CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -I./libmx/inc -I./inc -g

SOURCE_FILES = *.c
OBJECT_FILES = *.o
OBJECT_DIR = ./obj
SOURCE_DIR = ./src

LIBMX_DIR = ./libmx
LIBMX_FILE = libmx.a

SRC_PATH = $(wildcard $(SOURCE_DIR)/$(SOURCE_FILES))
OUTPUT_FILE = uls

all: clean obj ${OUTPUT_FILE}

obj:
	if [ ! -d $(OBJECT_DIR) ]; then mkdir $(OBJECT_DIR); fi
	$(CC) $(CFLAGS) -c $(SRC_PATH)
	mv $(OBJECT_FILES) $(OBJECT_DIR)

${OUTPUT_FILE}:
	@make -sC $(LIBMX_DIR)
	$(CC) $(CFLAGS) -o $(OUTPUT_FILE) $(OBJECT_DIR)/$(OBJECT_FILES) $(LIBMX_DIR)/$(LIBMX_FILE) # delete -g
	# ar -cq $(OUTPUT_FILE) $(OBJECT_DIR)/*.o

uninstall:
	@make -sC $(LIBMX_DIR) $@
	if [ -d $(OBJECT_DIR) ]; then rm -rf $(OBJECT_DIR); fi
	if [ -f $(OUTPUT_FILE) ]; then rm $(OUTPUT_FILE); fi
	if [ -f *.o ]; then rm *.o; fi

clean:
	if [ -d $(OBJECT_DIR) ]; then rm -rf $(OBJECT_DIR); fi

reinstall: uninstall all