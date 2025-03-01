#!/usr/bin/env python3

import urllib.parse
import os
import sys
import json
import threading


# Fonction de lecture avec timeout
def read_input_with_timeout(timeout):
    result = []
    thread = threading.Thread(target=lambda: result.append(sys.stdin.readline().strip()), daemon=True)
    thread.start()
    thread.join(timeout)
    if thread.is_alive():
        return None
    else:
        return result[0] if result else None

# Fonction pour vérifier l'utilisateur dans le fichier JSON
def verif_json(user_name, user_password):
    try:
        json_file = "json/users.json"

        if not os.path.exists(json_file) or os.stat(json_file).st_size == 0:
            return False
        
        try:
            with open(json_file, "r") as file:
                users = json.load(file)
                if not isinstance(users, list):
                    users = []
        except (json.JSONDecodeError, FileNotFoundError):
            return False
    
        for user in users:
            if user['name'] == user_name and user['password'] == user_password:
                return True
        
        return False

    except Exception as e:
        return False

# Fonction pour répondre en HTTP
def response_http(status_code, message, redirect_url=None):
    html_content = ""
    status_messages = {
        500: "Internal Server Error",
        200: "OK"
    }
    if status_code == 500:
        redirect_url = "html/conect/errors/errors_500/error_500.html"
    elif status_code == 200:
        redirect_url = "html/conect/index.html"

    reason = status_messages.get(status_code, "Unknown Status")

    response = f"HTTP/1.1 {status_code} {reason}\r\n"
    try:
        with open(redirect_url, "r") as file:
            html_content = file.read()
    except Exception as e:
        response += f""
    
    response += f"Content-Type: text/html\r\n"
    response += f"Content-Length: {len(html_content)}\r\n"
    response += "Connection: close\r\n\r\n"
    response += html_content
    print(response)

# Lecture des données avec un timeout
def get_form_data_with_timeout():
    user_name = None
    user_password = None
    
    while True:
        line = read_input_with_timeout(0.1)
        if 'user=' in line:
            parsed_data = urllib.parse.parse_qs(line)
            user_name = parsed_data.get("user", [""])[0]
            user_password = parsed_data.get("password", [""])[0]
            break
        if line is None:
            break
 
    return user_name, user_password

user_name, user_password = get_form_data_with_timeout()

if user_name and user_password:
    if verif_json(user_name, user_password):
        response_http(200, "")
    else:
        response_http(500, "")
else:
    response_http(500, "")
