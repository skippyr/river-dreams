#!/usr/bin/env zsh

river_dreams::directory_ownership::get_directory_owner() {
  ls -ld | awk '{print $3}'
}

river_dreams::directory_ownership() {
  local -r directory_ownership_symbol=${RIVER_DREAMS_DIRECTORY_OWNERSHIP_SYMBOL:- }
  local -r directory_ownership_symbol_color=${RIVER_DREAMS_DIRECTORY_OWNERSHIP_SYMBOL_COLOR:-normal}

  local -r user=$(whoami)
  local -r directory_owner=$(river_dreams::directory_ownership::get_directory_owner)

  if [[ ${user} != root && ${user} != ${directory_owner} ]]; then
    echo "%F{${directory_ownership_symbol_color}}${directory_ownership_symbol}%F{normal}"
  fi
}
