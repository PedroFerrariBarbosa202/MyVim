#!/bin/bash

FILE=$1
DIR=$2

SOURCE="$DIR/$FILE"
EXEC="./program"
OUTPUT="output.txt"
INPUT="input.txt"

# Check source exists
if [ ! -f "$SOURCE" ]; then
  echo "File not found: $SOURCE"
  exit 1
fi

# Compile
gcc "$SOURCE" -o "$EXEC"

if [ $? -ne 0 ]; then
  echo "Compilation failed."
  exit 1
fi

# Execute and save output
"$EXEC" <"$INPUT" >"$OUTPUT" 2>&1

echo "Finished. Output written to $OUTPUT"
