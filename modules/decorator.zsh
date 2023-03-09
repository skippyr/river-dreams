#!/usr/bin/env zsh

river_dreams::decorator() {
  local -r decorator_symbol=$(
    test ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true &&
    echo ">" ||
    echo "⤐"
  )
  echo "%(?.%F{green}.%F{red})${decorator_symbol} %f"
}
