# Enables ZSH prompt substitution.
#
# This make it substitute variables and functions in the prompt variables, but
# they need to be defined using single quotes for it to work.
setopt promptsubst
# Prevents the default changes made to the prompt when sourcing a virtual
# environment.
export VIRTUAL_ENV_DISABLE_PROMPT="1"

# To avoid conflicts with possible user defined variables and functions, all
# resources used in this theme use either the "RIVER_DREAMS" or "river_dreams"
# prefixes.
#
# They might also use an underline character (_) at their start to make them
# harder to find when using a regular tab completition.

# Sets the environment variable used by the binaries to track if it is to use
# fallback or not. If not set by the user, it will be set automatically based
# on the terminal emulator's color support.
export RIVER_DREAMS_USE_FALLBACK_TEXT=${RIVER_DREAMS_USE_FALLBACK_TEXT:-$(
  [[ $(tput colors) -eq "8" ]] &&
  echo "1" || # Enables if terminal emulator supports the 8 bits color palette.
  echo "0"    # Disables otherwise.
)}

# The directory where River Dreams is stored at.
typeset -gr RIVER_DREAMS_DIRECTORY_PATH="$(dirname "$(realpath "$0")")"
# The file containing the instructions Cargo need to use when compiling the
# source code.
typeset -gr RIVER_DREAMS_MANIFEST_FILE_PATH="${RIVER_DREAMS_DIRECTORY_PATH}/Cargo.toml"
# The directory where the binaries will be stored at.
typeset -gr RIVER_DREAMS_RELEASE_DIRECTORY_PATH="${RIVER_DREAMS_DIRECTORY_PATH}/target/release"

# Sources the fallback theme to be used in case the compilation fails.
source "${RIVER_DREAMS_DIRECTORY_PATH}/river_dreams_fallback.zsh-theme"

# Compiles the source code using Cargo.
function river_dreams::compile {
  cargo build --release --manifest-path "${RIVER_DREAMS_MANIFEST_FILE_PATH}" || (
    echo ""
    echo "[!] Compilation Error"
    echo "\tProgram:"
    echo "\t\tRiver Dreams."
    echo "\tDescription:"
    echo "\t\tCould not compile the source files using cargo."
    echo "\tSuggestion(s):"
    echo "\t\t* Ensure that you are connected to the internet."
    echo "\t\t* Ensure that you have installed all the required dependencies to compile them."
    echo "\t\t  You can find more information about it in the README.md file:"
    echo "\t\t* If you have modified the source code, ensure that it does not contain any syntax error."
    echo "\t\t  Read the instructions given above by cargo to track the issue easily."
    echo ""
    echo "\tAfter following the suggestions, restart your ZSH session to try to compile them again."
    echo ""
    echo "\tIf you need more help you can report an issue at:"
    echo "\t\thttps://github.com/skippyr/river_dreams/issues"
  )
}

# Toggles the fallback text feature.
function river_dreams::toggle_fallback_text {
  if [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} -eq "0" ]]; then
    export RIVER_DREAMS_USE_FALLBACK_TEXT="1"
    echo "Fallback text feature has been enabled."
  else
    export RIVER_DREAMS_USE_FALLBACK_TEXT="0"
    echo "Fallback text feature has been disabled."
  fi
}

# If the binaries can not be found in the startup, tries to automatically
# compile them again.
if [[
  ! -f "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/left_prompt" ||
  ! -f "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/right_prompt"
]]; then
  echo "Trying to automatically compile the source files using cargo."
  echo "This process may take some time depending of your internet connection."
  echo ""
  river_dreams::compile
fi
# After a sucessfull compilation, in which all the binaries will exist, sets
# their output as the prompt variables.
#
# Otherwise, uses the fallback theme sourced previously.
if [[
  -f "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/left_prompt" &&
  -f "${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/right_prompt"
]]; then
  PROMPT='$("${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/left_prompt")'
  RPROMPT='$("${RIVER_DREAMS_RELEASE_DIRECTORY_PATH}/right_prompt")'
else
  echo ""
  echo "\tYou are now running River Dreams using a fallback prompt with limited functionalities."
  echo "\tSolve previous reported errors to use the main prompt."
fi

