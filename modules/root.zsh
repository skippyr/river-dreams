#!/usr/bin/env zsh

river_dreams::root() {
  local -r root_symbol=${RIVER_DREAMS_ROOT_SYMBOL:-[🩑]}
  local -r root_symbol_color=${RIVER_DREAMS_ROOT_SYMBOL_COLOR:-yellow}

  local -r user=$(whoami)

  if [[ ${user} == root ]]; then
    echo "%F{${root_symbol_color}}${root_symbol}"
  fi
}
