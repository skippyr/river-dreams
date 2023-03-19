#!/usr/bin/env zsh

river_dreams::ignored_files() {
  local -r ignored_files_quantity=$(
    git status -sb --ignored 2>/dev/null | grep "! " | wc -l
  )
  if [[ ${ignored_files_quantity} -gt 0 ]]; then
    local -r ignored_files_symbol=$(
      [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
      echo "IGNORED" ||
      echo "󰮁"
    )
    echo "%F{magenta}${ignored_files_symbol} %f${ignored_files_quantity}"
  fi
}

