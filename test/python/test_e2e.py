import pytest
import subprocess
import time
import requests
import os
import signal

@pytest.fixture(scope="module")
def full_system_runner():
    start_process = None
    try:
        env = os.environ.copy()
        env["MOBILITY_AID_EXECUTABLE"] = "./builds/mobility_aid_e2e"

        print("\n Starting E2E System ")
        start_process = subprocess.Popen(
            ["python3", "start.py"],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            env=env
        )

        print("Waiting for system to initialize...")
        time.sleep(5) 
        yield start_process

    finally:
        print("\n E2E System Shutting Down")
        if start_process and start_process.poll() is None:
            start_process.send_signal(signal.SIGINT)
            try:
                start_process.wait(timeout=5)
                print("System shut down gracefully.")
            except subprocess.TimeoutExpired:
                print("System did not shut down gracefully, force killing.")
                start_process.kill()

@pytest.mark.e2e
@pytest.mark.slow
def test_e2e_web_config_change_affects_c_program_logic(full_system_runner):
    print("Sending POST request to change prox_vibrate to 5...")
    new_config = {
        'prox_vibrate': '5',
        'prox_yellow_led': '8'
    }
    try:
        response = requests.post("http://127.0.0.1:5000/", data=new_config, timeout=2)
        assert response.status_code == 200
    except requests.exceptions.ConnectionError:
        pytest.fail("E2E test failed: Could not to web app.")
        
    print("Waiting for C program to reload config...")
    time.sleep(3)

    full_system_runner.send_signal(signal.SIGINT)
    try:
        output, _ = full_system_runner.communicate(timeout=5)
    except subprocess.TimeoutExpired:
        output = "Test timed out trying to get final output."
    
    print(f"--- Full System Output ---\n{output}\n--------------------------")
    
    config_reload_line = "[CONFIG] Loaded prox_vibrate=5, prox_yellow_led=8"
    assert config_reload_line in output, "C program did not log the config reload."

    try:
        before_change, after_change = output.split(config_reload_line, 1)
    except ValueError:
        pytest.fail(f"Could not split log output on the line: {config_reload_line}")

    assert "[FEEDBACK] Triggering vibration" in before_change, \
        "Vibration feedback was NOT detected before the config change."

    assert "[FEEDBACK] Triggering vibration" not in after_change, \
        "Vibration feedback was detected AFTER the config change, but it should have stopped."