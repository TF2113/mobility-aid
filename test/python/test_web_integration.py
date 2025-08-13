import pytest
import sqlite3
from unittest.mock import patch
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

from web.app import create_app
from src.configs.db import Base

@pytest.fixture
def client_with_patched_db(tmp_path):
    db_path = tmp_path / "integration_test.db"
    test_engine = create_engine(f"sqlite:///{db_path}")
    Base.metadata.create_all(test_engine)

    TestingSession = sessionmaker(bind=test_engine)
    test_session = TestingSession()

    with patch('src.configs.db.session', test_session):
        app = create_app()
        app.config.update({"TESTING": True, "WTF_CSRF_ENABLED": False})
        yield app.test_client(), str(db_path)

    test_session.close()

@pytest.mark.integration
def test_app_successfully_updates_database(client_with_patched_db):
    client, db_path = client_with_patched_db

    form_data = {
        'prox_vibrate': '20',
        'prox_yellow_led': '999',
        'camera_enabled': 'on'
    }

    response = client.post('/', data=form_data)
    assert response.status_code == 302

    conn = None
    try:
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()
        cursor.execute("SELECT value FROM config WHERE key = ?", ("prox_yellow_led",))
        result = cursor.fetchone()
    finally:
        if conn:
            conn.close()
    
    assert result is not None
    assert result[0] == '999'