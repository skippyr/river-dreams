export VIRTUAL_ENV_DISABLE_PROMPT=1;
typeset -g _riverdreams_root_g=$(dirname ${0});

[[ ! -f ${_riverdreams_root_g}/out/lprompt ||
   ! -f ${_riverdreams_root_g}/out/rprompt ]] &&
	make -sC ${_riverdreams_root_g};

setopt promptsubst;
PROMPT='$(${_riverdreams_root_g}/out/lprompt)';
RPROMPT='$(${_riverdreams_root_g}/out/rprompt)';
