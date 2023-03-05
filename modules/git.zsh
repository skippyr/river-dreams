#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  git branch 2>/dev/null | grep "*" | tr -d "* "
}

river_dreams::git::has_changes() {
  git status -s 2>/dev/null | grep "[?NMD].* " &>/dev/null
}

river_dreams::git::get_changes_quantity() {
  git status -s 2>/dev/null | grep "[?NMD].* " | wc -l
}

river_dreams::git::needs_to_commit() {
  git status -s 2>/dev/null | grep "A.* " &>/dev/null
}

river_dreams::git::get_commits_quantity() {
  git status -s 2>/dev/null | grep "A.* " | wc -l
}

river_dreams::git::needs_push() {
  git status 2>/dev/null | grep -i "your branch is ahead" &>/dev/null
}

river_dreams::git::get_pushes_quantity() {
  git status 2>/dev/null | head -n 2 | grep -iv "on " | cut -f 8 -d " "
}

river_dreams::git() {
  local -r branch=$(river_dreams::git::get_branch)

  changes_quantity=$(river_dreams::git::get_changes_quantity)
  changes_section=$(
    river_dreams::git::has_changes &&
    echo "${changes_quantity}* " ||
    echo ""
  )
  commits_quantity=$(river_dreams::git::get_commits_quantity)
  commit_section=$(
    river_dreams::git::needs_to_commit &&
    echo "${commits_quantity}+ " ||
    echo ""
  )
  pushes_quantity=$(river_dreams::git::get_pushes_quantity)
  push_section=$(
    river_dreams::git::needs_push &&
    echo "${pushes_quantity}↑ " ||
    echo ""
  )

  if [[ -n ${branch} ]]; then
    echo "%F{red}::%F{yellow}${changes_section}%F{green}${commit_section}%F{red}${push_section}%F{normal}${branch}%F{red}%F{normal}"
  fi
}
