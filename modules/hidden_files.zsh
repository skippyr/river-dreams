#!/usr/bin/env zsh

river_dreams::hidden_files() {
  local -r hidden_files_quantity=$(ls -d .* 2>/dev/null | wc -l)
  local -r hidden_symbol=$(
    test ${RIVER_DREAMS_FALLBACK_TEXT} == true &&
    echo "HIDDEN" ||
    echo ""
  )

  if [[ ${hidden_files_quantity} -gt 0 ]]; then
    echo "%F{red}${hidden_symbol} %f${hidden_files_quantity}"
  fi
}
