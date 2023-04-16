#!/usr/bin/env zsh

setopt promptsubst

export VIRTUAL_ENV_DISABLE_PROMPT="1"
export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT:-$(
	[[ $(tput colors) -eq 8 ]] &&
	echo "1" ||
	echo "0"
)}

typeset -gr RIVER_DREAMS_ROOT_DIRECTORY="$(dirname $0)"
typeset -gr RIVER_DREAMS_SOURCES_DIRECTORY="${RIVER_DREAMS_ROOT_DIRECTORY}/sources"
typeset -gr RIVER_DREAMS_BUILDS_DIRECTORY="${RIVER_DREAMS_ROOT_DIRECTORY}/builds"

river_dreams::compile_source_files()
{
	rm -rf "${RIVER_DREAMS_BUILDS_DIRECTORY}"
	mkdir -p "${RIVER_DREAMS_BUILDS_DIRECTORY}"
	typeset -ra source_files=(
		"left_prompt"
		"right_prompt"
	)
	typeset -ra compilation_flags=(
		"-Wall"
		"-Wextra"
	)
	for source_file in "${source_files[@]}"; do
		gcc ${compilation_flags} "${RIVER_DREAMS_SOURCES_DIRECTORY}/${source_file}.c" -o "${RIVER_DREAMS_BUILDS_DIRECTORY}/${source_file}" ||
		(
			rm -rf "${RIVER_DREAMS_BUILDS_DIRECTORY}"
			break
		)
	done
}

[[ ! -d "${RIVER_DREAMS_BUILDS_DIRECTORY}" ]] &&
river_dreams::compile_source_files

PROMPT='$("${RIVER_DREAMS_BUILDS_DIRECTORY}/left_prompt") '
RPROMPT='$("${RIVER_DREAMS_BUILDS_DIRECTORY}/right_prompt")'
PS2='  %F{red}¦%f '

