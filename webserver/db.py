import sqlite3
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_FILE = os.path.join(BASE_DIR, "data", "sensor_data.db")

# Ensure data folder exists
os.makedirs(os.path.dirname(DB_FILE), exist_ok=True)

def init_db():
    """Initialize the database and create table if it doesn't exist."""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS sensor_data (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT NOT NULL,
            temperature REAL,
            pressure REAL,
            humidity REAL,
            lux INTEGER
        )
    """)
    conn.commit()
    conn.close()

def insert_measurement(timestamp, temperature, pressure, humidity, lux):
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()
    cursor.execute("""
        INSERT INTO sensor_data (timestamp, temperature, pressure, humidity, lux)
        VALUES (?, ?, ?, ?, ?)
    """, (timestamp, temperature, pressure, humidity, lux))
    conn.commit()
    conn.close()

#fetch as dict
def fetch_all_measurements(order_desc=True):
    conn = sqlite3.connect(DB_FILE)
    c = conn.cursor()
    
    order = "DESC" if order_desc else "ASC"
    c.execute(f"""
        SELECT timestamp, temperature, pressure, humidity, lux
        FROM sensor_data
        ORDER BY timestamp {order}
    """)
    
    rows = c.fetchall()
    conn.close()
    
    return [
        {
            "timestamp": row[0],
            "temperature": row[1],
            "pressure": row[2],
            "humidity": row[3],
            "lux": row[4]
        } for row in rows
    ]