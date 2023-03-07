#!/usr/bin/env zsh

river_dreams::commands_separator() {
  if [[ $(tput colors) -eq 8 ]]; then
    for column in $(seq 1 $((${COLUMNS}))); do
      echo -n "v"
    done
  else
    for column in $(seq 1 $((${COLUMNS} / 2))); do
      echo -n "▽▼"
    done

    if [[ ! $((${COLUMNS} % 2)) -eq 0 ]]; then
      echo -n ▽
    fi
  fi
}
