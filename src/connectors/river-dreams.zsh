setopt promptsubst;

export VIRTUAL_ENV_DISABLE_PROMPT=1;
_riverDreams_root_g=$(dirname $(dirname $(dirname ${0})));

if [[ ! $(command -v git) || ! $(command -v cmake) || ! $(command -v gcc) ||
      ! $(command -v make) ]]; then
  echo;
  echo "river-dreams";
  echo;
  echo "[ERROR] Some dependencies are missing.";
  echo "        Please install the ones defined in the README.md file."
  return;
fi

if [[ ! -f "${_riverDreams_root_g}/build/bin/left-prompt" ||
      ! -f "${_riverDreams_root_g}/build/bin/right-prompt" ]]; then
  echo;
  echo "river-dreams";
  echo;
  echo "Preparing the theme for the first time.";
  echo "Please, wait just a moment.";
  git -C ${_riverDreams_root_g} submodule init &> /dev/null;
  git -C ${_riverDreams_root_g} submodule update &> /dev/null;
  rm -rf "${_riverDreams_root_g}/build";
  cmake -S ${_riverDreams_root_g} \
        -B "${_riverDreams_root_g}/build/cmake" > /dev/null;
  cmake --build "${_riverDreams_root_g}/build/cmake" --config release > /dev/null;
  cmake --install "${_riverDreams_root_g}/build/cmake" > /dev/null;
  clear;
fi

PROMPT='$(${_riverDreams_root_g}/build/bin/left-prompt)';
RPROMPT='$(${_riverDreams_root_g}/build/bin/right-prompt)';
