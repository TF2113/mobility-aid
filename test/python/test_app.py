import pytest
from flask import url_for
from web.app import create_app

@pytest.fixture
def app():
    app = create_app()
    app.config.update({
        "TESTING": True,
    })
    yield app

@pytest.fixture
def client(app):
    return app.test_client()

@pytest.fixture
def mock_db(mocker):
    mock_get = mocker.patch("web.app.get_config_value")
    mock_set = mocker.patch("web.app.set_config_value")
    return mock_get, mock_set

def test_dashboard_get(client, mock_db):
    mock_get, _ = mock_db
    mock_get.side_effect = [15, 25, "true"]

    response = client.get(url_for("dashboard"))
    assert response.status_code == 200
    assert b'<h1 class="title">Huia Echo</h1>' in response.data
    assert b'value="15"' in response.data
    assert b'value="25"' in response.data
    assert b'name="camera_enabled" value="1"' in response.data

def test_dashboard_post_success(client, mock_db):
    mock_get, mock_set = mock_db
    mock_get.return_value = None

    response = client.post(url_for("dashboard"), data={
        "prox_vibrate": "20",
        "prox_yellow_led": "30",
        "camera_enabled": "on"
    }, follow_redirects=True)

    assert response.status_code == 200
    assert b"Configuration updated successfully!" in response.data
    mock_set.assert_any_call("prox_vibrate", 20)
    mock_set.assert_any_call("prox_yellow_led", 30)
    mock_set.assert_any_call("camera_enabled", True)

def test_dashboard_post_invalid_distance(client, mock_db):
    mock_get, mock_set = mock_db
    mock_get.return_value = None

    response = client.post(url_for("dashboard"), data={
        "prox_vibrate": "30",
        "prox_yellow_led": "20",
        "camera_enabled": "on"
    }, follow_redirects=True)

    assert response.status_code == 200
    assert b"Intermediate distance must be greater than minimum distance." in response.data
    mock_set.assert_not_called()

def test_dashboard_post_invalid_input(client, mock_db):
    mock_get, _ = mock_db
    mock_get.return_value = None

    response = client.post(url_for("dashboard"), data={
        "prox_vibrate": "abc",
        "prox_yellow_led": "30",
    }, follow_redirects=True)

    assert response.status_code == 200
    assert b"Invalid input: Please ensure values are correct." in response.data
    
def test_dashboard_post_empty_warning_input(client, mock_db):
    mock_get, _ = mock_db
    mock_get.return_value = None

    response = client.post(url_for("dashboard"), data={
        "prox_vibrate": "15",
        "prox_yellow_led": "",
    }, follow_redirects=True)

    assert response.status_code == 200
    assert b"Invalid input: Please ensure values are correct." in response.data
    
def test_dashboard_post_empty_alert_input(client, mock_db):
    mock_get, _ = mock_db
    mock_get.return_value = None

    response = client.post(url_for("dashboard"), data={
        "prox_vibrate": "",
        "prox_yellow_led": "20",
    }, follow_redirects=True)

    assert response.status_code == 200
    assert b"Invalid input: Please ensure values are correct." in response.data