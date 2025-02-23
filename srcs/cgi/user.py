#!/usr/bin/python3
import smtplib
import json
import cgi
import os
import time
# from email.mime.text import MIMEText
# from email.mime.multipart import MIMEMultipart

# # Fonction qui attend et traite les requêtes CGI
# def handle_request():
#     form = cgi.FieldStorage()

#     # Récupérer les valeurs du formulaire
#     user_name = form.getvalue("username")
#     user_email = form.getvalue("email")
#     user_password = form.getvalue("password")

#     if user_name and user_email and user_password:
#         # Si tout est bon, envoyer un email et ajouter l'utilisateur au fichier JSON
#         message = send_confirmation_email(user_email, user_name, user_password)

#         # Charger la page de succès existante
#         success_page = load_html_page('./html/index.html')
#         print(generate_http_response(200, success_page))
#     # else:
#     #     # Si des informations sont manquantes, envoyer une page d'erreur
#     #     error_page = load_html_page('./html/error.html')
#     #     print(generate_http_response(400, error_page))

# # Fonction pour envoyer un email de confirmation
# def send_confirmation_email(user_email, user_name, user_password):
#     sender_email = "omarsimonraphfbm@gmail.com"
#     sender_password = "kesobgtdxzoyyujm"
#     subject = "Confirmation d'ajout à la liste des utilisateurs"
#     body = f"Bonjour {user_name},\n\nNous vous confirmons que vous avez bien été ajouté à la liste des utilisateurs.\n\nMerci de votre inscription."

#     msg = MIMEMultipart()
#     msg['From'] = sender_email
#     msg['To'] = user_email
#     msg['Subject'] = subject
#     msg.attach(MIMEText(body, 'plain'))

#     try:
#         # Connexion au serveur SMTP
#         server = smtplib.SMTP('smtp.gmail.com', 587)
#         server.starttls()
#         server.login(sender_email, sender_password)
#         server.sendmail(sender_email, user_email, msg.as_string())
#         server.quit()
        
#         # Ajouter l'utilisateur au fichier JSON
#         add_user_to_json(user_name, user_email, user_password)
        
#         # Si tout va bien, renvoyer la page de succès
#         return "Utilisateur ajouté et email envoyé avec succès."

#     except Exception as e:
#         # En cas d'erreur, renvoyer une page d'erreur
#         return f"Erreur : {e}"

# # Fonction pour ajouter un utilisateur au fichier JSON
# def add_user_to_json(user_name, user_email, user_password):
#     try:
#         try:
#             with open('./json/users.json', 'r') as file:
#                 users = json.load(file)
#         except FileNotFoundError:
#             users = []
        
#         users.append({
#             "name": user_name,
#             "email": user_email,
#             "password": user_password
#         })

#         with open('./json/users.json', 'w') as file:
#             json.dump(users, file, indent=4)

#     except Exception as e:
#         raise Exception(f"Erreur lors de l'ajout dans le fichier JSON : {e}")

# # Fonction pour charger le contenu d'une page HTML
# def load_html_page(file_path):
#     try:
#         with open(file_path, 'r') as file:
#             return file.read()
#     except Exception as e:
#         return f"<html><body><h1>Erreur lors du chargement de la page</h1><p>{str(e)}</p></body></html>"

# # Fonction pour générer la réponse HTTP
# def generate_http_response(status_code, html_content):
#     if status_code == 200:
#         status_message = "OK"
#     elif status_code == 500:
#         status_message = "Internal Server Error"
#     else:
#         status_message = "Bad Request"
    
#     response = f"HTTP/1.1 {status_code} {status_message}\r\n"
#     response += "Content-Type: text/html\r\n"
#     response += "Connection: close\r\n"
#     response += "\r\n"  # Séparation entre le header et le body
#     response += html_content

#     return response

# # Boucle infinie pour attendre une requête avec un timeout
# def run_server(timeout=10):
#     start_time = time.time()  # Moment où on commence à attendre
#     while True:
#         current_time = time.time()
#         elapsed_time = current_time - start_time
        
#         # Vérifie si le timeout est dépassé
#         if elapsed_time > timeout:
#             print("Le temps d'attente a expiré, arrêt du serveur.")
#             break  # Quitter la boucle après le timeout

#         handle_request()  # Attendre et traiter une nouvelle requête

# run_server(timeout=10)  # Timeout de 30 secondes (modifiable)

# Exemple de script Python pour générer une page HTML


#!/usr/bin/env python3



import sys
import time

data = sys.stdin.read();



html_content = """
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Connexion Réussie</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #e7f7e7;
            text-align: center;
            padding: 50px;
        }
        h1 {
            color: #4CAF50;
        }
        p {
            color: #333;
        }
        .success-box {
            background-color: #d4edda;
            color: #155724;
            padding: 20px;
            border: 1px solid #c3e6cb;
            display: inline-block;
            margin-top: 20px;
        }
    </style>
</head>
<body>

    <h1>Connexion réussie</h1>

    <div class="success-box">
        <p><strong>Félicitations !</strong> Vous êtes maintenant connecté.</p>
        <p>Merci de vous être connecté avec succès. Vous pouvez maintenant accéder à votre compte.</p>
    </div>

</body>
</html>
"""

# Construction de la réponse HTTP avec en-têtes pour code 200
response = "HTTP/1.1 200 OK\r\n"  # Code 200 pour OK
response += "Content-Type: text/html; charset=UTF-8\r\n"  # Type du contenu
# response += "Content-Length: {}\r\n".format(len(html_content))  # Longueur du contenu
response += "Connection: close\r\n"  # Connexion fermée après la réponse
response += "\r\n"  # Ligne vide pour séparer l'en-tête du corps


time.sleep(2)

print(response)
  # Corps de la réponse contenant du HTML

# Afficher la réponse complète (en-tête + corps)
print(html_content)


