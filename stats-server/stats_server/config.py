import json
import os.path

def _get_config():
    with open(os.path.join(os.path.dirname(__file__), "config.json")) as config_file:
        return json.load(config_file)

CONFIG = _get_config()


def get_user_details(role):
    user_name = f"{role}_user"
    if user_name not in CONFIG.get("db_auth", {}):
        raise Exception(f"Given role {role} has no authorization details")
    
    return {
        "user": user_name,
        "password": CONFIG["db_auth"][user_name]
    }
