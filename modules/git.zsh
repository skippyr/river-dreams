#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  git branch --show-current 2>/dev/null
}

river_dreams::git::get_commit_hash() {
  git rev-parse --short HEAD 2>/dev/null
}

river_dreams::git() {
  local -r branch=$(river_dreams::git::get_branch)
  [[ -z ${branch} ]] && exit
  local commit_hash=$(river_dreams::git::get_commit_hash)
  [[ -n ${commit_hash} ]] && commit_hash=" ${commit_hash}"
  echo "git:(${branch}${commit_hash})"
}
