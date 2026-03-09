from flask import Flask, request, render_template, jsonify
import db

app = Flask(__name__)

db.init_db()

@app.route("/api/measurements", methods=["POST"])
def receive_measurement():
    
    # validate json
    data = request.get_json()
    if not data:
        return jsonify({"error": "invalid JSON"}), 400

    # validate fields
    required_fields = ["timestamp", "temperature", "pressure", "humidity", "lux"]
    for field in required_fields:
        if field not in data:
            return jsonify({"error": f"missing {field}"}), 400

    db.insert_measurement(
        data["timestamp"],
        data["temperature"],
        data["pressure"],
        data["humidity"],
        data["lux"]
    )

    return jsonify({"status": "ok"}), 200

@app.route("/")
def index():
    measurements = db.fetch_all_measurements(order_desc=True)
    return render_template("index.html", measurements=measurements)
    
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)