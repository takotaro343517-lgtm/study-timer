let startTime = null;
let timerId = null;

function start() {
  if (timerId !== null) return;

  startTime = Date.now();
  timerId = setInterval(update, 50);
}

function stop() {
  if (timerId === null) return;

  const elapsedThisSession = Date.now() - startTime;

  clearInterval(timerId);
  timerId = null;

  const seconds = Math.floor(elapsedThisSession / 1000);
  const todoId = document.getElementById("todoSelect").value;

  fetch("/add_time", {
    method: "POST",
    headers: {"Content-Type": "application/json"},
    body: JSON.stringify({
      id: todoId,
      seconds: seconds
    })
  }).then(() => location.reload());
}

function update() {
  const current = Date.now() - startTime;
  updateDisplay(current);
}

function updateDisplay(ms) {
  const totalSeconds = Math.floor(ms / 1000);
  const minutes = Math.floor(totalSeconds / 60);
  const seconds = totalSeconds % 60;

  document.getElementById("time").textContent =
    `${minutes}分 ${seconds}秒`;
}
