#!/usr/bin/env zsh

setopt promptsubst

function _river_dreams::get_git_branch() {
	typeset -r branch="$(git branch --show-current 2>/dev/null)"
	if [[ -n "${branch}" ]]; then
		echo "%F{green}:«%f${branch}%F{green}»%f"
	fi
}

PROMPT='%F{yellow}>%> %F{red}%~%f$(_river_dreams::get_git_branch)  '

