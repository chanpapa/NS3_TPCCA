import toml
import json
import subprocess
import sys
import time
from tqdm import tqdm
from utils.counter_exp import Counterexp
from utils.uni_exp import Uniexp
from utils.traceloader import Unitraceloader,Countertraceloader
from utils import arg_parser

args = arg_parser.argument_parser()
config_file_path = args.config
configs = toml.load(config_file_path)

Type=configs['type']['type']
exp_duration = configs['generalsetting']['exp_duration']
packet_buffer_list=configs['generalsetting']['packet_buffer_list']
iteration=configs['generalsetting']['iteration']
ccp_algs = configs['ccp_algs']
for ccp_alg in ccp_algs:
    print(ccp_alg)

if Type=="uni":
    delay_list = configs['unisetting']['delay_list']
    prebdw_list=configs['unisetting']['prebdw_list']
    postbdw_list=configs['unisetting']['postbdw_list']
    Unitraceloader(prebdw_list,postbdw_list)
    Uniexp(exp_duration,packet_buffer_list,iteration,ccp_algs,delay_list,prebdw_list,postbdw_list)

elif Type=="counter":
    delay_list=configs['countersetting']['delay_list']
    bdw_list=configs['countersetting']['bdw_list']
    Countertraceloader(bdw_list)    
    Counterexp(exp_duration,packet_buffer_list,iteration,ccp_algs,delay_list,bdw_list)

else :
    print("Wrong setting!")
    exit(1)
