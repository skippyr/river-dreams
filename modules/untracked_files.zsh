#!/usr/bin/env zsh

river_dreams::untracked_files() {
  local -r untracked_files_quantity=$(
    git status -s --ignored 2>/dev/null |
    grep "! " |
    wc -l
  )
  if [[ ${untracked_files_quantity} -gt 0 ]]; then
    echo "%F{magenta}󰮀 %F{normal}${untracked_files_quantity}"
  fi
}
