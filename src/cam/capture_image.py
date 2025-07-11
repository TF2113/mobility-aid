import subprocess
import time
from ultralytics import YOLO
import cv2

def capture_image(image_path: str, width=320, height=320):
    cmd = [
        "libcamera-jpeg",
        "-n",        
        "-t", "1",   
        "--width", str(width),
        "--height", str(height),
        "-o", image_path
    ]
    try:
        subprocess.run(cmd, check=True)
        print("Image successfully captured.")
        return True
    except subprocess.CalledProcessError:
        print("Error: libcamera-jpeg capture failed.")
        return False

def run_inference(model, image_path: str):
    results = model(image_path)
    for result in results:
        boxes = result.boxes
        if boxes is None or len(boxes) == 0:
            print("No detections found.")
            return

        for box in boxes:
            cls = int(box.cls[0])
            conf = box.conf[0]
            label = model.names[cls]
            print(f"Detected: {label} (conf: {conf:.3f})")

def main():
    model = YOLO('./ml/models/best.pt')
    image_path = './builds/captures/image0.jpg'

    while True:
        if capture_image(image_path):
            run_inference(model, image_path)
        else:
            print("Skipping inference due to capture failure.")

        time.sleep(0.5)

if __name__ == "__main__":
    main()
