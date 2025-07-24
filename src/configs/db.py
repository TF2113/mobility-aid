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