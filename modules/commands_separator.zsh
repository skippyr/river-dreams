#!/usr/bin/env zsh

river_dreams::commands_separator() {
  local -r commands_separator_symbol=${RIVER_DREAMS_COMMANDS_SEPARATOR_SYMBOL:-▽}
  local -r commands_separator_symbol_color=${RIVER_DREAMS_COMMANDS_SEPARATOR_SYMBOL_COLOR:-normal}
  
  for column in $(seq 1 ${COLUMNS}); do
    echo -n "%F{${commands_separator_symbol_color}}${commands_separator_symbol}%F{normal}"
  done
}
