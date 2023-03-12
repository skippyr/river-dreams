#!/usr/bin/env zsh

river_dreams::disk_usage() {
  local -r disk_usage=$(
    df |
    awk '{if ($6 ~ "/$") {print $5}}'
  )
  local -r disk_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    echo "DISK" ||
    echo ""
  )

  echo "%F{green}${disk_symbol} %f${disk_usage}% [%F{green}/%f]"
}
