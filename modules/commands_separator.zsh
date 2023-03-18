#!/usr/bin/env zsh

river_dreams::commands_separator() {
  local commands_separator_symbol_even="▲"
  local commands_separator_symbol_odd="▼"
  if [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]]; then
    local commands_separator_symbol_even="/"
    local commands_separator_symbol_odd="\\"
  fi
  local odd_symbol_color=$(
    [[ ${RIVER_DREAMS_USE_COLOR_VARIANTS} == true ]] &&
      echo blue ||
      echo yellow
  )
  for column in $(seq 1 ${COLUMNS}); do
    [[ $((${column} % 2 )) -eq 0 ]] &&
      echo -n "%F{red}${commands_separator_symbol_even}%f" ||
      echo -n "%F{${odd_symbol_color}}${commands_separator_symbol_odd}%f"
  done
}
