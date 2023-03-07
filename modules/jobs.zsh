#!/usr/bin/env zsh

river_dreams::jobs() {
  local -r jobs_symbol=$(
    test $(tput colors) -eq 8 &&
    echo "JOBS" ||
    echo ""
  )
  echo "%(1j.%F{green}${jobs_symbol} %f%j.)"
}
