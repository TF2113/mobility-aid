from flask import Flask, render_template, request, redirect, url_for, flash
from src.configs.db import get_config_value, set_config_value

def create_app(config=None):
    app = Flask(__name__)
    
    app.secret_key = "devkey321"

    if config:
        app.config.from_object(config)

    default_config = {
        "prox_vibrate": 15,
        "prox_yellow_led": 25,
        "camera_enabled": True,
    }

    def load_config_from_db():
        config = {}
        for key, default in default_config.items():
            val = get_config_value(key)
            if val is None:
                config[key] = default
            else:
                if isinstance(default, bool):
                    config[key] = str(val).lower() in ("true", "1")
                elif isinstance(default, int):
                    config[key] = int(val)
                else:
                    config[key] = val
        return config

    def save_to_db(config_to_save):
        for key, value in config_to_save.items():
            set_config_value(key, value)

    @app.route("/", methods=["GET", "POST"])
    def dashboard():
        if request.method == "POST":
            try:
                # Load current config to update it
                updated_config = load_config_from_db()

                updated_config["prox_vibrate"] = int(request.form.get("prox_vibrate"))
                updated_config["prox_yellow_led"] = int(request.form.get("prox_yellow_led"))
                updated_config["camera_enabled"] = "camera_enabled" in request.form
                
                if updated_config["prox_yellow_led"] <= updated_config["prox_vibrate"]:
                    flash("Intermediate distance must be greater than minimum distance.", "danger")
                else:
                    save_to_db(updated_config)
                    flash("Configuration updated successfully!", "success")

            except (ValueError, TypeError) as e:
                flash(f"Invalid input: Please ensure values are correct. ({e})", "danger")
            except Exception as e:
                flash(f"Error updating configuration: {e}", "danger")
            
            return redirect(url_for("dashboard"))
        
        # For GET request
        config = load_config_from_db()
        return render_template("index.html", config=config)

    return app