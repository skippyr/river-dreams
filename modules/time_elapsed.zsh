#!/usr/bin/env zsh

river_dreams::time_elapsed() {
  local -r time_elapsed=$(history -D | tail -n 1 | awk '{print $2}')
  local time_elapsed_in_seconds=$(
    echo ${time_elapsed} |
    cut -f 2 -d :
  )
  local time_elapsed_in_minutes=$(
    echo ${time_elapsed} |
    cut -f 1 -d :    
  )
  if [[ ! ${time_elapsed_in_seconds} -eq 0 && ! ${time_elapsed_in_minutes} -eq 0 ]]; then
    [[ ${time_elapsed_in_seconds} -lt 10 ]] &&
      time_elapsed_in_seconds="${time_elapsed_in_seconds:1:1}s" ||
      time_elapsed_in_seconds="${time_elapsed_in_seconds}s"
    [[ ${time_elapsed_in_minutes} -eq 0 ]] &&
        time_elapsed_in_minutes="" ||
        time_elapsed_in_minutes="${time_elapsed_in_minutes}m"
    local -r time_elapsed_symbol=$(
      [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
      echo "TIME" ||
      echo "󰔛"
    )
    echo "%F{yellow}${time_elapsed_symbol} %f${time_elapsed_in_minutes}${time_elapsed_in_seconds}"
  fi
}
