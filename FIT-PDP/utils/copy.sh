#!/bin/bash

# Root directory
pdp_directory="/Users/Luke/Development/FIT/PDP"

# Utils directory
utils_directory="$pdp_directory/utils"

# Target directory
directory=$1

# Files to copy
files=( "connect.sh" "upload.sh" "test.sh" )

# Iterate over files to copy
for file in ${files[*]}
do
	# Create symbolic link to the target directory
	ln -s "$utils_directory/$file" "$pdp_directory/$directory/$file"
	
done
