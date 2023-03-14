#!/usr/bin/env zsh

river_dreams::left_prompt_component() {
  local ligature_symbol="╚"
  [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] && ligature_symbol="└"
  echo %F{red}${ligature_symbol}%f$(river_dreams::exit_code)$(river_dreams::root)$(river_dreams::vi_mode)$(river_dreams::decorator)$(river_dreams::directory)$(river_dreams::git)$(river_dreams::directory_ownership)
}
