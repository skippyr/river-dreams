#!/usr/bin/env zsh

river_dreams::get_hidden_files_quantity() {
  ls -d .* 2>/dev/null | wc -l
}

river_dreams::hidden_files() {
  local -r hidden_files_symbol=${RIVER_DREAMS_HIDDEN_FILES_SYMBOL:- }
  local -r hidden_files_quantity_color=${RIVER_DREAMS_HIDDEN_FILES_SYMBOL_COLOR:-red}

  local -r hidden_files_quantity=$(river_dreams::get_hidden_files_quantity)

  if [[ ${hidden_files_quantity} -gt 0 ]]; then
    echo "%F{${hidden_files_quantity_color}}${hidden_files_symbol}%F{normal}${hidden_files_quantity}"
  fi
}
