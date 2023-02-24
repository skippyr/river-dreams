#!/usr/bin/env zsh

river_dreams::decorator() {
  local -r decorator_symbol=${RIVER_DREAMS_DECORATOR_SYMBOL:-⤐ }
  local -r decorator_symbol_color=${RIVER_DREAMS_DECORATOR_SYMBOL_COLOR:-yellow}
  
  echo "%F{${decorator_symbol_color}}${decorator_symbol}%F{normal}"
}
