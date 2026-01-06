import os
import sqlite3
from flask import Flask, render_template, request, redirect, url_for, jsonify, g

app = Flask(__name__,
            template_folder="apps/list_and_timer_app/templates")

BASE_DIR = os.path.dirname(__file__)
DB_PATH = os.path.join(BASE_DIR, "todos.db")


# ---------- コア関数 ----------
def get_conn():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row

    # ★ここで必ず保証★
    conn.execute("""
        CREATE TABLE IF NOT EXISTS todos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            planned INTEGER NOT NULL,
            elapsed INTEGER NOT NULL DEFAULT 0
        )
    """)
    conn.commit()
    return conn


@app.teardown_appcontext
def close_conn(e=None):
    conn = g.pop("conn", None)
    if conn:
        conn.close()


# ---------- 画面 ----------
@app.route("/", methods=["GET"])
def todo_page():
    conn = get_conn()
    todos = conn.execute("SELECT * FROM todos").fetchall()
    return render_template("index.html", todos=todos)


@app.route("/add", methods=["POST"])
def add_todo():
    conn = get_conn()
    data = request.get_json()

    conn.execute(
        "INSERT INTO todos (title, planned, elapsed) VALUES (?, ?, 0)",
        (data["title"], int(data["planned"]))
    )
    conn.commit()
    return jsonify({"status": "ok"})


if __name__ == "__main__":
    app.run(debug=True)
