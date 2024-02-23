export VIRTUAL_ENV_DISABLE_PROMPT=1;
typeset -g _RIVER_DREAMS_ROOT=$(dirname ${0});

[[ ! -f ${_RIVER_DREAMS_ROOT}/bin/left-prompt ||
   ! -f ${_RIVER_DREAMS_ROOT}/bin/right-prompt ]] &&
  make -sC ${_RIVER_DREAMS_ROOT};

setopt promptsubst;
PROMPT='$(${_RIVER_DREAMS_ROOT}/bin/left-prompt)';
RPROMPT='$(${_RIVER_DREAMS_ROOT}/bin/right-prompt)';
