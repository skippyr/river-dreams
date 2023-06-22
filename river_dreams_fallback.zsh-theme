# This is a fallback theme that will be used every time the compilation
# fails, so the user will still be able to interact with the shell with a
# decent prompt that has Git support.

# As this theme is source by the main theme file, there is no need to
# set "promptsubst" again.

# If inside a Git repository, prints the name of the branch.
function _river_dreams::print_git_branch() {
  typeset -r branch=$(git branch --show-current 2>/dev/null)
  [[ -n "${branch}" ]] &&
  echo "%F{green}:«%f${branch}%F{green}»%f"
}

PROMPT='%F{yellow}>%> %F{red}%~%f$(_river_dreams::print_git_branch)  '

