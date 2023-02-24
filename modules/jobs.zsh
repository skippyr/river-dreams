#!/usr/bin/env zsh

river_dreams::jobs() {
  local -r jobs_symbol=${RIVER_DREAMS_JOBS_SYMBOL:- }
  local -r jobs_symbol_color=${RIVER_DREAMS_JOBS_SYMBOL_COLOR:-green}

  echo "%(1j.%F{${jobs_symbol_color}}${jobs_symbol}%F{normal}%j.)"
}
