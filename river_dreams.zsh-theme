#!/usr/bin/env zsh

setopt promptsubst

export VIRTUAL_ENV_DISABLE_PROMPT="1"
export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT:-$(
	[[ $(tput colors) -eq "8" ]] &&
	echo "1" ||
	echo "0"
)}

typeset -gr RIVER_DREAMS_DIRECTORY="$(dirname "$(realpath "$0")")"
typeset -gr RIVER_DREAMS_MANIFEST_FILE="${RIVER_DREAMS_DIRECTORY}/Cargo.toml"
typeset -gr RIVER_DREAMS_RELEASE_DIRECTORY="${RIVER_DREAMS_DIRECTORY}/target/release"

river_dreams::compile()
{ cargo build -r --manifest-path "${RIVER_DREAMS_MANIFEST_FILE}" }

if [[ ! -d "${RIVER_DREAMS_RELEASE_DIRECTORY}" ]]; then
	river_dreams::compile ||
	(
		rm -rf "${RIVER_DREAMS_RELEASE_DIRECTORY}"
		echo "Opsie!"
		echo "\tprogram: river_dreams."
		echo "\tdescription: could not compile the source files."
		echo "\tsuggestion: ensure that you have installed all the required dependencies to compile them."
		echo "\tYou can find more information about it in the README.md file."
	)
fi
if [[ -d "${RIVER_DREAMS_RELEASE_DIRECTORY}" ]]; then
	PROMPT='$("${RIVER_DREAMS_RELEASE_DIRECTORY}/left_prompt")'
	RPROMPT='$("${RIVER_DREAMS_RELEASE_DIRECTORY}/right_prompt")'
fi

