#!/usr/bin/env zsh

river_dreams::python_environment() {
  local -r python_environment=$(
    echo ${VIRTUAL_ENV} |
    rev |
    cut -f 1 -d / |
    rev
  )
  local -r python_environment_symbol=$(
    test ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true &&
    echo "ENV" ||
    echo "󰚐"
  )
  
  if [[ -n ${python_environment} ]]; then
    echo "%F{red}${python_environment_symbol} %f${python_environment}"
  fi
}
