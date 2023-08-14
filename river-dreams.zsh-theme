export VIRTUAL_ENV_DISABLE_PROMPT=1

typeset -gr __RIVER_DREAMS_DIRECTORY=$(dirname "${0}")

setopt promptsubst

function __River_Dreams() {
    typeset -r manifest="${__RIVER_DREAMS_DIRECTORY}/Cargo.toml"
    typeset -r binary="${__RIVER_DREAMS_DIRECTORY}/target/release/river-dreams"
    [[ ! -f "${binary}" ]] && cargo build -r --manifest-path "${manifest}"
    "${binary}"
}

PROMPT='$(__River_Dreams)'
