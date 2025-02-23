#!/usr/bin/env python3


# import smtplib
# import cgi
# import sys
# import json
# from email.mime.text import MIMEText
# from email.mime.multipart import MIMEMultipart
# from email.mime.image import MIMEImage

# def send_confirmation_email(user_email, user_name, user_password):
#     sender_email = "omarsimonraphfbm@gmail.com"  # Remplace avec ton adresse email
#     sender_password = "kesobgtdxzoyyujm"  # ⚠️ NE JAMAIS METTRE LE MOT DE PASSE ICI ! (utilise plutôt un mot de passe d'application)
#     subject = "Confirmation d'ajout à la liste des utilisateurs"

#     body = f"""
#     Bonjour {user_name},
    
#     Félicitations ! 🎉 Vous avez été ajouté(e) avec succès à notre liste d'utilisateurs.
    
#     À très bientôt !!
#     """

#     msg = MIMEMultipart()
#     msg['From'] = sender_email
#     msg['To'] = user_email
#     msg['Subject'] = subject
#     msg.attach(MIMEText(body, 'plain'))
    
#     try:
#         with open('homer.gif', 'rb') as img_file:
#             img = MIMEImage(img_file.read(), _subtype='gif')
#             img.add_header('Content-ID', '<image1>')
#             msg.attach(img)
        
#         server = smtplib.SMTP('smtp.gmail.com', 587)
#         server.starttls()
#         server.login(sender_email, sender_password)
#         server.sendmail(sender_email, user_email, msg.as_string())
#         server.quit()
        
#         add_user_to_json(user_name, user_email, user_password)
#         response_http(200, f"L'email a été envoyé avec succès à {user_email}.")
#     except Exception as e:
#         response_http(500, f"Une erreur est survenue : {e}")

# def add_user_to_json(user_name, user_email, user_password):
#     try:
#         try:
#             with open('json/users.json', 'r') as file:
#                 users = json.load(file)
#         except FileNotFoundError:
#             users = []
        
#         users.append({
#             "name": user_name,
#             "email": user_email,
#             "password": user_password
#         })
        
#         with open('json/users.json', 'w') as file:
#             json.dump(users, file, indent=4)
#     except Exception as e:
#         response_http(500, f"Problème lors de la mise à jour du fichier JSON : {e}")

# def response_http(status_code, message):
#     print(f"Status: {status_code} \r\n")
#     print("Content-Type: application/json\r\n")
#     print("\r\n")
#     print(json.dumps({"status": status_code, "message": message}))
# form = cgi.FieldStorage()
# print(form)
# user_email = form.getvalue("email")
# user_name = form.getvalue("username")
# user_password = form.getvalue("password")
# print(user_password)
# print(user_name)
# print(user_email)


# if user_name and user_email and user_password:
#     send_confirmation_email(user_email, user_name, user_password)
# else:
#     response_http(400, "Informations manquantes dans la requête POST")

import smtplib
import cgi
import sys
import json
import threading
import time
import random
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.image import MIMEImage

# Fonction de lecture avec timeout
def read_input_with_timeout(timeout):
    result = []
    
    # Démarre un thread pour lire depuis stdin
    thread = threading.Thread(target=lambda: result.append(sys.stdin.readline().strip()), daemon=True)
    thread.start()

    # Attends que le thread termine ou que le timeout expire
    thread.join(timeout)

    if thread.is_alive():
        return None
    else:
        return result[0] if result else None

# Fonction pour envoyer un email de confirmation
def send_confirmation_email(user_email, user_name, user_password, userID):
    sender_email = "omarsimonraphfbm@gmail.com"  # Remplace avec ton adresse email
    sender_password = "kesobgtdxzoyyujm"  # ⚠️ NE JAMAIS METTRE LE MOT DE PASSE ICI ! (utilise plutôt un mot de passe d'application)
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
        with open('homer.gif', 'rb') as img_file:
            img = MIMEImage(img_file.read(), _subtype='gif')
            img.add_header('Content-ID', '<image1>')
            msg.attach(img)
        
        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.starttls()
        server.login(sender_email, sender_password)
        server.sendmail(sender_email, user_email, msg.as_string())
        server.quit()
        
        add_user_to_json(user_name, user_email, user_password, userID)
        response_http(200, f"L'email a été envoyé avec succès à {user_email}.\n")
    except Exception as e:
        response_http(500, f"Une erreur est survenue : {e}\n")

