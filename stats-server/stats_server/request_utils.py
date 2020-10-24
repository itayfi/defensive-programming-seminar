import datetime as dt

from flask import request

from stats_server.user_exception import UserException


def _get_param(key, type_, required=False):
    if key not in request.json:
        if not required:
            return None
        else:
            raise UserException(f"Missing parameter {key}", 400)

    if not isinstance(request.json[key], type_):
        raise UserException(f"Invalid parameter {key}", 400)

    return request.json[key]



def get_list(key, required=False):
    return _get_param(key, list, required)


def get_string(key, required=False):
    return _get_param(key, str, required)


def get_number(key, required=False):
    return _get_param(key, (int, float), required)


def get_date(key, required=False):
    value = _get_param(key, str, required)

    try:
        return dt.datetime.fromisoformat(value)
    except ValueError:
        raise UserException(f"Invalid parameter {key}", 400)
