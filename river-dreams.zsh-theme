export VIRTUAL_ENV_DISABLE_PROMPT=1;
typeset -g _riverDreams_g_rootDirectory=$(dirname ${0});

[[ ! -f ${_riverDreams_g_rootDirectory}/out/left-prompt || ! -f ${_riverDreams_g_rootDirectory}/out/right-prompt ]] &&
	make -sC ${_riverDreams_g_rootDirectory};

setopt promptsubst;
PROMPT='$(${_riverDreams_g_rootDirectory}/out/left-prompt)';
RPROMPT='$(${_riverDreams_g_rootDirectory}/out/right-prompt)';
