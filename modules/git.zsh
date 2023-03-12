#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  git branch --show-current 2>/dev/null
}

river_dreams::git::get_commit_hash() {
  git rev-parse --short HEAD 2>/dev/null
}

river_dreams::git::get_changes_quantity() {
  git ls-files -mo | wc -l
}

river_dreams::git() {
  local -r branch=$(river_dreams::git::get_branch)
  [[ -z ${branch} ]] && exit
  local commit_hash=$(river_dreams::git::get_commit_hash)
  [[ -n ${commit_hash} ]] && commit_hash=" ${commit_hash}"
  local changes_quantity=$(river_dreams::git::get_changes_quantity)
  local changes_section=""
  [[ ${changes_quantity} -gt 0 ]] && changes_section="${changes_quantity}* "
  echo "%F{red}«%F{yellow}${changes_section}%f${branch}%F{yellow}${commit_hash}%F{red}»"
}

