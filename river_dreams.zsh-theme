#!/usr/bin/env zsh

setopt promptsubst
setopt +o nomatch

export VIRTUAL_ENV_DISABLE_PROMPT=1
export RIVER_DREAMS_MODULES_DIRECTORY=$(dirname $0)/modules
export RIVER_DREAMS_PROMPT_COMPONENTS_DIRECTORY=$(dirname $0)/prompt_components
export RIVER_DREAMS_CONFIGURATIONS_FILE=~/.config/river_dreams/configurations.zsh
export RIVER_DREAMS_FALLBACK_TEXT=${RIVER_DREAMS_FALLBACK_TEXT:-$(
  test $(tput colors) -eq 8 &&
  echo "true" ||
  echo "false"
)}

for module_file in $(ls ${RIVER_DREAMS_MODULES_DIRECTORY}); do
  source ${RIVER_DREAMS_MODULES_DIRECTORY}/${module_file}
done

for prompt_component_file in $(ls ${RIVER_DREAMS_PROMPT_COMPONENTS_DIRECTORY}); do
  source ${RIVER_DREAMS_PROMPT_COMPONENTS_DIRECTORY}/${prompt_component_file}
done

source ${RIVER_DREAMS_CONFIGURATIONS_FILE} 2>/dev/null

PROMPT='$(river_dreams::commands_separator)$(river_dreams::top_prompt_component)
$(river_dreams::left_prompt_component)%f '
RPROMPT='$(river_dreams::right_prompt_component)'

