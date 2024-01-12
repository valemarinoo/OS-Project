cmd_/home/matte/lab6/Module.symvers := sed 's/ko$$/o/' /home/matte/lab6/modules.order | scripts/mod/modpost -m -a  -o /home/matte/lab6/Module.symvers -e -i Module.symvers   -T -
