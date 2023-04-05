#!/usr/bin/env zsh

setopt promptsubst

export VIRTUAL_ENV_DISABLE_PROMPT=1
export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT:-$(
	[[ $(tput colors) -eq 8 ]] &&
	echo 1 ||
	echo 0
)}

typeset -gr RIVER_DREAMS_ROOT_DIRECTORY=$(dirname $0)
typeset -gr RIVER_DREAMS_SOURCES_DIRECTORY=${RIVER_DREAMS_ROOT_DIRECTORY}/sources
typeset -gr RIVER_DREAMS_BUILDS_DIRECTORY=${RIVER_DREAMS_ROOT_DIRECTORY}/builds

river_dreams::recompile() {
	rm -rf ${RIVER_DREAMS_BUILDS_DIRECTORY}
	mkdir -p ${RIVER_DREAMS_BUILDS_DIRECTORY}

	gcc ${RIVER_DREAMS_SOURCES_DIRECTORY}/left_prompt.c -o\
	${RIVER_DREAMS_BUILDS_DIRECTORY}/left_prompt

	gcc ${RIVER_DREAMS_SOURCES_DIRECTORY}/right_prompt.c -o\
	${RIVER_DREAMS_BUILDS_DIRECTORY}/right_prompt
}

river_dreams::execute() {
	${RIVER_DREAMS_BUILDS_DIRECTORY}/$@
}

[[ ! -d ${RIVER_DREAMS_BUILDS_DIRECTORY} ]] && river_dreams::recompile

PROMPT='$(river_dreams::execute left_prompt) '
RPROMPT='$(river_dreams::execute right_prompt)'

