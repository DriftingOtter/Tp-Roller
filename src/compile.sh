#!/bin/bash

# Function to handle errors and cleanup
handle_error() {
    echo "Error: $1"
    cleanup
    exit 1
}

# Function to perform cleanup (remove object files)
cleanup() {
    rm -f main.o tp_transpiler.o
}

# Trap interrupts and call cleanup function
trap 'handle_error "Script interrupted"; exit 1' INT

# Check if the number of arguments is correct
if [ "$#" -ne 0 ]; then
    echo "Usage: $0"
    exit 1
fi

# Compile main.cpp and tp_transpiler.cpp
echo "Compiling source files..."
g++ -c main.cpp -o main.o || handle_error "Compilation failed"
g++ -c tp_transpiler.cpp -o tp_transpiler.o || handle_error "Compilation failed"

# Link object files and create executable named "tp_roller"
echo "Linking object files..."
g++ main.o tp_transpiler.o -o tp_roller || handle_error "Linking failed"

# Output completion message
echo "Compilation successful. Executable 'tp_roller' created."

# Remove object files
cleanup
