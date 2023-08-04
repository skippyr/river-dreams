setopt promptsubst
export VIRTUAL_ENV_DISABLE_PROMPT=1

RIVER_DREAMS_ROOT=$(dirname ${0})
PROMPT='$(${RIVER_DREAMS_ROOT}/target/release/river-dreams)'
