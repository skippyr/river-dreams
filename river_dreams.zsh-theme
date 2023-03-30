#!/usr/bin/env zsh

export VIRTUAL_ENV_DISABLE_PROMPT=1
export RIVER_DREAMS_ROOT_DIRECTORY=$(dirname $0)
typeset -g RIVER_DREAMS_SOURCES_DIRECTORY=${RIVER_DREAMS_ROOT_DIRECTORY}/sources
typeset -g RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY=${RIVER_DREAMS_ROOT_DIRECTORY}/distributions
export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT:-$(
  [[ $(tput colors) -eq 8 ]] &&
    echo 1 ||
    echo 0
)}

setopt promptsubst
setopt +o nomatch

river_dreams::recompile() {
  echo "Recompiling sources"
  rm -rf ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}
  mkdir -p ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}
  for file in $(ls ${RIVER_DREAMS_SOURCES_DIRECTORY}/modules); do
    gcc ${RIVER_DREAMS_SOURCES_DIRECTORY}/modules/${file} -o\
    ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}/$(echo ${file} | cut -f 1 -d ".")
    echo "  * Recompiled ${file}"
  done
}

if [[
  ! $(ls ${RIVER_DREAMS_SOURCES_DIRECTORY}/modules | wc -l) -eq
  $(ls ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY} 2>/dev/null | wc -l)
]]; then
  river_dreams::recompile
fi

river_dreams::commands_separator() {
  ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}/commands_separator ${COLUMNS}
}

river_dreams::sysdisk() {
  ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}/sysdisk
}

river_dreams::arrow() {
  ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}/arrow
}

river_dreams::directory() {
  ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}/directory
}

river_dreams::left_prompt() {
  echo "$(river_dreams::commands_separator)[$(river_dreams::sysdisk)]
$(river_dreams::arrow)$(river_dreams::directory)"
}

PROMPT='$(river_dreams::left_prompt) '

