#!/usr/bin/env zsh

river_dreams::directory_ownership() {
  local -r user=$(whoami)
  local -r directory_owner=$(ls -ld | awk '{print $3}')
  if [[ ${user} != root && ${user} != ${directory_owner} ]]; then
    echo "%F{normal} "
  fi
}
