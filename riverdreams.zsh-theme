setopt promptsubst;

export VIRTUAL_ENV_DISABLE_PROMPT=1;
_riverdreams_root_g=$(dirname ${0});

[[ ! -f ${_riverdreams_root_g}/build/bin/lprompt ||
   ! -f ${_riverdreams_root_g}/build/bin/rprompt ]] &&
	make -sC ${_riverdreams_root_g};

PROMPT='$(${_riverdreams_root_g}/build/bin/lprompt)';
RPROMPT='$(${_riverdreams_root_g}/build/bin/rprompt)';
