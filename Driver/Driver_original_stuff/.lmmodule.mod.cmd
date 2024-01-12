cmd_/home/matte/lab6/lmmodule.mod := printf '%s\n'   lmmodule.o | awk '!x[$$0]++ { print("/home/matte/lab6/"$$0) }' > /home/matte/lab6/lmmodule.mod
