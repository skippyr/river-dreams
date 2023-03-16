#!/usr/bin/env zsh

river_dreams::elapsed_time() {
  local elapsed_time="$1"
  local elapsed_time_in_seconds=$(
    echo ${elapsed_time} |
    cut -f 2 -d :
  )
  local elapsed_time_in_minutes=$(
    echo ${elapsed_time} |
    cut -f 1 -d :    
  )
  if [[ ! ${elapsed_time_in_seconds} -eq 0 || ! ${elapsed_time_in_minutes} -eq 0 ]]; then
    [[ ${elapsed_time_in_seconds} -lt 10 ]] &&
      elapsed_time_in_seconds="${elapsed_time_in_seconds:1:1}s" ||
      elapsed_time_in_seconds="${elapsed_time_in_seconds}s"
    [[ ${elapsed_time_in_minutes} -eq 0 ]] &&
        elapsed_time_in_minutes="" ||
        elapsed_time_in_minutes="${elapsed_time_in_minutes}m"
    local -r elapsed_time_symbol=$(
      [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
      echo "TIME" ||
      echo "󰔛"
    )
    echo "%F{yellow}${elapsed_time_symbol} %f${elapsed_time_in_minutes}${elapsed_time_in_seconds}"
  fi
}
