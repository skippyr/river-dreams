#!/usr/bin/env zsh

river_dreams::hidden_files() {
  local -r hidden_files_quantity=$(ls -d .* 2>/dev/null | wc -l)

  [[ ${hidden_files_quantity} -eq 0 ]] && exit

  local -r hidden_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    echo "HIDDEN" ||
    echo ""
  )

  echo "%F{red}${hidden_symbol} %f${hidden_files_quantity}"
}
