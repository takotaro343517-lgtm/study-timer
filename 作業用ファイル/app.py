from flask import Flask, render_template, request, redirect, url_for, jsonify
import sqlite3

app = Flask(__name__)
DB = "todos.db"

# ---------- DBユーティリティ ----------

def get_db():
    return sqlite3.connect(DB)

def init_db():
    with get_db() as conn:
        conn.execute("""
        CREATE TABLE IF NOT EXISTS todos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT,
            planned INTEGER,
            elapsed INTEGER DEFAULT 0
        )
        """)

# ---------- ページ ----------

@app.route("/")
def todo_page():
    with get_db() as conn:
        todos = conn.execute("SELECT * FROM todos").fetchall()
    return render_template("todo.html", todos=todos)

@app.route("/timer")
def timer_page():
    with get_db() as conn:
        rows = conn.execute("SELECT * FROM todos").fetchall()

    todos = []
    for t in rows:
        progress = min(100, int(t[3] / (t[2] * 60) * 100)) if t[2] else 0
        todos.append({
            "id": t[0],
            "title": t[1],
            "planned": t[2],
            "elapsed": t[3],
            "progress": progress
        })

    return render_template("timer.html", todos=todos)

# ---------- 処理 ----------

@app.route("/add_todo", methods=["POST"])
def add_todo():
    title = request.form["title"]
    planned = int(request.form["planned"])

    with get_db() as conn:
        conn.execute(
            "INSERT INTO todos (title, planned) VALUES (?, ?)",
            (title, planned)
        )

    return redirect(url_for("todo_page"))

@app.route("/add_time", methods=["POST"])
def add_time():
    data = request.get_json()
    todo_id = int(data["id"])
    seconds = int(data["seconds"])

    with get_db() as conn:
        conn.execute(
            "UPDATE todos SET elapsed = elapsed + ? WHERE id = ?",
            (seconds, todo_id)
        )

    return jsonify({"status": "ok"})

# ---------- 起動 ----------

if __name__ == "__main__":
    init_db()
    app.run()
