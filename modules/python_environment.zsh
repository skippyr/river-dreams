#!/usr/bin/env zsh
river_dreams::python_environment() {
  if [[ -n ${VIRTUAL_ENV} ]]; then
    local -r python_environment=$(
      echo ${VIRTUAL_ENV} |
      rev |
      cut -f 1 -d / |
      rev
    )
    echo "%F{red}%F{normal} ${python_environment}"
  fi
}