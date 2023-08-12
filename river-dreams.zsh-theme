export VIRTUAL_ENV_DISABLE_PROMPT=1

typeset -gr __RIVER_DREAMS_ROOT="$(dirname ${0})"

setopt promptsubst

function __River_Dreams()
{
	typeset -r manifest="${__RIVER_DREAMS_ROOT}/Cargo.toml"
	typeset -r binary="${__RIVER_DREAMS_ROOT}/target/release/river-dreams"
	[[ ! -f "${binary}" ]] && cargo build -r --manifest-path "${manifest}"
	"${binary}"
}

PROMPT='$(__River_Dreams)'
