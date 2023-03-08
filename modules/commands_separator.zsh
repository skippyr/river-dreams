#!/usr/bin/env zsh

river_dreams::commands_separator() {
  if [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]]; then
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
