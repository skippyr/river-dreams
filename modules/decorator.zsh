#!/usr/bin/env zsh

river_dreams::decorator() {
  local -r decorator_symbol=$(
    test $(tput colors) -eq 8 &&
    echo ">=>> " ||
    echo "⤐ "
  )
  echo "%(?.%F{green}.%F{red})${decorator_symbol}%f"
}
