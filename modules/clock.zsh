#!/usr/bin/env zsh

river_dreams::clock() {
  local symbol=""
  local clock_symbol=""
  local symbol_color=""
  local -r current_hours=$(date +%_H)

  if [[ ${current_hours} -lt 6 ]]; then
    symbol=" "
    symbol_color="magenta"
  elif [[ ${current_hours} -lt 12 ]]; then
    symbol=" "
    symbol_color="yellow"
  elif [[ ${current_hours} -lt 18 ]]; then
    symbol="󰖚 "
    symbol_color="red"
  else
    symbol="󰽥"
    symbol_color="yellow"
  fi

  echo "%F{${symbol_color}}${symbol}%f $(date +%Hh%Mm)"
}
