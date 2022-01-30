import toml
import json
import subprocess
import sys
import time

def Counterexp(exp_duration,packet_buffer_list,iteration,ccp_algs,delay_list,bdw_list,time):
    for packet_buffer in packet_buffer_list:
        for bdw,var,num,interval in bdw_list:
            bdw=str(bdw)+"Mbps"
            trace_file="counter"+"-"+str(bdw)+"-"+str(var)+".txt"
            for predelay,postdelay in delay_list:        
                for ccp_alg in ccp_algs:
                    for iter_num in range(iteration):    
                        filename="counter"+"-"+f'{ccp_alg}'+str(bdw)+"-"+str(var)+"-"+str(predelay)+"-"+str(postdelay)+"-"+str(packet_buffer)+"-"+str(iter_num)+".pcap"
                        sub= " \" trace_file=f'{trace_file}' filename=f'{filename}'  tcpTypeId =f'{ccp_alg}'  experimenttype=1 preBdw=f'{bdw}' preDelay=f'{predelay}' postDelay=f'{postdelay}' stopTime=f'{exp_duration}'  packetbuffer=f'{packet_buffer}' delaychangingtime=f'{time}' \" "
                        cmd=" cd .. && ./waf --run test "+sub+ "&& cd scratch "
                        subprocess.run(
                            cmd,
                            shell=True)
                    
