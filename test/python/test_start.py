import pytest
import start
from unittest.mock import MagicMock

@pytest.fixture
def mock_dependencies(mocker):
    mock_get_config = mocker.patch("start.get_config_value")

    mock_popen = mocker.patch("subprocess.Popen")
    mock_sleep = mocker.patch("time.sleep")
    mock_os_path_exists = mocker.patch("os.path.exists")
    mock_os_remove = mocker.patch("os.remove")
    
    mock_popen.return_value = MagicMock()

    return {
        "popen": mock_popen,
        "sleep": mock_sleep,
        "get_config": mock_get_config,
        "os_path_exists": mock_os_path_exists,
        "os_remove": mock_os_remove
    }

def test_process_changes_enables_inference(mock_dependencies):
    start.inference = None
    mock_dependencies["get_config"].return_value = "true"

    start.process_changes()

    mock_dependencies["popen"].assert_called_once()
    assert start.inference is not None

def test_process_changes_disables_inference(mock_dependencies):
    mock_inference_process = MagicMock()
    start.inference = mock_inference_process
    mock_dependencies["get_config"].return_value = "false"
    mock_dependencies["os_path_exists"].return_value = True

    start.process_changes()

    mock_inference_process.terminate.assert_called_once()
    mock_inference_process.wait.assert_called_once()
    mock_dependencies["os_remove"].assert_called_once_with(start.img_path)
    assert start.inference is None

def test_process_changes_handles_none_config_value(mock_dependencies):
    mock_inference_process = MagicMock()
    start.inference = mock_inference_process
    mock_dependencies["get_config"].return_value = None
    mock_dependencies["os_path_exists"].return_value = True

    start.process_changes()

    mock_inference_process.terminate.assert_called_once()
    assert start.inference is None

def test_process_changes_disables_without_deleting_nonexistent_file(mock_dependencies):
    mock_inference_process = MagicMock()
    start.inference = mock_inference_process
    mock_dependencies["get_config"].return_value = "false"
    mock_dependencies["os_path_exists"].return_value = False

    start.process_changes()

    mock_inference_process.terminate.assert_called_once()
    mock_dependencies["os_remove"].assert_not_called()

def test_detect_changes_calls_process_changes_on_update(mocker, mock_dependencies):
    mock_dependencies["get_config"].side_effect = ["false", "false", "true"]
    mock_dependencies["sleep"].side_effect = [None, None, StopIteration]

    mock_process_changes = mocker.patch("start.process_changes")

    with pytest.raises(StopIteration):
        start.detect_changes(poll_interval=0.01)

    assert mock_process_changes.call_count == 2