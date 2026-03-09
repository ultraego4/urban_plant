from flask import Flask, request, render_template, jsonify
import json
import time
import os

app = Flask(__name__)


BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_FILE = os.path.join(BASE_DIR, "data", "sensor_data.json")

# Ensure data folder exists
if not os.path.exists(DATA_FILE):
    os.makedirs(os.path.dirname(DATA_FILE), exist_ok=True)
    with open(DATA_FILE, "w") as f:
        json.dump([], f)

@app.route("/api/measurements", methods=["POST"])
def receive_measurement():

    # validate json
    data = request.get_json()
    if not data:
        return jsonify({"error": "invalid JSON"}), 400

    # validate required fields
    required_fields = ["temperature", "pressure", "humidity", "lux"]
    for field in required_fields:
        if field not in data:
            return jsonify({"error": f"missing {field}"}), 400

    # add timestamp
    data["received_at"] = time.time()

    # write file, only keep last 1000 entries
    with open(DATA_FILE, "r") as f:
        all_data = json.load(f)

    all_data.append(data)
    all_data = all_data[-1000:]

    with open(DATA_FILE, "w") as f:
        json.dump(all_data, f)

    # status ok
    return jsonify({"status": "ok"}), 200

@app.route("/")
def index():
    # load data for rendering
    with open(DATA_FILE, "r") as f:
        all_data = json.load(f)

    return render_template("index.html", measurements=all_data)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)