from sqlalchemy import create_engine, Column, Integer, String
from sqlalchemy.orm import sessionmaker, declarative_base

Base = declarative_base()

class Config(Base):
    __tablename__ = "config"
    id = Column(Integer, primary_key=True)
    key = Column(String, unique=True, nullable=False)
    value = Column(String, nullable=False)
    
engine = create_engine("sqlite:///./src/configs/config.db")
Session = sessionmaker(bind=engine)
session = Session()
Base.metadata.create_all(engine)

def get_config_value(key):
    item = session.query(Config).filter_by(key=key).first()
    return item.value if item else None

def set_config_value(key, value):
    item = session.query(Config).filter_by(key=key).first()
    if item:
        item.value = str(value)
    else:
        item = Config(key=key, value=str(value))
        session.add(item)
    session.commit()