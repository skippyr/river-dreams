#!/usr/bin/env zsh

river_dreams::top_prompt() {
  local top_prompt_components=()

  top_prompt_components+=($(river_dreams::clock))
  top_prompt_components+=($(river_dreams::disk_usage))
  top_prompt_components+=($(river_dreams::ip_address))
  top_prompt_components+=($(river_dreams::python_environment))

  echo ${top_prompt_components[@]}
}
