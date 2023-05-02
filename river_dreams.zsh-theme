#!/usr/bin/env zsh

setopt promptsubst

typeset -gr RIVER_DREAMS_DIRECTORY="$(dirname "$(realpath "$0")")"
typeset -gr RIVER_DREAMS_RELEASE_DIRECTORY="${RIVER_DREAMS_DIRECTORY}/target/release"

if [[ -d "${RIVER_DREAMS_RELEASE_DIRECTORY}" ]]; then
	PROMPT='$("${RIVER_DREAMS_RELEASE_DIRECTORY}/left_prompt") '
fi

