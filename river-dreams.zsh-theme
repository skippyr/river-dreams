export VIRTUAL_ENV_DISABLE_PROMPT=1
export RIVER_DREAMS_ROOT_DIRECTORY=$(dirname ${0})

setopt promptsubst

PROMPT='$(${RIVER_DREAMS_ROOT_DIRECTORY}/binaries/left-prompt)'
RPROMPT='$(${RIVER_DREAMS_ROOT_DIRECTORY}/binaries/right-prompt)'
