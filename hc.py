import json
from flask import Flask, request, jsonify

app = Flask(__name__)

jarak_DATA = []

@app.route("/")
def root_route():
    return "Hello world!"

@app.route("/jarak")
def get_jarak():
    return jsonify(jarak_DATA)

@app.route("/submit-jarak", methods=["POST"])
def post_jarak():
    pesan = request.data.decode("utf8")
    jarak_DATA.append(float(pesan))
    print(pesan)
    return f"Received jarak {pesan}"

@app.route("/submit", methods=["POST"])
def post_data():
    pesan = request.data.decode("utf8")
    pesan = json.loads(pesan)
    jarak_DATA.append(float(pesan["jarak"]))
    print(pesan)
    return f"Received data {pesan}"

if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0')
