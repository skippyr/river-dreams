#!/usr/bin/env zsh

river_dreams::executable_files() {
  local -r executable_files_quantity=$(
    find -maxdepth 1 -executable -type f |
    wc -l
  )
  [[ ${executable_files_quantity} -eq 0 ]] && exit
  local -r executable_files_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
      echo "EXECUTABLE" ||
      echo ""
  )
  echo "%F{green}${executable_files_symbol} %f${executable_files_quantity}"
}
