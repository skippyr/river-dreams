#!/usr/bin/env zsh

# ZSH options

setopt promptsubst
setopt +o nomatch

# Exports

export VIRTUAL_ENV_DISABLE_PROMPT=1
export RIVER_DREAMS_REPOSITORY_DIRECTORY=$(dirname $0)
RIVER_DREAMS_DEPENDENCIES_DIRECTORY=${RIVER_DREAMS_REPOSITORY_DIRECTORY}/dependencies
RIVER_DREAMS_MODULES_DIRECTORY=${RIVER_DREAMS_REPOSITORY_DIRECTORY}/modules
export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT:-$(
  test $(tput colors) -eq 8 &&
  echo "true" ||
  echo "false"
)}
export RIVER_DREAMS_USE_COLOR_VARIANTS=${RIVER_DREAMS_USE_COLOR_VARIANTS:-false}

# Dependencies

source ${RIVER_DREAMS_DEPENDENCIES_DIRECTORY}/zsh_async/async.zsh
async_init

# Modules

for file in $(find ${RIVER_DREAMS_MODULES_DIRECTORY} -maxdepth 1 -type f); do
  source ${file}
done

# Prompt Definition

precmd() {
  river_dreams::async::restart_worker
}

PROMPT='$(river_dreams::commands_separator)
%F{red}┌─%F{yellow}[%f${RIVER_DREAMS_TOP_PROMPT}%F{yellow}]%f
%F{red}└%f$(river_dreams::exit_code)$(river_dreams::root)$(river_dreams::vi_mode)$(river_dreams::decorator)$(river_dreams::directory)${RIVER_DREAMS_GIT}$(river_dreams::directory_ownership)%f '
RPROMPT='${RIVER_DREAMS_RIGHT_PROMPT}'

