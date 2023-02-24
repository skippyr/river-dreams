#!/usr/bin/env zsh

setopt promptsubst
setopt +o nomatch

export VIRTUAL_ENV_DISABLE_PROMPT=1
export RIVER_DREAMS_MODULES_DIRECTORY=$(dirname $0)/modules
export RIVER_DREAMS_PROMPTS_DIRECTORY=$(dirname $0)/prompts

for module_file in $(ls ${RIVER_DREAMS_MODULES_DIRECTORY}); do
  source ${RIVER_DREAMS_MODULES_DIRECTORY}/${module_file}
done

for prompt_file in $(ls ${RIVER_DREAMS_PROMPTS_DIRECTORY}); do
  source ${RIVER_DREAMS_PROMPTS_DIRECTORY}/${prompt_file}
done


PROMPT='$(river_dreams::commands_separator)$(river_dreams::top_prompt)
$(river_dreams::left_prompt)%F{normal} '
RPROMPT='$(river_dreams::right_prompt)'
