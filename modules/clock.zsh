#!/usr/bin/env zsh

river_dreams::clock() {
  local symbol=""
  local symbol_color=""
  local current_hours=$(date +%_H)

  if [[ ${current_hours} -lt 6 ]]; then
    symbol="✪"
    symbol_color="blue"
  elif [[ ${current_hours} -lt 12 ]]; then
    symbol=""
    symbol_color="yellow"
  elif [[ ${current_hours} -lt 18 ]]; then
    symbol=""
    symbol_color="yellow"
  else
    symbol="⏾"
    symbol_color="blue"
  fi

  echo "%F{yellow} %F{${symbol_color}}${symbol} %F{normal}$(date +%Hh%Mm)"
}
