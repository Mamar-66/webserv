

# import smtplib
# import cgi
# import sys
# import json
# from email.mime.text import MIMEText
# from email.mime.multipart import MIMEMultipart

# def send_confirmation_email(user_email):
#     sender_email = "omarsimonraphfbm@gmail.com"  # Remplace avec ton adresse email
#     sender_password = "kesobgtdxzoyyujm"  # ⚠️ NE JAMAIS METTRE LE MOT DE PASSE ICI ! (utilise plutôt un mot de passe d'application)
#     subject = "Confirmation d'ajout à la liste des utilisateurs"
#     body = f"Bonjour,\n\nNous vous confirmons que vous avez bien été ajouté à la liste des utilisateurs.\n\nMerci de votre inscription."

#     msg = MIMEMultipart()
#     msg['From'] = sender_email
#     msg['To'] = user_email
#     msg['Subject'] = subject
#     msg.attach(MIMEText(body, 'plain'))

#     try:
#         server = smtplib.SMTP('smtp.gmail.com', 587)
#         server.starttls()  
#         server.login(sender_email, sender_password)
#         server.sendmail(sender_email, user_email, msg.as_string())
#         server.quit()

#         # Ajout de l'email dans le fichier JSON
#         add_user_to_json(user_email)

#         print("Content-Type: text/plain\r\n")
#         print(f"Email envoyé à {user_email}")
#     except Exception as e:
#         print("Content-Type: text/plain\r\n")
#         print(f"Erreur : {e}")

# def add_user_to_json(user_email):
#     try:
#         # Charger le fichier JSON existant ou créer une nouvelle liste d'utilisateurs
#         try:
#             with open('json/users.json', 'r') as file:
#                 users = json.load(file)
#         except FileNotFoundError:
#             users = []
#             print("Le fichier users.json n'existe pas, il va être créé.")

#         # Ajouter l'email à la liste des utilisateurs
#         users.append({"email": user_email})

#         # Sauvegarder la liste mise à jour dans le fichier JSON
#         with open('json/users.json', 'w') as file:
#             json.dump(users, file, indent=4)
#             print("Le fichier users.json a été mis à jour.")

#     except Exception as e:
#         print(f"Erreur lors de l'ajout de l'email dans le fichier JSON : {e}")

# # 📩 Récupération de l'email envoyé en `POST`
# form = cgi.FieldStorage()
# user_email = form.getvalue("email")

# if user_email:
#     send_confirmation_email(user_email)
# else:
#     print("Content-Type: text/plain\r\n")
#     print("Erreur : Aucun email reçu")

import smtplib
import cgi
import sys
import json
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

def send_confirmation_email(user_email, user_name, user_password):
    sender_email = "omarsimonraphfbm@gmail.com"  # Remplace avec ton adresse email
    sender_password = "kesobgtdxzoyyujm"  # ⚠️ NE JAMAIS METTRE LE MOT DE PASSE ICI ! (utilise plutôt un mot de passe d'application)
    subject = "Confirmation d'ajout à la liste des utilisateurs"
    body = f"Bonjour {user_name},\n\nNous vous confirmons que vous avez bien été ajouté à la liste des utilisateurs.\n\nMerci de votre inscription."

    msg = MIMEMultipart()
    msg['From'] = sender_email
    msg['To'] = user_email
    msg['Subject'] = subject
    msg.attach(MIMEText(body, 'plain'))

    try:
        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.starttls()  
        server.login(sender_email, sender_password)
        server.sendmail(sender_email, user_email, msg.as_string())
        server.quit()

        # Ajout de l'utilisateur dans le fichier JSON
        add_user_to_json(user_name, user_email, user_password)

        print("Content-Type: text/plain\r\n")
        print(f"Email envoyé à {user_email}")
    except Exception as e:
        print("Content-Type: text/plain\r\n")
        print(f"Erreur : {e}")

def add_user_to_json(user_name, user_email, user_password):
    try:
        # Charger le fichier JSON existant ou créer une nouvelle liste d'utilisateurs
        try:
            with open('json/users.json', 'r') as file:
                users = json.load(file)
        except FileNotFoundError:
            users = []
            print("Le fichier users.json n'existe pas, il va être créé.")

        # Ajouter l'utilisateur à la liste des utilisateurs
        users.append({
            "name": user_name,
            "email": user_email,
            "password": user_password
        })

        # Sauvegarder la liste mise à jour dans le fichier JSON
        with open('json/users.json', 'w') as file:
            json.dump(users, file, indent=4)
            print("Le fichier users.json a été mis à jour.")

    except Exception as e:
        print(f"Erreur lors de l'ajout de l'utilisateur dans le fichier JSON : {e}")

# 📩 Récupération des données envoyées en `POST`
form = cgi.FieldStorage()

# Affichage des clés et valeurs pour debug
for key in form.keys():
    print(f"Key: {key}, Value: {form.getvalue(key)}")
user_email = form.getvalue("email")
user_name = form.getvalue("username")
print(user_name)
print(user_email)
user_password = form.getvalue("password")
print(user_password)

if user_name and user_email and user_password:
    send_confirmation_email(user_email, user_name, user_password)
else:
    print("Content-Type: text/plain\r\n")
    print("Erreur : Informations manquantes dans la requête POST")
