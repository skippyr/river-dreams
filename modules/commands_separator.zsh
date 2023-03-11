#!/usr/bin/env zsh

river_dreams::commands_separator() {
  local commands_separator_symbol=▽
  [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    commands_separator_symbol=v
  for column in $(seq 1 ${COLUMNS}); do
    echo -n ${commands_separator_symbol}
  done
}
