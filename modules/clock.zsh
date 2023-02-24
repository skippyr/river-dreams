#!/usr/bin/env zsh

river_dreams::clock::get_current_hours_and_minutes() {
  date +%Hh%Mm
}

river_dreams::clock() {
  local -r clock_symbol=${RIVER_DREAMS_CLOCK_SYMBOL:- }
  local -r clock_symbol_color=${RIVER_DREAMS_CLOCK_SYMBOL_COLOR:-yellow}

  local -r current_hours_and_minutes=$(river_dreams::clock::get_current_hours_and_minutes)
  echo "%F{${clock_symbol_color}}${clock_symbol}%F{normal}${current_hours_and_minutes}"
}
