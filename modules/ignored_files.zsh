#!/usr/bin/env zsh

river_dreams::ignored_files() {
  local -r ignored_files_quantity=$(
    git clean -ndX 2>/dev/null | wc -l
  )
  [[ ${ignored_files_quantity} -eq 0 ]] && exit
  local -r ignored_files_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    echo "IGNORED" ||
    echo "󰮁"
  )
  echo "%F{magenta}${ignored_files_symbol} %f${ignored_files_quantity}"
}
