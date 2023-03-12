#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  git branch --show-current 2>/dev/null
}

river_dreams::git() {
  local -r branch=$(river_dreams::git::get_branch)
  [[ -z ${branch} ]] && exit
  echo "git:(${branch})"
}
