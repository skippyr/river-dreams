#!/usr/bin/env zsh

river_dreams::exit_code() {
  local -r exit_code_symbol=$(
    test ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true &&
    echo "X" ||
    echo ""
  )
  echo "%(?..%F{yellow}[%F{red}${exit_code_symbol} %B%?%b%F{yellow}]%f)"
}
