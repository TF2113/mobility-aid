from ultralytics import YOLO
import cv2

model = YOLO('./ml/models/best.pt')

image_path = './builds/captures/image0.jpg'
img = cv2.imread(image_path)
if img is None:
    raise Exception(f"Failed to load image {image_path}")

results = model(img, imgsz=320)
result = results[0]

boxes = result.boxes.xyxy.cpu().numpy()
scores = result.boxes.conf.cpu().numpy()
class_ids = result.boxes.cls.cpu().numpy().astype(int)

class_names = [
    "ped_green", "ped_red", "ped_wait", "traffic_amber", "traffic_green", "traffic_red"
]

detected_labels = set()

for box, score, cls in zip(boxes, scores, class_ids):
    x1, y1, x2, y2 = box.astype(int)
    label_name = class_names[cls] if cls < len(class_names) else f"ID:{cls}"
    detected_labels.add(label_name)
    label = f"{label_name} {score:.2f}"
    cv2.rectangle(img, (x1, y1), (x2, y2), (0, 255, 0), 2)
    cv2.putText(img, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,255), 2)

output_path = 'output.jpg'
cv2.imwrite(output_path, img)
print(f"Output saved to {output_path}")

print("Detected labels in the image:", detected_labels)