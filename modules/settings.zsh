#!/usr/bin/env zsh

river_dreams::settings::toggle_fallback_text() {
  if [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == false ]]; then
    export RIVER_DREAMS_USE_FALLBACK_TEXT=true
  else
    export RIVER_DREAMS_USE_FALLBACK_TEXT=false
  fi
}

river_dreams::settings::toggle_color_variants() {
  if [[ ${RIVER_DREAMS_USE_COLOR_VARIANTS} == false ]]; then
    export RIVER_DREAMS_USE_COLOR_VARIANTS=true
  else
    export RIVER_DREAMS_USE_COLOR_VARIANTS=false
  fi
}

river_dreams::settings::get_version() {
  if [[ $(git -C ${RIVER_DREAMS_REPOSITORY_DIRECTORY} branch 2>/dev/null) ]]; then
    echo "$(git -C "${RIVER_DREAMS_REPOSITORY_DIRECTORY}" branch --show-current)_$(git -C "${RIVER_DREAMS_REPOSITORY_DIRECTORY}" rev-parse --short HEAD 2>/dev/null) running in ${OSTYPE}"
  else
    echo "river_dreams: could not get version. Is git installed and are you the user owner of the repository downloaded?"
  fi
}

