#!/usr/bin/env zsh

river_dreams::directory_ownership() {
  local -r user=$(whoami)
  local -r directory_owner=$(ls -ld | awk '{print $3}')

  [[ ${user} == root || ${user} == ${directory_owner} ]] && exit

  local -r directory_ownership_symbol=$(
    test ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true &&
    echo "*" ||
    echo ""
  )
  echo "%F{red} ${directory_ownership_symbol}%f"
}
