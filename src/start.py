import subprocess
import time
import os
import threading

config_path = "./src/configs/config.txt"
inference = None

def process_changes(file_path):
    global inference
    with open(file_path) as config:
        content = config.readlines()

        enabled = any(line.strip() == "camera_enabled=1" for line in content)
        disabled = any(line.strip() == "camera_enabled=0" for line in content)

        if enabled and inference is None:
            print("[CONFIG] Inference enabled — starting...")
            inference = subprocess.Popen(["python", "src/cam/inference.py"])
        elif disabled and inference is not None:
            print("[CONFIG] Inference disabled — stopping...")
            inference.terminate()
            inference = None

def detect_changes(file_path):
    last_modified = os.path.getmtime(file_path)
    
    process_changes(config_path)
            
    while True:
        current_modify = os.path.getmtime(file_path)
        if current_modify != last_modified:
            print("[CONFIG] Camera status updated — reloading...")
            process_changes(config_path)
            last_modified = current_modify

        time.sleep(1)


change_thread = threading.Thread(target=detect_changes, args=(config_path,))
change_thread.daemon = True  # Allow the thread to exit when the main program exits
change_thread.start()

server = subprocess.Popen(["python", "web/app.py"]) 
mobility = subprocess.Popen(["./builds/mobility_aid"])
 
try:
    while True:
        time.sleep(1)  # Keep the main thread alive while others run
except KeyboardInterrupt:
    server.terminate()
    mobility.terminate()
    print("Program terminated.")