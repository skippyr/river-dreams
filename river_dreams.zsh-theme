#!/usr/bin/env zsh

setopt promptsubst

export VIRTUAL_ENV_DISABLE_PROMPT="1"
export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT:-$(
	[[ $(tput colors) -eq "8" ]] &&
	echo "1" ||
	echo "0"
)}

typeset -gr RIVER_DREAMS_DIRECTORY="$(dirname "$(realpath "$0")")"
typeset -gr RIVER_DREAMS_RELEASE_DIRECTORY="${RIVER_DREAMS_DIRECTORY}/target/release"

if [[ -d "${RIVER_DREAMS_RELEASE_DIRECTORY}" ]]; then
	PROMPT='$("${RIVER_DREAMS_RELEASE_DIRECTORY}/left_prompt") '
fi

