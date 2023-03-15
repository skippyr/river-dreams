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

river_dreams::async::callback() {
  local -r function_name="$1"
  local -r output="$3"
  case ${function_name} in
    river_dreams::directory)
      RIVER_DREAMS_DIRECTORY=${output}
      ;;
    river_dreams::decorator)
      RIVER_DREAMS_DECORATOR=${output}
      ;;
    river_dreams::git)
      RIVER_DREAMS_GIT=${output}
      ;;
    river_dreams::directory_ownership)
      RIVER_DREAMS_DIRECTORY_OWNERSHIP=${output}
      ;;
  esac
  zle reset-prompt
}

river_dreams::async::restart_worker() {
  async_start_worker RIVER_DREAMS_ASYNC_WORKER -n
  async_flush_jobs RIVER_DREAMS_ASYNC_WORKER
  async_worker_eval RIVER_DREAMS_ASYNC_WORKER cd ${PWD}

  async_register_callback RIVER_DREAMS_ASYNC_WORKER river_dreams::async::callback
  async_job RIVER_DREAMS_ASYNC_WORKER river_dreams::directory
  async_job RIVER_DREAMS_ASYNC_WORKER river_dreams::decorator
  async_job RIVER_DREAMS_ASYNC_WORKER river_dreams::git
  async_job RIVER_DREAMS_ASYNC_WORKER river_dreams::directory_ownership
}

precmd() {
  river_dreams::async::restart_worker
}

PROMPT='${RIVER_DREAMS_DECORATOR}${RIVER_DREAMS_DIRECTORY}${RIVER_DREAMS_GIT}${RIVER_DREAMS_DIRECTORY_OWNERSHIP}%f '

