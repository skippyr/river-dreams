setopt promptsubst;

export VIRTUAL_ENV_DISABLE_PROMPT=1;
_riverDreams_root_g=$(dirname $(dirname $(dirname ${0})));

if [[ ! -f "${_riverDreams_root_g}/build/bin/river-dreams" ]];
then
  if [[ ! $(command -v git) || ! $(command -v cmake) || ! $(command -v gcc) ||
        ! $(command -v make) ]];
  then
    echo;
    echo "\x1b[31m[ERROR] \x1b[1;39mriver-dreams \x1b[22;38;5;8m(code 1)\x1b[39m: some dependencies are missing. Install the ones defined in README.md file.";
    return;
  fi
  echo;
  echo "\x1b[34m[INFO] \x1b[1;39mriver-dreams\x1b[22m: wait while the theme is being prepared for the first time.";
  git -C ${_riverDreams_root_g} submodule init &> /dev/null;
  git -C ${_riverDreams_root_g} submodule update &> /dev/null;
  rm -rf "${_riverDreams_root_g}/build";
  cmake -S ${_riverDreams_root_g} \
        -B "${_riverDreams_root_g}/build/cmake" > /dev/null;
  cmake --build "${_riverDreams_root_g}/build/cmake" --config release > /dev/null;
  cmake --install "${_riverDreams_root_g}/build/cmake" > /dev/null;
  clear;
fi

export PATH="${PATH}:${_riverDreams_root_g}/build/bin";

PROMPT='$(river-dreams zsh left)';
RPROMPT='$(river-dreams zsh right)';
