#!/usr/bin/python3

import cgi
import json
import os

json_file = "./json/user.jon";
form = cgi.fieldStorage()
username = form.getvalue("username");
password = form.getvalue("password");


if os.path.exists(json_file):
    with open(json_file, "r") as f:
        users = json.load(f)
else:
    users = {}

if username and password:
	users[username] = password

	with open(json_file, "w") as f:
		json.dump(users, f, indent=4)

	print("Content-Type: text/html\n")
	print("<html><body>")
	print(f"<h2>Utilisateur '{username}' ajouté avec succès !</h2>")
	print("</body></html>")
else:
    print("Content-Type: text/html\n")
    print("<html><body>")
    print("<h2>Nom d'utilisateur ou mot de passe manquant</h2>")
    print("</body></html>")
