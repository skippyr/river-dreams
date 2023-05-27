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

source "${RIVER_DREAMS_DIRECTORY_PATH}/river_dreams_fallback.zsh-theme"

function river_dreams::compile
{
	cargo build --release --manifest-path "${RIVER_DREAMS_MANIFEST_FILE_PATH}" ||
	(
		rm -rf "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}"
		echo ""
		echo "Opsie!"
		echo "\tprogram: river_dreams."
		echo "\tdescription: could not compile the source files."
		echo "\tsuggestion:"
		echo "\t\t* Ensure that you have installed all the required dependencies to compile them."
		echo "\t\tYou can find more information about it in the README.md file."
		echo "\t\t* Ensure that you are connected to the internet when downloading the dependencies crates."
		echo "\t\t* If you have modified the source code, ensure that it does not contain any syntax error."
		echo "\t\tRead the instructions given above by Cargo to track the issue easily."
	)
}

function river_dreams::toggle_fallback_text
{
	if [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} -eq "0" ]]; then
		export RIVER_DREAMS_USE_FALLBACK_TEXT="1"
	else
		export RIVER_DREAMS_USE_FALLBACK_TEXT="0"
	fi
}

if [[
	! -f "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/left_prompt" ||
	! -f "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/right_prompt"
]]; then
	echo "Were not possible to find the required binaries files for River Dreams."
	echo "Trying to automatically compile the source files using cargo."
	echo ""
	echo "This process may take some time depending of your internet connection."
	echo ""
	river_dreams::compile
fi
if [[
	-f "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/left_prompt" &&
	-f "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/right_prompt"
]]; then
	PROMPT='$("${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/left_prompt")'
	RPROMPT='$("${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/right_prompt")'
else
	echo ""
	echo "Attention: you are running River Dreams using a fallback prompt with limited functionalities."
	echo "Solve previous related errors to use the main prompt."
fi

