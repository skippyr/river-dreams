#!/usr/bin/env zsh

setopt promptsubst

function river_dreams::_get_git_branch()
{
	typeset -r branch="$(git branch --show-current 2>/dev/null)"
	if [[ -n "${branch}" ]]; then
		echo "%F{green}:«%f${branch}%F{green}»%f"
	fi
}

PROMPT='%F{yellow}~> %F{red}%~%f$(river_dreams::_get_git_branch)  '

