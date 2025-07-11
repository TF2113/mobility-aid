import subprocess
import time
from ultralytics import YOLO

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
    results = model(image_path)
    print(f"[Capture {count}]")
    for result in results:
        for box in result.boxes:
            cls = int(box.cls[0])
            conf = box.conf[0]
            label = model.names[cls]
            print(f"Detected: {label} (conf: {conf:.3f})")

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