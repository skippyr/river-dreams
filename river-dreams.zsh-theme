setopt promptsubst
export VIRTUAL_ENV_DISABLE_PROMPT=1

typeset -r RIVER_DREAMS_ROOT="$(dirname ${0})"

_river_dreams() {
  typeset -r RIVER_DREAMS_MANIFEST="${RIVER_DREAMS_ROOT}/Cargo.toml"
  typeset -r RIVER_DREAMS_BINARY="${RIVER_DREAMS_ROOT}/target/release/river-dreams"
  [[ ! -f "${RIVER_DREAMS_BINARY}" ]] &&
     cargo build -r --manifest-path "${RIVER_DREAMS_MANIFEST}"
  "${RIVER_DREAMS_BINARY}"
}

PROMPT='$(_river_dreams)'
