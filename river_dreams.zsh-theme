#!/usr/bin/env zsh

setopt promptsubst
setopt +o nomatch

export VIRTUAL_ENV_DISABLE_PROMPT=1
export RIVER_DREAMS_REPOSITORY_DIRECTORY=$(dirname $0)
RIVER_DREAMS_DEPENDENCIES_DIRECTORY=${RIVER_DREAMS_REPOSITORY_DIRECTORY}/dependencies
RIVER_DREAMS_MODULES_DIRECTORY=${RIVER_DREAMS_REPOSITORY_DIRECTORY}/modules
RIVER_DREAMS_PROMPT_COMPONENTS_DIRECTORY=${RIVER_DREAMS_REPOSITORY_DIRECTORY}/prompt_components
export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT:-$(
  test $(tput colors) -eq 8 &&
  echo "true" ||
  echo "false"
)}

source ${RIVER_DREAMS_DEPENDENCIES_DIRECTORY}/zsh_async/async.zsh
async_init

for file in $(
  find ${RIVER_DREAMS_MODULES_DIRECTORY}\
  ${RIVER_DREAMS_PROMPT_COMPONENTS_DIRECTORY} -maxdepth 1 -type f
); do
  source ${file}
done

PROMPT='$(river_dreams::commands_separator)$(river_dreams::top_prompt_component)
$(river_dreams::left_prompt_component)%f '
RPROMPT='$(river_dreams::right_prompt_component)'

