setopt promptsubst
export VIRTUAL_ENV_DISABLE_PROMPT=1

typeset -r _river_dreams_root="$(dirname ${0})"

_river_dreams() {
  typeset -r manifest="${_river_dreams_root}/Cargo.toml"
  typeset -r binary="${_river_dreams_root}/target/release/river-dreams"
  [[ ! -f "${binary}" ]] &&
    cargo build -r --manifest-path "${manifest}"
  "${binary}"
}

PROMPT='$(_river_dreams)'
