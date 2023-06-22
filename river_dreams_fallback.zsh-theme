# If inside a Git repository, prints the name of the branch.
function _river_dreams::print_git_branch() {
  typeset -r branch=$(git branch --show-current 2>/dev/null)
  [[ -n "${branch}" ]] &&
  echo "%F{green}:«%f${branch}%F{green}»%f"
}

PROMPT='%F{yellow}>%> %F{red}%~%f$(_river_dreams::print_git_branch)  '

