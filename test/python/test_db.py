import pytest
from unittest.mock import patch
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

from src.configs.db import get_config_value, set_config_value, Base

@pytest.fixture(scope="function")
def db_session(tmp_path):
    db_file = tmp_path / "test_config.db"
    engine = create_engine(f"sqlite:///{db_file}")
    Base.metadata.create_all(engine)

    TestingSessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)
    session = TestingSessionLocal()

    with patch('src.configs.db.session', session):
        yield session

    session.close()

def test_set_and_get_new_key(db_session):
    set_config_value("prox_vibrate", 50)

    retrieved_value = get_config_value("prox_vibrate")
    assert retrieved_value == "50"

def test_empty_key_returns_none(db_session):
    assert get_config_value("empty") is None

def test_set_updates_existing_key(db_session):
    set_config_value("camera_enabled", "True")
    initial_value = get_config_value("camera_enabled")
    assert initial_value == "True"

    set_config_value("camera_enabled", "False")

    updated_value = get_config_value("camera_enabled")
    assert updated_value == "False"

def test_set_config_value_with_integer(db_session):
    set_config_value("prox_yellow_led", 123)

    retrieved_value = get_config_value("prox_yellow_led")
    assert retrieved_value == "123"
    assert isinstance(retrieved_value, str)

def test_data_is_saved(db_session):
    set_config_value("test_data", "data_is_saved")
    engine = db_session.get_bind()

    with sessionmaker(bind=engine)() as verification_session:
        from src.configs.db import Config
        item = verification_session.query(Config).filter_by(key="test_data").first()

        assert item is not None
        assert item.value == "data_is_saved"