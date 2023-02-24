#!/usr/bin/env zsh

river_dreams::python_environment::get_python_environment() {
  echo ${VIRTUAL_ENV} |
  rev |
  cut -f 1 -d / |
  rev
}

river_dreams::python_environment() {
  local -r python_environment_symbol=${RIVER_DREAMS_PYTHON_ENVIRONMENT_SYMBOL:-󰚐}
  local -r python_environment_symbol_color=${RIVER_DREAMS_PYTHON_ENVIRONMENT_SYMBOL_COLOR:-magenta}

  local -r python_environment=$(river_dreams::python_environment::get_python_environment)
  
  if [[ -n ${python_environment} ]]; then
    echo "%F{${python_environment_symbol_color}}${python_environment_symbol}%F{normal} ${python_environment}"
  fi
}