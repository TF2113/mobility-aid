
CONFIG_PATH = "./src/configs/config.txt"

def save_to_config(config):
    with open(CONFIG_PATH, "w") as f:
        f.write(f"prox_vibrate={config['prox_vibrate']}\n")
        f.write(f"prox_yellow_led={config['prox_yellow_led']}\n")
        f.write(f"camera_enabled={int(config['camera_enabled'])}\n")