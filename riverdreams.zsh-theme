export VIRTUAL_ENV_DISABLE_PROMPT=1;
RIVERDREAMSROOT=$(dirname ${0});

[[ ! -f ${RIVERDREAMSROOT}/out/lprompt || ! -f ${RIVERDREAMSROOT}/out/rprompt ]] &&
	make -s -C ${RIVERDREAMSROOT};

setopt promptsubst;
PROMPT='$(${RIVERDREAMSROOT}/out/lprompt)';
RPROMPT='$(${RIVERDREAMSROOT}/out/rprompt)';
