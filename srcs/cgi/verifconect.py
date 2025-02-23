import cgi
import json

# Fonction pour charger les utilisateurs depuis le fichier JSON
def load_users():
    try:
        with open('json/users.json', 'r') as file:
            return json.load(file)  # Charge la liste d'utilisateurs
    except FileNotFoundError:
        return []

# Fonction pour vérifier le nom d'utilisateur et le mot de passe
def validate_user(username, password):
    users = load_users()  # Charger les utilisateurs depuis le fichier
    for user in users:
        if user["name"] == username and user["password"] == password:
            return True  # Correspondance trouvée
    return False

# Initialisation de la réponse HTTP
print("Content-Type: text/html\n")

# Récupérer les données du formulaire
form = cgi.FieldStorage()
username = form.getvalue("user")
password = form.getvalue("password")

print(username)  # Debug
print(password)  # Debug

# Vérifier la validité des données
if username and password:
    if validate_user(username, password):
        print("<h2>Connexion réussie</h2>")
        print('<a href="/dashboard.html">Accéder à votre tableau de bord</a>')
    else:
        print("<h2>Nom d'utilisateur ou mot de passe incorrect</h2>")
        print('<a href="creat_user.html">Créer un compte</a>')
else:
    print("<h2>Veuillez entrer un nom d'utilisateur et un mot de passe</h2>")
    print('<a href="creat_user.html">Créer un compte</a>')
