# Slit a file into 64kb chunks in a random tmp directory
# and return the tmp directory name for use by user or host application.
#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: sh split_file path/to/file"
    exit 1
fi

if [ ! -f $1 ]; then
    echo "File $1 does not exist!"
    exit 2
fi

# Make tmp dir
TMP_DIR=`mktemp -d`

# Split the target file into the new directory
split -b 64k $1 $TMP_DIR/

# Echo for possible capture by host application or user.
echo $TMP_DIR

exit 0

