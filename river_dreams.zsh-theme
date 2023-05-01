#!/usr/bin/env zsh

typeset -g RIVER_DREAMS_DIRECTORY="$(dirname "$(realpath "$0")")"
typeset -g RIVER_DREAMS_MANIFEST_FILE="${RIVER_DREAMS_DIRECTORY}/Cargo.toml"
typeset -g RIVER_DREAMS_RELEASE_DIRECTORY="${RIVER_DREAMS_DIRECTORY}/target/release"

river_dreams::compile()
{
	cargo build -r --manifest-path "${RIVER_DREAMS_MANIFEST_FILE}"
}

[[ ! -d "${RIVER_DREAMS_RELEASE_DIRECTORY}" ]] && river_dreams::compile
if [[ -d "${RIVER_DREAMS_RELEASE_DIRECTORY}" ]]; then
	PROMPT="$("${RIVER_DREAMS_RELEASE_DIRECTORY}/left_prompt")"
fi

