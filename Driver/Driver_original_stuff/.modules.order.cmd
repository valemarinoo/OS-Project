cmd_/home/matte/lab6/modules.order := {   echo /home/matte/lab6/lmmodule.ko; :; } | awk '!x[$$0]++' - > /home/matte/lab6/modules.order
