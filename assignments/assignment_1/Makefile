CC = gcc
CFLAGS = -Wall -Wextra

# Directories
EXAM1_DIR = exam1
EXAM2_DIR = exam2

# Default target builds all exams
all: exam1 exam2

# Exam1 targets - Static Library
exam1:
	@echo "Building exam1 (static library)..."
	@cd $(EXAM1_DIR) && \
	$(CC) $(CFLAGS) -c *.c && \
	ar rcs libhello.a hello.o && \
	$(CC) $(CFLAGS) main.o -L. -lhello -o exam1

# Exam2 targets - Dynamic Library
exam2:
	@echo "Building exam2 (dynamic library)..."
	@cd $(EXAM2_DIR) && \
	$(CC) $(CFLAGS) -fPIC -c hello.c && \
	$(CC) -shared -o libhello.so hello.o && \
	$(CC) $(CFLAGS) main.c -L. -lhello -Wl,-rpath,'$$ORIGIN' -o exam2

# Clean all exams
clean:
	@echo "Cleaning all exams..."
	@if [ -d $(EXAM1_DIR) ]; then \
		rm -f $(EXAM1_DIR)/exam1 $(EXAM1_DIR)/*.o $(EXAM1_DIR)/*.a; \
	fi
	@if [ -d $(EXAM2_DIR) ]; then \
		rm -f $(EXAM2_DIR)/exam2 $(EXAM2_DIR)/*.o $(EXAM2_DIR)/*.so; \
	fi

# Clean and rebuild all
rebuild: clean all

.PHONY: all exam1 exam2 clean rebuild
