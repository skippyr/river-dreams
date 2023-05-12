#!/usr/bin/env zsh

setopt promptsubst

export VIRTUAL_ENV_DISABLE_PROMPT="1"
export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT:-$(
	[[ $(tput colors) -eq "8" ]] &&
	echo "1" ||
	echo "0"
)}

typeset -gr RIVER_DREAMS_DIRECTORY_PATH="$(dirname "$(realpath "$0")")"
typeset -gr RIVER_DREAMS_MANIFEST_FILE_PATH="${RIVER_DREAMS_DIRECTORY_PATH}/Cargo.toml"
typeset -gr RIVER_DREAMS_RELEASE_DIRECTORY_PATH="${RIVER_DREAMS_DIRECTORY_PATH}/target/release"

river_dreams::compile()
{ cargo build --release --manifest-path "${RIVER_DREAMS_MANIFEST_FILE_PATH}" }

if [[ ! -d "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}" ]]; then
	river_dreams::compile ||
	(
		rm -rf "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}"
		echo "Opsie!"
		echo "\tprogram: river_dreams."
		echo "\tdescription: could not compile the source files."
		echo "\tsuggestion: ensure that you have installed all the required dependencies to compile them."
		echo "\tYou can find more information about it in the README.md file."
	)
fi
if [[ -d "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}" ]]; then
	PROMPT='$("${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/left_prompt")  '
	RPROMPT='$("${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/right_prompt")'
fi

