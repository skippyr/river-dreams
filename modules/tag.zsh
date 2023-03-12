#!/usr/bin/env bash

river_dreams::tag() {
  local -r tag=$(git describe --tags --abbrev=0 2>/dev/null)
  [[ -z ${tag} ]] && exit
  local -r tag_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    echo "TAG" ||
    echo ""
  )
  echo "%F{blue}${tag_symbol} %f${tag}"
}

