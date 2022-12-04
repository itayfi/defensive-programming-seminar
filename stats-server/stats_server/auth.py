import datetime as dt
import functools
import secrets

import bcrypt
from flask import request

from stats_server import models
from stats_server.user_exception import UserException

ACCESS_TOKEN_LIFETIME = dt.timedelta(days=1)
DEFAULT_SALT = bcrypt.gensalt()


class AuthException(UserException):
    def __init__(self, message):
        super().__init__(message, 401)


def verify_password(password, user_hash=None):
    result = bcrypt.checkpw(password.encode("utf8"), user_hash or DEFAULT_SALT)

    if user_hash is None:
        return False

    return result


def generate_token(user):
    user.access_token = secrets.token_hex()
    user.access_token_expires = dt.datetime.utcnow() + ACCESS_TOKEN_LIFETIME

    return user.access_token, user.access_token_expires


def get_access_token():
    authorization_header = request.headers.get("Authorization")
    if not authorization_header:
        return None
    parts = authorization_header.split(" ")
    if len(parts) != 2 or parts[0] != "Bearer":
        return None

    return parts[1]


def with_auth(needs_admin=False):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            access_token = get_access_token()
            if not access_token:
                raise AuthException("Missing access token")
            with models.get_session("auth") as session:
                user = session.query(models.User).filter(models.User.access_token == access_token).one_or_none()
                if user is None or user.access_token_expires < dt.datetime.utcnow():
                    raise AuthException("Invalid access token")
                if needs_admin and not user.is_admin:
                    raise AuthException("No permissions")

            return func(user, *args, **kwargs)
        return wrapper
    return decorator
