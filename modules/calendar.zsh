#!/usr/bin/env zsh

river_dreams::calendar() {
  local -r calendar=$(date +"%a, %Y/%b/%d")
  local -r calendar_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
      echo "CALENDAR" ||
      echo ""
  )
  echo "%F{red}${calendar_symbol} %f${calendar}"
}

