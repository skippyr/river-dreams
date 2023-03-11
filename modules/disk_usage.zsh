#!/usr/bin/env zsh

river_dreams::disk_usage() {
  local -r disk_usage=$(
    df |
    grep "/$" |
    awk '{print $5}' |
    tr -d "%"
  )
  local -r disk_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    echo "DISK" ||
    echo ""
  )

  echo "%F{green}${disk_symbol} %f${disk_usage}%% [%F{green}/%f]"
}
