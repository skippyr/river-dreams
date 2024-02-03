export VIRTUAL_ENV_DISABLE_PROMPT=1;
_RDROOT=$(dirname ${0});

[[ ! -f ${_RDROOT}/out/lprompt || ! -f ${_RDROOT}/out/rprompt ]] &&
  make -s -C ${_RDROOT};

setopt promptsubst;
PROMPT='$(${_RDROOT}/out/lprompt)';
RPROMPT='$(${_RDROOT}/out/rprompt)';
