#!/usr/bin/env bash

river_dreams::tag() {
  [[ ! $(ls .git 2>/dev/null) ]] && exit

  local -r tag=$(git tag 2>/dev/null | tail -n 1)

  [[ -z ${tag} ]] && exit

  local -r tag_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    echo "TAG" ||
    echo ""
  )
  echo "%F{blue}${tag_symbol} %f${tag}"
}

