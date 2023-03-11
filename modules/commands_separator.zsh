#!/usr/bin/env zsh

river_dreams::commands_separator() {
  local commands_separator_symbol=▽
  if [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]]; then
    commands_separator_symbol=v
  fi    
  for column in $(seq 1 ${COLUMNS}); do
    echo -n ${commands_separator_symbol}
  done
}
