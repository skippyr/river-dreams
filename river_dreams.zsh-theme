#!/usr/bin/env zsh

setopt promptsubst
setopt +o nomatch

export VIRTUAL_ENV_DISABLE_PROMPT=1
export RIVER_DREAMS_REPOSITORY_DIRECTORY=$(dirname $0)
RIVER_DREAMS_DEPENDENCIES_DIRECTORY=${RIVER_DREAMS_REPOSITORY_DIRECTORY}/dependencies
RIVER_DREAMS_MODULES_DIRECTORY=${RIVER_DREAMS_REPOSITORY_DIRECTORY}/modules
export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT:-$(
  test $(tput colors) -eq 8 &&
  echo "true" ||
  echo "false"
)}
[[ $(git -C ${RIVER_DREAMS_REPOSITORY_DIRECTORY} branch 2>/dev/null) ]] &&
  export RIVER_DREAMS_VERSION="${OSTYPE}_$(git -C "${RIVER_DREAMS_REPOSITORY_DIRECTORY}" branch --show-current 2>/dev/null)_$(git -C "${RIVER_DREAMS_REPOSITORY_DIRECTORY}" rev-parse --short HEAD 2>/dev/null)" ||
  export RIVER_DREAMS_VERSION="river_dreams: could not get version. Is git installed and are you the user owner of the repository downloaded?"
export RIVER_DREAMS_USE_COLOR_VARIANTS=${RIVER_DREAMS_USE_COLOR_VARIANTS:-false}

source ${RIVER_DREAMS_DEPENDENCIES_DIRECTORY}/zsh_async/async.zsh
async_init

for file in $(
  find ${RIVER_DREAMS_MODULES_DIRECTORY} -maxdepth 1 -type f
); do
  source ${file}
done

precmd() {
  river_dreams::async::restart_worker
}

PROMPT='$(river_dreams::commands_separator)
%F{red}┌─%F{yellow}[%f${RIVER_DREAMS_TOP_PROMPT}%F{yellow}]%f
%F{red}└%f$(river_dreams::exit_code)$(river_dreams::root)$(river_dreams::vi_mode)$(river_dreams::decorator)$(river_dreams::directory)${RIVER_DREAMS_GIT}$(river_dreams::directory_ownership)%f '
RPROMPT='${RIVER_DREAMS_RIGHT_PROMPT}'
