export VIRTUAL_ENV_DISABLE_PROMPT=1

setopt promptsubst

__RIVER_DREAMS_ROOT_DIRECTORY=$(dirname ${0})
PROMPT='$(${__RIVER_DREAMS_ROOT_DIRECTORY}/binaries/left-prompt)'
RPROMPT='$(${__RIVER_DREAMS_ROOT_DIRECTORY}/binaries/right-prompt)'
