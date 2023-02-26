#!/usr/bin/env zsh

river_dreams::ignored_files() {
  local -r ignored_files_quantity=$(
    git status -s --ignored 2>/dev/null |
    grep "! " |
    wc -l
  )

  if [[ ${ignored_files_quantity} -gt 0 ]]; then
    echo "%F{magenta}󰮀 %F{normal}${ignored_files_quantity}"
  fi
}