# Fonction pour ajouter un utilisateur au fichier JSON
def add_user_to_json(user_name, user_email, user_password, userID):
    try:
        try:
            with open('json/users.json', 'r') as file:
                users = json.load(file)
        except FileNotFoundError:
            users = []
        
        users.append({
            "name": user_name,
            "email": user_email,
            "password": user_password,
            "userID": userID
        })
        
        with open('json/users.json', 'w') as file:
            json.dump(users, file, indent=4)
    except Exception as e:
        response_http(500, f"Problème lors de la mise à jour du fichier JSON : {e}\n")

# Fonction pour répondre en HTTP
def response_http(status_code, message, redirect_url=None):
    status_messages = {
        200: "OK",
        400: "Bad Request",
        500: "Internal Server Error"
    }
    if status_code == 200:
        os.system("xdg-open success.html")
        redirect_url = "/success.html"
    elif status_code == 400:
        redirect_url = "400.html"
    elif status_code == 500:
        redirect_url = "500.html"
    reason = status_messages.get(status_code, "Unknown Status")

    response = f"HTTP/1.1 {status_code} {reason}\r\n"
    if redirect_url:
        response += f"Location: {redirect_url}\r\n"

    response += f"Content-Type: text/html\r\n"
    response += f"Content-Length: {len(message)}\r\n"
    response += message
    response += "Connection: close\r\n\r\n"

    print(response)

def generateID():
    lstElem = "0123456789abcdef"
    userId = str([lstElem[random.randint(0, 15)] for i in range 8]) + "-" + str([lstElem[random.randint(0, 15)] for i in range 8]) + "-" str([lstElem[random.randint(0, 15)] for i in range 8]) + "-" str([lstElem[random.randint(0, 15)] for i in range 8])
    file = open(alreadyUsed.txt)
    for line in file:
        if line == userID:
            return (generateID())
    file.write(f"{userID}\n")
    file.close()
    return userID

# Lecture des données avec un timeout
def get_form_data_with_timeout():
    user_email = "sim@bd"
    user_name = "saim"
    user_password = "kat"
    userID = generateID()
    
    while True:
        line = read_input_with_timeout(0.1)  # Timeout de 100ms pour chaque ligne
        if line is None:
            break  # Sortie de la boucle si aucun input n'est reçu dans le temps imparti

        # Recherche des informations pertinentes dans chaque ligne
        # if 'email=' in line:
        #     user_email = line.split('=')[1]
        # elif 'username=' in line:
        #     user_name = line.split('=')[1]
        # elif 'password=' in line:
        #     user_password = line.split('=')[1]
        
        # # # Si toutes les informations sont trouvées, on arrête la boucle
        # if user_email and user_name and user_password:
        #     break
#     print(line, "line")
#    // print("s" + user_name)
#     //print("s" + user_password)
    # print(line, "line")
    # print("Email trouvé:", user_email)
    # print("Username trouvé:", user_name)
    # print("Password trouvé:", user_password)
    
    # return user_name, user_email, user_password
    # Utilisation de la fonction pour lire les paramètres du formulaire avec un timeout

    return user_name, user_email, user_password, userID

# Récupération des données utilisateur
user_name, user_email, user_password, userID = get_form_data_with_timeout()

# Vérification si toutes les informations sont présentes
if user_name and user_email and user_password and userID:
    send_confirmation_email(user_email, user_name, user_password, userID)
else:
    response_http(400, "Informations manquantes dans la requête POST ou GET.\n")
