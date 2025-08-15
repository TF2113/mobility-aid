import subprocess
import argparse

parser = argparse.ArgumentParser(description="Run YOLO CLI inference on a video and save output to a text file.")
parser.add_argument("--video", required=True, help="Path to the input video file")
parser.add_argument("--output", required=True, help="Path to the output text file")
args = parser.parse_args()

video_path = args.video
output_file = args.output
model_path = "ml/models/best.pt"
confidence = 0.3

cmd = [
    "yolo",
    "detect",
    "predict",
    f"model={model_path}",
    f"source={video_path}",
    f"conf={confidence}",
    "show=False",
    "save=False"
]

with open(output_file, "w") as f:
    subprocess.run(cmd, stdout=f, text=True)

print(f"Detections written to {output_file}")
