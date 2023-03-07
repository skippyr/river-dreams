#!/usr/bin/env zsh

river_dreams::hidden_files() {
  local -r hidden_files_quantity=$(ls -d .* 2>/dev/null | wc -l)

  if [[ ${hidden_files_quantity} -gt 0 ]]; then
    echo "%F{red} %f${hidden_files_quantity}"
  fi
}
