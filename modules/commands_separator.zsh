#!/usr/bin/env zsh

river_dreams::commands_separator() {
  for column in $(seq 1 $((${COLUMNS} / 2))); do
    echo -n "▽▼"
  done

  if [[ ! $((${COLUMNS} % 2)) -eq 0 ]]; then
    echo -n ▽
  fi
}
