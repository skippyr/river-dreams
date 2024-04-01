export VIRTUAL_ENV_DISABLE_PROMPT=1;
_RIVER_DREAMS_ROOT=$(dirname ${0});

setopt promptsubst;
PROMPT='$(${_RIVER_DREAMS_ROOT}/bin/left-prompt)';
RPROMPT='$(${_RIVER_DREAMS_ROOT}/bin/right-prompt)';

[[ ! -f ${_RIVER_DREAMS_ROOT}/bin/left-prompt ||
   ! -f ${_RIVER_DREAMS_ROOT}/bin/right-prompt ]] &&
  make -sC ${_RIVER_DREAMS_ROOT};
