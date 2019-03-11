#!/bin/sh
. ${HOME}/.bash_profile

./killp PurenessScopeServer
nohup ./PurenessScopeServer > nohup.out 2>&1 &
