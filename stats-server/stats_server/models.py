from contextlib import contextmanager

import bcrypt
import sqlalchemy
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from sqlalchemy import Column, Boolean, Numeric, Integer, String, DateTime


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
    timestamp = Column(DateTime, nullable=False)

    def __repr__(self):
        return f"<Statistic #{self.id} {self.tag}@{self.timetamp.isoformat()}={self.value}>"


engine = sqlalchemy.create_engine("sqlite:///stats.db", echo=True)
Session = sessionmaker(bind=engine)


@contextmanager
def get_session():
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


def create_all():
    Base.metadata.create_all(engine)
    with get_session() as session:
        if session.query(User).count() > 0:
            return
        session.add(User(name="admin", password_hash=bcrypt.hashpw(b"password", bcrypt.gensalt()), is_admin=True))

