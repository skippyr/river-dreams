#!/usr/bin/env zsh

river_dreams::right_prompt_component() {
  local right_prompt_components=()

  right_prompt_components+=($(river_dreams::hidden_files))
  right_prompt_components+=($(river_dreams::untracked_files))
  right_prompt_components+=($(river_dreams::docker_containers))
  right_prompt_components+=($(river_dreams::jobs))
  right_prompt_components+=($(river_dreams::time_elapsed))

  echo ${right_prompt_components[@]}
}
