import subprocess
import socket
from pprint import pprint

URL = "http://pushgateway.example.org:9091/metrics/job/"
JOB = "arp_job"

def cmdCall(cmd):
    subp = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    subp.wait()
    if subp.poll() != 0:
        raise Exception('failed')
    return subp.stdout.readlines()

def pushGateWay(metric, val):
    push_cmd =   "echo " +  '"' + metric + " " + val + '"' + " | curl --data-binary @- " + URL + JOB
    print(push_cmd)
    #cmdCall(push_cmd)
    


def getArpStats():
    for line in cmdCall('./client'):
        line = line.strip()
        if line.startswith(b'arp'):
            pushGateWay('arp', line.split()[2].decode())
        elif line.startswith(b'ipv4'):
            pushGateWay('ipv4', line.split()[2].decode())
        elif line.startswith(b'ipv6'):
            pushGateWay('ipv6', line.split()[2].decode())
        elif line.startswith(b'multicast'):
            pushGateWay('multicast', line.split()[2].decode())

def pushData():
    getArpStats()
    

if __name__ == "__main__":
    pushData()
