from flask import Flask
from flask import jsonify
from flask import request
import sqlalchemy as sa

from stats_server import auth
from stats_server import models
from stats_server import request_utils
from stats_server.user_exception import UserException


app = Flask(__name__)

@app.before_first_request
def init_db():
    models.create_all()


@app.errorhandler(UserException)
def handle_user_error(error):
    return jsonify(message=error.message,
                   success=False), error.status


@app.errorhandler(500)
def handle_generic_error(error):
    return jsonify(message="An unexpected error has occurred",
                   success=False), 500


@app.route("/")
def index():
    return jsonify({
        "name": "Stats Server API",
        "version": "1.0.0"
    })


@app.route("/login", methods=["POST"])
def login():
    with models.get_session() as session:
        user = session.query(models.User).filter(models.User.name == request.json.get("user")).first()
        password = request.json.get("password", "")

        if auth.verify_password(password, user.password_hash if user else None):
            access_token, access_token_expires = auth.generate_token(user)
            return jsonify(access_token=access_token,
                           access_token_expires=access_token_expires,
                           success=True)

    raise auth.AuthException("Wrong username or password")


@app.route("/stats", methods=["POST"])
@auth.with_auth(needs_admin=True)
def add_stats(user):
    with models.get_session() as session:
        session.add(models.Statistic(
            tag=request_utils.get_string("tag", required=True),
            value=request_utils.get_number("value", required=True)
        ))

    return jsonify(success=True)


@app.route("/query", methods=["POST"])
@auth.with_auth()
def get_stats(user):
    AGGREGATION_FUNCTIONS = {
        "count": sa.func.count().label("count"),
        "sum": sa.func.sum(models.Statistic.value).label("sum"),
        "avg": sa.func.avg(models.Statistic.value).label("avg"),
        "min": sa.func.min(models.Statistic.value).label("min"),
        "max": sa.func.max(models.Statistic.value).label("max"),
        "first": sa.func.min(models.Statistic.timestamp).label("first"),
        "last": sa.func.max(models.Statistic.timestamp).label("last")
    }

    with models.get_session() as session:
        try:
            aggregations = [AGGREGATION_FUNCTIONS[agg] for agg in request_utils.get_list("aggregations", required=True)]
        except KeyError as err:
            raise UserException(f"Unknown aggregation '{err.args[0]}'", 400)

        query = session.query(models.Statistic.tag, *aggregations)
        query = query.group_by(models.Statistic.tag)
        if "tag" in request.json:
            query = query.filter(models.Statistic.tag == request_utils.get_string("tag"))
        if "since" in request.json:
            query = query.filter(models.Statistic.timestamp > request_utils.get_date("since"))
        if "until" in request.json:
            query = query.filter(models.Statistic.timestamp < request_utils.get_date("until"))
        results = query.all()

    return jsonify(
        results=[{key: str(getattr(item, key)) for key in item.keys()} for item in results],
        success=True
    )


@app.route("/users", methods=["POST"])
@auth.with_auth(needs_admin=True)
def add_user(user):
    username = request_utils.get_string("username", required=True)
    with models.get_session() as session:
        if session.query(models.User).filter_by(name=username).count() > 0:
            raise UserException(f"User {username} already exists", 400)

    password = request_utils.get_string("password", required=True)
    password_hash = bcrypt.hashpw(password, bcrypt.gensalt())
    with models.get_session() as session:
        session.add(models.User(
            name=username,
            password_hash=password_hash
        ))

    return jsonify(success=True)


if __name__ == "__main__":
    app.run(port=8002, ssl_context="adhoc")
