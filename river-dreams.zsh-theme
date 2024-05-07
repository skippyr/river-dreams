setopt promptsubst;

export VIRTUAL_ENV_DISABLE_PROMPT=1;
_river_dreams_root_g=$(dirname ${0});

[[ ! -f ${_river_dreams_root_g}/build/bin/left-prompt ||
   ! -f ${_river_dreams_root_g}/build/bin/right-prompt ]] &&
  make -sC ${_river_dreams_root_g};

PROMPT='$(${_river_dreams_root_g}/build/bin/left-prompt ${ZVM_MODE})';
RPROMPT='$(${_river_dreams_root_g}/build/bin/right-prompt)';
