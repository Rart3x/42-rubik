import subprocess
import threading
import queue

class Solver:
    def __init__(self, path: str):
        self.proc = subprocess.Popen(
            [path, "-c"],
            stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            bufsize=1, text=True  # line-buffered text I/O
        )
        self._q = queue.Queue()
        threading.Thread(target=self._read_stdout, daemon=True).start()

    def __del__(self):
        self.close()

    def _read_stdout(self):
        for line in self.proc.stdout:
            self._q.put(line.rstrip("\n"))

    def solve(self, moves: str, timeout_sec=5.0) -> list[str]:
        self.proc.stdin.write(moves + "\n")
        self.proc.stdin.flush()
        try:
            line = self._q.get(timeout=timeout_sec)
        except queue.Empty:
            raise TimeoutError("Solver timed out")
        if line == "SOLVED":
            return []
        if line.startswith("ERROR: "):
            raise RuntimeError(line)
        return line.split()

    def close(self):
        try:
            self.proc.stdin.write("QUIT\n"); self.proc.stdin.flush()
        except Exception:
            pass
        self.proc.terminate()
