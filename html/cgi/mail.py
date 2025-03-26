#!/usr/bin/env python3

import urllib.parse
import os
import smtplib
import sys
import json
import threading
import random
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.image import MIMEImage
from time import sleep

# Fonction pour envoyer un email de confirmation
def send_confirmation_email(user_email, user_name, user_password, userID):
	os.mkdir(f"./uploadServer/{user_name}_{userID}")
	sender_email = "omarsimonraphfbm@gmail.com"
	sender_password = "kesobgtdxzoyyujm"
	subject = "Confirmation d'ajout à la liste des utilisateurs"

	body = f"""
	Bonjour {user_name},
	
	Félicitations ! 🎉 Vous avez été ajouté(e) avec succès à notre liste d'utilisateurs.
	
	À très bientôt !!
	"""

	msg = MIMEMultipart()
	msg['From'] = sender_email
	msg['To'] = user_email
	msg['Subject'] = subject
	msg.attach(MIMEText(body, 'plain'))
	
	try:
		# with open('homer.gif', 'rb') as img_file:
		# 	img = MIMEImage(img_file.read(), _subtype='gif')
		# 	img.add_header('Content-ID', '<image1>')
		# 	msg.attach(img)

		server = smtplib.SMTP('smtp.gmail.com', 587)
		server.starttls()
		server.login(sender_email, sender_password)
		server.sendmail(sender_email, user_email, msg.as_string())
		server.quit()

		add_user_to_json(user_name, user_email, user_password, userID)
		response_http(200, f"", userID, user_name)
	except Exception as e:
		response_http(500, f"", userID, user_name)

# Fonction pour ajouter un utilisateur au fichier JSON
def add_user_to_json(user_name, user_email, user_password, userID):
	try:
		json_file = "json/users.json"

		if not os.path.exists(json_file) or os.stat(json_file).st_size == 0:
			users = []
		else:
			try:
				with open(json_file, "r") as file:
					users = json.load(file)
					if not isinstance(users, list):
						users = []
			except (json.JSONDecodeError, FileNotFoundError):
				users = []
		users.append({
			"name": user_name,
			"email": user_email,
			"password": user_password,
			"userID": userID
		})
		with open(json_file, "w") as file:
			json.dump(users, file, indent=4)

	except Exception as e:
		response_http(500, f"", userID, user_name)


# Fonction pour répondre en HTTP
def response_http(status_code, message, userID, user_name, redirect_url=None,):
	html_content = ""
	status_messages = {
		200: "OK",
		500: "Internal Server Error"
	}
	if status_code == 200:
		redirect_url = "html/succes.html"
	elif status_code == 500:
		redirect_url = "html/conect/errors/errors_500/error_500.html"
	reason = status_messages.get(status_code, "Unknown Status")

	response = f"HTTP/1.1 {status_code} {reason}\r\n"
	try:
		with open(redirect_url, "r") as file:
			html_content = file.read()
	except Exception as e:
		response += ""
	response += f"Content-Type: text/html\r\n"
	response += f"Content-Length: {len(html_content)}\r\n"
	response += "Connection: close\r\n"
	response += "Set-Cookie: " + user_name + "|" + userID + "\r\n\r\n"
	# response += html_content
	print(response)
	print(html_content)

# Vérifier et creer l'ID
def generateID():
	lstElem = "0123456789abcdef"
	userID = "-".join("".join(random.choice(lstElem) for _ in range(8)) for _ in range(4))

	try:
		with open("json/alreadyUsed.txt", "r") as file:
			used_ids = {line.strip() for line in file}
	except FileNotFoundError:
		used_ids = set()

	if userID in used_ids:
		return generateID()

	with open("json/alreadyUsed.txt", "a") as file:
		file.write(f"{userID}\n")

	return userID

# Lecture des données avec un timeout
def get_form_data():
	user_email = None
	user_name = None
	user_password = None
	userID = generateID()

	if (os.getenv("METHOD") == "POST") :
		line = sys.stdin.read()
	elif (os.getenv("METHOD") == "GET") :
		line = os.getenv("FORM")

	if line is None:
		return None, None, None, None

	parsed_data = urllib.parse.parse_qs(line)
	if ("email" in parsed_data) :
		user_email = urllib.parse.unquote(parsed_data.get("email", [""])[0])
	if ("username" in parsed_data) :
		user_name = parsed_data.get("username", [""])[0]
	if ("password" in parsed_data) :
		user_password = parsed_data.get("password", [""])[0][:-1]
	
	return user_name, user_email, user_password, userID

#rend le buff liberable a chaque print
sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', buffering=1)

user_name, user_email, user_password, userID = get_form_data()

if user_name and user_email and user_password and userID:
	# while True:
	# 	sleep(2)
	send_confirmation_email(user_email, user_name, user_password, userID)
else:
	response_http(500, "", userID, user_name)
