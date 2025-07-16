import subprocess
import time

p = subprocess.Popen(["./builds/mobility_aid"])
time.sleep(5)
p.terminate() 
