#!/bin/bash

# ENV PATH
export OMPL_APPS_DIR=`pwd`

# This Script's Path
SCRIPT_PATH=$( dirname ${BASH_SOURCE:-$0} )

# List of the Execution files
EXE_FILES=$(ls ${SCRIPT_PATH}/bin)

function _func_complete_omplrun()
{
    local cur=${COMP_WORDS[COMP_CWORD]}
    if [ "$COMP_CWORD" -eq 1 ]; then
        COMPREPLY=( $(compgen -W ${EXE_FILES} -- $cur) )
fi
}

function func_ompl()
{
    `find ${SCRIPT_PATH} -name $1`
}

alias omplrun=func_ompl
complete -o default -F _func_complete_omplrun omplrun
