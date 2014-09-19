#!/bin/sh
. ${HOME}/.bash_profile

killp PurenessScopeServer
./PurenessScopeServer > /dev/null
