#!/usr/bin/env zsh

river_dreams::commands_separator() {
  local -r commands_separator_symbol_i=${RIVER_DREAMS_COMMANDS_SEPARATOR_SYMBOL:-▽}
  local -r commands_separator_symbol_ii=${RIVER_DREAMS_COMMANDS_SEPARATOR_SYMBOL:-▼}
  local -r commands_separator_symbol_color=${RIVER_DREAMS_COMMANDS_SEPARATOR_SYMBOL_COLOR:-normal}

  for column in $(seq 1 $((${COLUMNS} / 2))); do
    echo -n "%F{${commands_separator_symbol_color}}${commands_separator_symbol_i}${commands_separator_symbol_ii}%F{normal}"
  done

  if [[ ! $((${COLUMNS} % 2)) -eq 0 ]]; then
    echo -n ${commands_separator_symbol_i}
  fi
}
