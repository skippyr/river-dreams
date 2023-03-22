#!/usr/bin/env zsh

river_dreams::clock() {
  local clock_symbol=""
  local clock_symbol_color=""
  local -r current_hours=$(date +%_H)

  if [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]]; then
    clock_symbol="CLOCK "
    clock_symbol_color="yellow"
  else
    if [[ ${current_hours} -lt 6 ]]; then
      clock_symbol=" "
      clock_symbol_color="cyan"
    elif [[ ${current_hours} -lt 12 ]]; then
      clock_symbol="盛"
      clock_symbol_color="red"
    elif [[ ${current_hours} -lt 18 ]]; then
      clock_symbol=" "
      clock_symbol_color="blue"
    else
      clock_symbol="󰽥 "
      clock_symbol_color="yellow"
    fi
  fi

  echo "%F{${clock_symbol_color}}${clock_symbol}%f$(date +%Hh%Mm)"
}
