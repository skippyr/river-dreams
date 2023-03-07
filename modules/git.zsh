#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  git branch 2>/dev/null | grep "*" | tr -d "* "
}

river_dreams::git::has_changes() {
  git status -s 2>/dev/null | grep " [?NMD].* " &>/dev/null
}

river_dreams::git::get_changes_quantity() {
  git status -s 2>/dev/null | grep " [?NMD].* " | wc -l
}

river_dreams::git::needs_to_commit() {
  git status -s 2>/dev/null | grep "^[?ANMD].* " &>/dev/null
}

river_dreams::git::get_commits_quantity() {
  git status -s 2>/dev/null | grep "^[?ANMD].* " | wc -l
}

river_dreams::git::needs_push() {
  git status 2>/dev/null | grep -i "your branch is ahead" &>/dev/null
}

river_dreams::git::get_pushes_quantity() {
  git status 2>/dev/null | head -n 2 | grep -iv "on " | cut -f 8 -d " "
}

river_dreams::git() {
  local -r branch=$(river_dreams::git::get_branch)

  local -r changes_quantity=$(river_dreams::git::get_changes_quantity)
  local -r changes_section=$(
    river_dreams::git::has_changes &&
    echo "${changes_quantity}* " ||
    echo ""
  )
  local -r commits_quantity=$(river_dreams::git::get_commits_quantity)
  local -r commit_section=$(
    river_dreams::git::needs_to_commit &&
    echo "${commits_quantity}+ " ||
    echo ""
  )
  local -r pushes_quantity=$(river_dreams::git::get_pushes_quantity)
  local -r push_section=$(
    river_dreams::git::needs_push &&
    echo "${pushes_quantity}↑ " ||
    echo ""
  )
  local -r commit_hash=$(git log -1 --pretty=oneline 2>/dev/null | cut -c -8)

  if [[ -n ${branch} ]]; then
    echo "%F{red}::«%F{yellow}${changes_section}%F{green}${commit_section}%F{red}${push_section}%f${branch} %F{yellow}${commit_hash}%F{red}»%f"
  fi
}
