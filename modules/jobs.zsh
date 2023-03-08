#!/usr/bin/env zsh

river_dreams::jobs() {
  local -r jobs_symbol=$(
    test ${RIVER_DREAMS_FALLBACK_TEXT} == true &&
    echo "JOBS" ||
    echo ""
  )
  echo "%(1j.%F{green}${jobs_symbol} %f%j.)"
}
