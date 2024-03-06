export VIRTUAL_ENV_DISABLE_PROMPT=1;
typeset -g _riverDreams_g_root=$(dirname ${0});

[[ ! -f ${_riverDreams_g_root}/out/left-prompt ||
   ! -f ${_riverDreams_g_root}/out/right-prompt ]] &&
	make -sC ${_riverDreams_g_root};

setopt promptsubst;
PROMPT='$(${_riverDreams_g_root}/out/left-prompt)';
RPROMPT='$(${_riverDreams_g_root}/out/right-prompt)';
