#!/usr/bin/env zsh

river_dreams::disk_usage() {
  local -r disk_usage=$(
    df |
    grep "/$" |
    awk '{print $5}' |
    tr -d "%"
  )
  local -r disk_symbol=$(
    test ${RIVER_DREAMS_FALLBACK_TEXT} == true &&
    echo "DISK" ||
    echo ""
  )

  echo "%F{green}${disk_symbol} %f${disk_usage}%% [%F{green}/%f]"
}
