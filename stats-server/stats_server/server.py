from flask import Flask
from flask import jsonify
from flask import request

import auth
import models
from user_exception import UserException


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

    return jsonify(message="Wrong username or password",
                   success=False), 401

if __name__ == "__main__":
    app.run(port=8002, ssl_context="adhoc")
