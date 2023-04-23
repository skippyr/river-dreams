#!/usr/bin/env zsh

setopt promptsubst
export VIRTUAL_ENV_DISABLE_PROMPT="1"
if [[ ! ${RIVER_DREAMS_USE_FALLBACK_TEXT} ]]; then
	if [[ $(tput colors) -eq "8" ]]; then
		export RIVER_DREAMS_USE_FALLBACK_TEXT="1"
	else
		export RIVER_DREAMS_USE_FALLBACK_TEXT="0"
	fi
fi
typeset -gr RIVER_DREAMS_ROOT_DIRECTORY="$(dirname "$0")"
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
	typeset -i compilation_has_failed="0"
	echo "\x1b[1;31mCompiling Source Files\x1b[0m"
	echo "\t\x1b[33m*\x1b[0m Created directory structure for builds."
	echo "\t\x1b[33m*\x1b[0m Compiling source files:"
	for source_file in "${source_files[@]}"; do
		echo "\t\t\x1b[33m*\x1b[0m Compiling source file \x1b[34m${source_file}.c\x1b[0m."
		gcc ${compilation_flags} "${RIVER_DREAMS_SOURCES_DIRECTORY}/${source_file}.c" -o "${RIVER_DREAMS_BUILDS_DIRECTORY}/${source_file}" ||
		compilation_has_failed="1"
		if [[ "${compilation_has_failed}" -eq "1" ]]; then
			rm -rf "${RIVER_DREAMS_BUILDS_DIRECTORY}"
			echo ""
			echo "\x1b[1;31mOpsie!\x1b[0m Compilation of source file \x1b[34m${source_file}.c\x1b[0m has \x1b[34mfailed\x1b[0m."
			echo "Please, try the following:"
			echo "\t\x1b[33m*\x1b[0m Ensure that have installed all the required dependencies."
			echo "\t\x1b[33m*\x1b[0m If you have modified the source code, verify the error message of gcc."
			echo "After following these instructions, restart your ZSH session to try to compile again."
			echo ""
			echo "If you can not find a reason for this issue, file an issue on GitHub:"
			echo "\t\x1b[34mhttps://github.com/skippyr/river_dreams\x1b[0m"
			echo "Remember to post this output there to help track this issue."
			break
		fi
	done
	if [[ ${compilation_has_failed} -eq "0" ]]; then
		echo "Builds were stored at \x1b[34m${RIVER_DREAMS_BUILDS_DIRECTORY}\x1b[0m."
	fi
}

if [[ ! -d "${RIVER_DREAMS_BUILDS_DIRECTORY}" ]]; then
	river_dreams::compile_source_files
fi
if [[ -d "${RIVER_DREAMS_BUILDS_DIRECTORY}" ]]; then
	PROMPT='$("${RIVER_DREAMS_BUILDS_DIRECTORY}/left_prompt") '
	RPROMPT='$("${RIVER_DREAMS_BUILDS_DIRECTORY}/right_prompt")'
	PS2=$'\t%F{red}¦%f '
fi

