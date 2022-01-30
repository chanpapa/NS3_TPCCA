import toml
import json
import subprocess
import sys
import time

def Uniexp(exp_duration,packet_buffer_list,iteration,ccp_algs,delay_list,prebdw_list,postbdw_list):
    for packet_buffer in packet_buffer_list:
        for prebdw,prevar,prenum,preinterval in prebdw_list:
            for postbdw,postvar,postnum,postinterval in postbdw_list:
                trace_file="uni"+"-"+str(prebdw)+"-"+str(postbdw)+".txt"
                for predelay in delay_list:        
                    for ccp_alg in ccp_algs:
                        for iter_num in range(iteration):    
                            filename="Uni"+"-"+f'{ccp_alg}'+str(prebdw)+"-"+str(postbdw)+"-"+str(predelay)+"-"+str(packet_buffer)+"-"+str(iter_num)+".pcap"
                            sub=" \"trace_file=f'{trace_file}' filename=f'{filename}'  tcpTypeId =f'{ccp_alg}'  experimenttype=0  preDelay=f'{predelay}' postDelay=f'{predelay}' stopTime=f'{exp_duration}'  packetbuffer=f'{packet_buffer}' \" "
                            subprocess.run(
                                "cd .. && ./waf --run test "+sub+" && cd scratch",
                                shell=True)
                    
