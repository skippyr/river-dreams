export VIRTUAL_ENV_DISABLE_PROMPT=1;
typeset -g _RDROOT=$(dirname ${0});

[[ ! -f ${_RDROOT}/out/lprompt || ! -f ${_RDROOT}/out/rprompt ]] && make -sC ${_RDROOT};

setopt promptsubst;
PROMPT='$(${_RDROOT}/out/lprompt)';
RPROMPT='$(${_RDROOT}/out/rprompt)';
