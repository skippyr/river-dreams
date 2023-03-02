#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  git branch 2>/dev/null | grep "*" | tr -d "* "
}

river_dreams::git::has_changes() {
  git status -s 2>/dev/null | grep "[?NMD].* " &>/dev/null
}

river_dreams::git::needs_to_commit() {
  git status -s 2>/dev/null | grep "A.* " &>/dev/null
}

river_dreams::git::needs_push() {
  git status 2>/dev/null | grep -i "your branch is ahead" &>/dev/null
}

river_dreams::git() {
  local -r branch=$(river_dreams::git::get_branch)

  changes_symbol=$(
    river_dreams::git::has_changes &&
    echo "?" ||
    echo ""
  )
  commit_symbol=$(
    river_dreams::git::needs_to_commit &&
    echo "+" ||
    echo ""
  )
  push_symbol=$(
    river_dreams::git::needs_push &&
    echo "󰄿" ||
    echo ""
  )

  if [[ -n ${branch} ]]; then
    echo "%F{red}::%F{yellow}${changes_symbol}%F{cyan}${commit_symbol}%F{blue}${push_symbol}%F{normal}${branch}%F{red}"
  fi
}
