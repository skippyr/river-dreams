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
  rm -rf ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}
  mkdir -p ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}
  for file in $(ls ${RIVER_DREAMS_SOURCES_DIRECTORY}); do
    gcc ${RIVER_DREAMS_SOURCES_DIRECTORY}/${file} -o\
    ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}/$(echo ${file} | cut -f 1 -d ".")
  done
}

if [[
  ! $(ls ${RIVER_DREAMS_SOURCES_DIRECTORY} | wc -l) -eq
  $(ls ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY} 2>/dev/null | wc -l)
]]; then
  river_dreams::recompile
fi

river_dreams::arrow() {
  echo "%(?.%F{yellow}.%F{red})${RIVER_DREAMS_SYMBOLS[arrow]}%f"
}

river_dreams::commands_separator() {
  ${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}/commands_separator\
  ${COLUMNS}\
  ${RIVER_DREAMS_SYMBOLS[commands_separator_odd]}\
  ${RIVER_DREAMS_SYMBOLS[commands_separator_even]}
}

river_dreams::directory_path() {
  echo "%F{green}$(${RIVER_DREAMS_DISTRIBUTIONS_DIRECTORY}/directory_path)%f"
}

river_dreams::refresh_symbols() {
  [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} -eq 0 ]] &&
    typeset -gA RIVER_DREAMS_SYMBOLS=(
      ["arrow"]="⤐  "
      ["commands_separator_odd"]="▼"
      ["commands_separator_even"]="▲"
    ) ||
    typeset -gA RIVER_DREAMS_SYMBOLS=(
      ["arrow"]=">"
      ["commands_separator_odd"]="\\"
      ["commands_separator_even"]="/"
    )
}

precmd() {
  river_dreams::refresh_symbols
}

river_dreams::left_prompt() {
  echo "$(river_dreams::commands_separator)$(river_dreams::arrow)$(river_dreams::directory_path)"
}

PROMPT='$(river_dreams::left_prompt) '

