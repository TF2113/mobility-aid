from flask import Flask, render_template, request, redirect, url_for, flash

app = Flask(__name__)
app.secret_key = "devkey321"

#Default config with min and intermediate distances
config = {
    "prox_vibrate": 15,   # cm
    "prox_yellow_led": 25,  # cm
    "camera_enabled": True,
}

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
                flash("Configuration updated successfully!", "success")
        except Exception as e:
            flash(f"Error updating configuration: {e}", "danger")
        return redirect(url_for("dashboard"))
    return render_template("index.html", config=config)

if __name__ == "__main__":
    app.run(debug=True)
