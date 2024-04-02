export VIRTUAL_ENV_DISABLE_PROMPT=1;
_river_dreams_root_g=$(dirname ${0});

setopt promptsubst;
PROMPT='$(${_river_dreams_root_g}/build/bin/left-prompt)';
RPROMPT='$(${_river_dreams_root_g}/build/bin/right-prompt)';

[[ ! -f ${_river_dreams_root_g}/build/bin/left-prompt ||
   ! -f ${_river_dreams_root_g}/build/bin/right-prompt ]] &&
  make -sC ${_river_dreams_root_g};
