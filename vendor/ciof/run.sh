#!/bin/bash

# PCANNON RUN.SH v1.0S - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20250608
# https://github.com/pcannon09/pcannonProjectStandards
# NOTES:
# - Made CIOF Distribution

set -e

PROJECT_INFO_PATH=".private/project.json"

projectName=$(jq -r '.exeName' $PROJECT_INFO_PATH)

LICENSE_STR=$(cat <<EOF
CIOF Copyright (C) 2025  pcannon09
This program comes with ABSOLUTELY NO WARRANTY; for details type \`./run.sh --license'.
This is free software, and you are welcome to redistribute it
under certain conditions; type \`./run.sh --license' for details.
EOF
)

if [[ "$1" == "GDB" ]]; then
	gdb ./build/bin/$projectName

elif [[ $1 == "--license" ]]; then
	echo "$LICENSE_STR"

else
	./build/bin/${projectName}_non_install "$@"

fi

