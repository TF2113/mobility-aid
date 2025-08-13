import pytest
import subprocess
import sqlite3
import signal 

@pytest.fixture(scope="function")
def populated_c_test_db(tmp_path):
    db_path = tmp_path / "c_test_config.db"
    conn = None
    try:
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()
        cursor.execute("CREATE TABLE config (key TEXT PRIMARY KEY, value TEXT)")
        cursor.execute("INSERT INTO config (key, value) VALUES (?, ?)", ("prox_vibrate", "101"))
        cursor.execute("INSERT INTO config (key, value) VALUES (?, ?)", ("prox_yellow_led", "111"))
        conn.commit()
    finally:
        if conn:
            conn.close()
    yield str(db_path)

@pytest.mark.integration
def test_mobility_aid_loads_config_and_shuts_down_cleanly(populated_c_test_db):
    executable_path = "./builds/mobility_aid_testable"
    process = None

    try:
        cmd = ["stdbuf", "-o0", executable_path, populated_c_test_db]
    
        process = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )

        output_lines = []

        for line in iter(process.stdout.readline, ''):
            print(line, end='')
            output_lines.append(line)
            if "System running" in line:
                break

        process.send_signal(signal.SIGINT)

        remaining_output, _ = process.communicate(timeout=5)
        print(remaining_output, end='')
        output_lines.append(remaining_output)

        full_output = "".join(output_lines)

        assert "Distance: 10.00 cm" in full_output
        assert "[CONFIG] Loaded prox_vibrate=101" in full_output
        assert "[FEEDBACK] Triggering vibration" in full_output
        assert process.returncode == 0

    finally:
        if process and process.poll() is None:
            process.kill()