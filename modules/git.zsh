#!/usr/bin/env zsh

river_dreams::git() {
  local -r branch=$(git branch 2>/dev/null | grep "*" | tr -d "* ")
  local -r unstaged_changes_symbol=$(
    git status -s 2>/dev/null |
    grep "[?NMD] " &>/dev/null &&
    echo "%F{magenta}✗" ||
    echo ""
  )
  local -r commit_changes_symbol=$(
    git status -s 2>/dev/null |
    grep "A " &>/dev/null &&
    echo "%F{red}+" ||
    echo ""
  )
  local -r pull_changes_symbol=$(
    git status 2>/dev/null |
    grep -i "your branch is ahead" &>/dev/null &&
    echo "%F{green}⇡" ||
    echo ""
  )
  if [[ -n ${branch} ]]; then
    echo "%F{red}:git(%F{yellow}${unstaged_changes_symbol}${commit_changes_symbol}${pull_changes_symbol}%F{normal}${branch}%F{red})"
  fi
}
