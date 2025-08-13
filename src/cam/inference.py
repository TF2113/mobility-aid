import subprocess
import time
import queue
import threading
from ultralytics import YOLO

last_vibrate = 0
vibrate_cooldown = 1
vibrate_queue = queue.Queue(maxsize=3)

def queue_manager():
    while True:
        cmd = vibrate_queue.get()
        if cmd is None:
            break
        subprocess.run(cmd)

vib_thread = threading.Thread(target=queue_manager, daemon=True)
vib_thread.start()

def capture_image(image_path: str, width=320, height=320):
    cmd = [
        "rpicam-still",
        "-n",
        "-t", "1",
        "--width", str(width),
        "--height", str(height),
        "-o", image_path
    ]
    try:
        subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return True
    except subprocess.CalledProcessError:
        print("Error: capture failed.")
        return False

def run_inference(model, image_path: str, count):
    global last_vibrate
    
    results = model(image_path)
    detected_ped_green = False
    detected_ped_red = False
    
    print(f"[Capture {count}]")
    for result in results:
        for box in result.boxes:
            cls = int(box.cls[0])
            conf = box.conf[0]
            label = model.names[cls]
            print(f"Detected: {label} (conf: {conf:.3f})")
            if label == "ped_green" and conf > 0.5:
                detected_ped_green = True
            if label == "ped_red" and conf > 0.5:
                detected_ped_red = True
    
    now = time.time()
    if detected_ped_red and (now - last_vibrate) > vibrate_cooldown:
        if not vibrate_queue.full():
            vibrate_queue.put(["./builds/vibrate", "3", "0.5", "0.75"])
            last_vibrate = now
        else:
            print("Queue full, discarding new vibration")
    elif detected_ped_green and (now - last_vibrate) > vibrate_cooldown:
            if not vibrate_queue.full():
                vibrate_queue.put(["./builds/vibrate", "1", "2", "0"])
                last_vibrate = now
            else:
                print("Queue full, discarding new vibration")
                
def main():
    model = YOLO('./ml/models/best.pt')
    image_path = './builds/captures/image0.jpg'
    count = 0

    try:
        while True:
            count += 1
            if capture_image(image_path):
                run_inference(model, image_path, count)
            else:
                print(f"[Capture {count}] Skipped due to error.")
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("Exiting...")
    
    vibrate_queue.put(None)
    vib_thread.join()
    
if __name__ == "__main__":
    main()