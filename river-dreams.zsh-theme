export VIRTUAL_ENV_DISABLE_PROMPT=1;
_g_riverDreamsRoot=$(dirname ${0});

[[ ! -f ${_g_riverDreamsRoot}/bin/left-prompt || ! -f ${_g_riverDreamsRoot}/bin/right-prompt ]] &&
    make -sC ${_g_riverDreamsRoot};

setopt promptsubst;
PROMPT='$(${_g_riverDreamsRoot}/bin/left-prompt)';
RPROMPT='$(${_g_riverDreamsRoot}/bin/right-prompt)';
