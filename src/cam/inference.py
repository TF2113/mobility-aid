import subprocess
import time
from ultralytics import YOLO

last_vibrate = 0
vibrate_cooldown = 1

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
        subprocess.run(["./builds/vibrate", "3", "0.5", "0.75"])
        last_vibrate = now
    elif detected_ped_green and (now - last_vibrate) > vibrate_cooldown:
        subprocess.run(["./builds/vibrate", "1", "2", "0"])
        last_vibrate = now
        
        
def main():
    model = YOLO('./ml/models/best.pt')
    image_path = './builds/captures/image0.jpg'
    count = 0

    while True:
        count += 1
        if capture_image(image_path):
            run_inference(model, image_path, count)
        else:
            print(f"[Capture {count}] Skipped due to error.")
        time.sleep(0.5)

if __name__ == "__main__":
    main()