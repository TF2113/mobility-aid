from src.configs.db import get_config_value
import subprocess
import time
import os
import threading

img_path = "./builds/captures/image0.jpg"
inference = None
last_camera_enabled = None

def process_changes():
    global inference, last_camera_enabled
    val = get_config_value("camera_enabled")
    if val is None:
        print("[CONFIG] camera_enabled not set, running as disabled")
        camera_enabled = False
    else:
        camera_enabled = val == "1" or val.lower() == "true"
        
    if camera_enabled and inference is None:
        print("[CONFIG] Inference enabled — starting...")
        inference = subprocess.Popen(["python", "./src/cam/inference.py"])
    elif not camera_enabled and inference is not None:
        print("[CONFIG] Inference disabled — stopping...")
        if os.path.exists(img_path):
            os.remove(img_path)
        else:
            print("Image not found")
        inference.terminate()
        inference = None

def detect_changes(poll_interval=1):
    global last_camera_enabled

    # Initialize last_camera_enabled to current DB state
    val = get_config_value("camera_enabled")
    last_camera_enabled = val == '1' or (val and val.lower() == 'true')

    # Call once at start to trigger proper state
    process_changes()

    while True:
        val = get_config_value("camera_enabled")
        current_camera_enabled = val == '1' or (val and val.lower() == 'true')

        if current_camera_enabled != last_camera_enabled:
            print("[CONFIG] Camera status updated — reloading...")
            process_changes()
            last_camera_enabled = current_camera_enabled

        time.sleep(poll_interval)


change_thread = threading.Thread(target=detect_changes)
change_thread.daemon = True  # Allow the thread to exit when the main program exits
change_thread.start()

server = subprocess.Popen(["python", "-m", "web.app"]) 
mobility = subprocess.Popen(["./builds/mobility_aid"])
 
try:
    while True:
        time.sleep(1)  # Keep the main thread alive while others run
except KeyboardInterrupt:
    if os.path.exists(img_path):
        os.remove(img_path)
    else:
        print("Image not found")
    server.terminate()
    mobility.terminate()
    print("Program terminated.")