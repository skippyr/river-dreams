#!/usr/bin/env zsh

river_dreams::ignored_files::get_ignored_files_quantity() {
  git status -s --ignored 2>/dev/null |
  grep "! " |
  wc -l
}

river_dreams::ignored_files() {
  local -r ignored_files_symbol=${RIVER_DREAMS_IGNORED_FILES_SYMBOL:-󰮀 }
  local -r ignored_files_symbol_color=${RIVER_DREAMS_IGNORED_FILES_SYMBOL_COLOR:-magenta}

  local -r ignored_files_quantity=$(river_dreams::ignored_files::get_ignored_files_quantity)

  if [[ ${ignored_files_quantity} -gt 0 ]]; then
    echo "%F{${ignored_files_symbol_color}}${ignored_files_symbol}%F{normal}${ignored_files_quantity}"
  fi
}
