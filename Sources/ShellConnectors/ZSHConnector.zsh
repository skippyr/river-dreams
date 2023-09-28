setopt promptsubst

export VIRTUAL_ENV_DISABLE_PROMPT="1"
export RIVER_DREAMS_BINARIES_DIRECTORY="$(dirname $(dirname $(dirname ${0})))/Build/Binaries"

PROMPT='$(${RIVER_DREAMS_BINARIES_DIRECTORY}/LeftPrompt)'
RPROMPT='$(${RIVER_DREAMS_BINARIES_DIRECTORY}/RightPrompt)'
