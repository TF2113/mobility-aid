from flask import Flask, render_template, request, redirect, url_for, flash
from src.configs.db import get_config_value, set_config_value

app = Flask(__name__)
app.secret_key = "devkey321"

#Default config with min and intermediate distances
default_config = {
    "prox_vibrate": 15,   # cm
    "prox_yellow_led": 25,  # cm
    "camera_enabled": True,
}

def load_config():
    config = {}
    for key, default in default_config.items():
        val = get_config_value(key)
        if val is None:
            config[key] = default
        else:
            if isinstance(default, bool):
                config[key] = val == "True" or val == "1"
            elif isinstance(default, int):
                config[key] = int(val)
            else:
                config[key] = val
    return config

def save_to_db(config):
    for key, value in config.items():
        set_config_value(key, value)

@app.route("/", methods=["GET", "POST"])
def dashboard():
    global config
    if request.method == "POST":
        try:
            config["prox_vibrate"] = int(request.form.get("prox_vibrate", 30))
            config["prox_yellow_led"] = int(request.form.get("prox_yellow_led", 60))
            config["camera_enabled"] = bool(request.form.get("camera_enabled"))
            
            #Ensure intermediate is > than min
            if config["prox_yellow_led"] <= config["prox_vibrate"]:
                flash("Intermediate distance must be greater than minimum distance.", "danger")
            else:
                save_to_db(config)
                flash("Configuration updated successfully!", "success")
        except Exception as e:
            flash(f"Error updating configuration: {e}", "danger")
        return redirect(url_for("dashboard"))
    
    config = load_config()
    return render_template("index.html", config=config)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)
