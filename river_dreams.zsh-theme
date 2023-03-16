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
export RIVER_DREAMS_VERSION="$(git -C "${RIVER_DREAMS_REPOSITORY_DIRECTORY}" branch --show-current)_$(git -C "${RIVER_DREAMS_REPOSITORY_DIRECTORY}" rev-parse --short HEAD)"

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
    river_dreams::git)
      RIVER_DREAMS_GIT=${output}
      ;;
    river_dreams::directory_ownership)
      RIVER_DREAMS_DIRECTORY_OWNERSHIP=${output}
      ;;
    river_dreams::hidden_files)
      RIVER_DREAMS_HIDDEN_FILES=${output}
      ;;
    river_dreams::executable_files)
      RIVER_DREAMS_EXECUTABLE_FILES=${output}
      ;;
    river_dreams::symbolic_links)
      RIVER_DREAMS_SYMBOLIC_LINKS=${output}
      ;;
    river_dreams::ignored_files)
      RIVER_DREAMS_IGNORED_FILES=${output}
      ;;
    river_dreams::time_elapsed)
      RIVER_DREAMS_TIME_ELAPSED=${output}
      ;;
  esac
  RIVER_DREAMS_RIGHT_PROMPT=(
    ${RIVER_DREAMS_HIDDEN_FILES}
    ${RIVER_DREAMS_EXECUTABLE_FILES}
    ${RIVER_DREAMS_SYMBOLIC_LINKS}
    ${RIVER_DREAMS_IGNORED_FILES}
    ${RIVER_DREAMS_TIME_ELAPSED}
  )
  zle reset-prompt
}

river_dreams::async::restart_worker() {
  RIVER_DREAMS_GIT=""
  RIVER_DREAMS_RIGHT_PROMPT=""

  async_start_worker RIVER_DREAMS_ASYNC_WORKER -n
  async_flush_jobs RIVER_DREAMS_ASYNC_WORKER
  async_worker_eval RIVER_DREAMS_ASYNC_WORKER cd ${PWD}
  async_worker_eval RIVER_DREAMS_ASYNC_WORKER export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT}
  async_register_callback RIVER_DREAMS_ASYNC_WORKER river_dreams::async::callback

  # Left Prompt Components
  async_job RIVER_DREAMS_ASYNC_WORKER river_dreams::git

  # Right Prompt Components
  async_job RIVER_DREAMS_ASYNC_WORKER river_dreams::hidden_files
  async_job RIVER_DREAMS_ASYNC_WORKER river_dreams::executable_files
  async_job RIVER_DREAMS_ASYNC_WORKER river_dreams::symbolic_links
  async_job RIVER_DREAMS_ASYNC_WORKER river_dreams::ignored_files
  async_job RIVER_DREAMS_ASYNC_WORKER river_dreams::time_elapsed
}

precmd() {
  river_dreams::async::restart_worker
}

PROMPT='$(river_dreams::exit_code)$(river_dreams::root)$(river_dreams::vi_mode)$(river_dreams::decorator)$(river_dreams::directory)${RIVER_DREAMS_GIT}$(river_dreams::directory_ownership)%f '
RPROMPT='${RIVER_DREAMS_RIGHT_PROMPT}'

