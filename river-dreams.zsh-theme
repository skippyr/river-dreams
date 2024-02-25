export VIRTUAL_ENV_DISABLE_PROMPT=1;
typeset -g _riverDreams_root_g=$(dirname ${0});

[[ ! -f ${_riverDreams_root_g}/out/left-prompt ||
   ! -f ${_riverDreams_root_g}/out/right-prompt ]] &&
	make -sC ${_riverDreams_root_g};

setopt promptsubst;
PROMPT='$(${_riverDreams_root_g}/out/left-prompt)';
RPROMPT='$(${_riverDreams_root_g}/out/right-prompt)';
