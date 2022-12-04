from contextlib import contextmanager
import datetime as dt

import sqlalchemy
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from sqlalchemy import Column, Boolean, Numeric, Integer, String, DateTime

from stats_server.config import get_user_details


Base = declarative_base()

class User(Base):
    __tablename__ = "users"

    id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False, unique=True)
    password_hash = Column(String, nullable=False)
    is_admin = Column(Boolean, default=False, nullable=False)
    access_token = Column(String, index=True)
    access_token_expires = Column(DateTime)

    def __repr__(self):
        return f"<User #{self.id}: {self.name}>"


class Statistic(Base):
    __tablename__ = "statistics"

    id = Column(Integer, primary_key=True)
    tag = Column(String)
    value = Column(Numeric, nullable=False)
    timestamp = Column(DateTime, nullable=False, default=dt.datetime.utcnow)

    def __repr__(self):
        return f"<Statistic #{self.id} {self.tag}@{self.timestamp.isoformat()}={self.value}>"


@contextmanager
def get_session(role):
    user_details = get_user_details(role)
    engine = sqlalchemy.create_engine("postgres://{user}:{password}@localhost/statsdb".format(**user_details))
    Session = sessionmaker(bind=engine)
    session = Session()
    try:
        yield session
    except:
        session.rollback()
        raise
    else:
        session.commit()
    finally:
        session.close()
