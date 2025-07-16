import subprocess
import time

server = subprocess.Popen(["python", "web/app.py"])
inference = subprocess.Popen(["python", "src/cam/inference.py"])
mobility = subprocess.Popen(["./builds/mobility_aid"])
time.sleep(60)
server.terminate() 
inference.terminate() 
mobility.terminate() 