#!/bin/bash

# PCANNON OPEN.SH v1.0S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20250608
# https://github.com/pcannon09/pcannonProjectStandards

defaultEdit="$1" # Set the default editor in parameter $1 (such as: code, vim, nvim, ...)

if [ "$1" == "" ]; then
	defaultEdit="nvim"
fi

if [ -f "src/main.c*" ]; then
	$defaultEdit src/main.c* $(find src/ inc/ -type f) CMakeLists.txt

	exit

else
	if [ -d "./test/" ]; then
		if [ ! -d "./test/src" ]; then
			$defaultEdit test/main.c* $(find test/ src/ inc/ -type f) CMakeLists.txt

		else
			$defaultEdit test/src/main.c* $(find test/src test/inc test/ src/ inc/ -type f) CMakeLists.txt
		fi

		exit

	elif [ -d "./tests/" ]; then
		if [ ! -d "./tests/src" ]; then
			$defaultEdit tests/main.c* $(find tests/ src/ inc/ -type f) CMakeLists.txt

		else
			$defaultEdit tests/src/main.c* $(find tests/src tests/inc tests/ src/ inc/ -type f) CMakeLists.txt
		fi

		exit
	fi
fi

# DEFAULT
$defaultEdit src/main.c* $(find src/ inc/ -type f) CMakeLists.txt

