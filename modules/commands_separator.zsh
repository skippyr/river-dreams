#!/usr/bin/env zsh

river_dreams::commands_separator() {
  local -r commands_separator_symbol=${RIVER_DREAMS_COMMANDS_SEPARATOR_SYMBOL:-▽}
  
  for column in $(seq 1 ${COLUMNS}); do
    echo -n ${commands_separator_symbol}
  done
}
