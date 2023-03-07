#!/usr/bin/env zsh

river_dreams::exit_code() {
  local -r exit_code_symbol=$(
    test $(tput colors) -eq 8 &&
    echo "X" ||
    echo ""
  )
  echo "%(?..%F{yellow}[%F{red}${exit_code_symbol} %B%?%b%F{yellow}]%f)"
}
