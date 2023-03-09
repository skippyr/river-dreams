#!/usr/bin/env bash

river_dreams::tag() {
  local -r tag=$(git tag 2>/dev/null | tail -n 1)
  local -r tag_symbol=$(
    test ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true &&
    echo "TAG" ||
    echo ""
  )
  if [[ -n ${tag} ]]; then
    echo "%F{blue}${tag_symbol} %f${tag}"
  fi
}

